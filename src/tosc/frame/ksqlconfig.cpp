#include "ksqlconfig.h"
#include <QVariant>
#include <QDir>
#include <QSqlDriver>
#include <QDebug>

static int CDBSNO = 0;

#define K_ERROR qDebug() << 

/* 
	获取查询的记录数,注意:
	获取记录数后,如果有数据则记录指针指向第一条记录 
*/
int KSQLConfig::getRecordCount(QSqlQuery* query)
{
	int nCount = 0;
	if (query->driver()->hasFeature(QSqlDriver::QuerySize))
	{
		nCount = query->size();
	}
	else if (query->last())
	{
		nCount = query->at() + 1; // 可能会返回-1
		if (nCount < 0)
		{
			nCount = 0;
		}
	}

	// 移动记录指针到首行
	if (nCount > 0)
	{
		if (!query->first()) nCount = 0;
	}
	return nCount;
}

/*
	构造函数,打开配置数据库连接
*/
KSQLConfig::KSQLConfig(const QString& sDbPath, QObject* parent)
	: QObject(parent),
	m_sDbPath(sDbPath),
	m_sDbName(""),
	m_sTableName(""),
	m_bDbOpened(false),
	m_nRecordCount(0),
	m_pExec(nullptr),
	m_pQuery(nullptr)
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
		m_sLastErrorMsg = QStringLiteral("系统没有安装SQLITE驱动,无法连接SQLITE");
		//ThrowBizErr(ERR_RUN, m_sLastErrorMsg);
		qDebug() << m_sLastErrorMsg;
		throw 1;
    }
}

KSQLConfig::~KSQLConfig()
{
	closeDataset();
}

/*
	设置数据库库文件路径,相对与当前工作目录,如:
	设置为data,则数据库文件全路径为:当前目录/data/DBfile
*/
void KSQLConfig::setDbPath(const QString& sDBPath)
{
	closeDataset();
	m_sDbPath = sDBPath;
}

QString KSQLConfig::dbPath() const
{
	return m_sDbPath;
}

/*
	打开数据库
*/
bool KSQLConfig::openDatabase(const QString& sDbName)
{
	closeDataset();

	++CDBSNO;
	m_sDbName = sDbName;
	m_sLastErrorMsg.clear();
	QString sDbFile = m_sDbName;
	m_oDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QString("%1_%2").arg(m_sDbName).arg(CDBSNO));
	if (m_oDb.isValid())
	{
		sDbFile = QDir::currentPath() + "/" + m_sDbPath + "/" + sDbFile;
		m_oDb.setDatabaseName(sDbFile);
		m_bDbOpened = m_oDb.open();
		if (m_bDbOpened)
		{
			m_pQuery = new QSqlQuery(m_oDb);
			m_pExec = new QSqlQuery(m_oDb);
		}
		m_sLastErrorMsg = m_oDb.lastError().text();
	}
	else
	{
		m_sLastErrorMsg = m_oDb.lastError().text();
		m_bDbOpened = false;
	}
	return m_bDbOpened;
}

/*
	关闭数据库
*/
void KSQLConfig::closeDataset()
{
	if (!m_bDbOpened) return;

	m_nRecordCount = 0;
	m_bDbOpened = false;
	if (m_pQuery)
	{
		delete m_pQuery;
		m_pQuery = nullptr;
	}
	if (m_pExec)
	{
		delete m_pExec;
		m_pExec = nullptr;
	}
	if (m_oDb.isOpen())
	{
		m_oDb.close();
	}
}

/* 开始事务 */
bool KSQLConfig::beginTransaction()
{
	if (m_oDb.isOpen())
	{
		if (!m_oDb.driver()->beginTransaction())
		{
			K_ERROR(QStringLiteral("启动事务失败:%1").arg(m_oDb.driver()->lastError().text()));
			return false;
		}
		return true;
	}
	else
	{
		K_ERROR(QStringLiteral("启动事务失败:数据库没有打开"));
		return false;
	}
}

/* 提交事务 */
bool KSQLConfig::commitTransaction()
{
	if (m_oDb.isOpen())
	{
		if (!m_oDb.driver()->commitTransaction())
		{
			K_ERROR(QStringLiteral("提交事务失败:%1").arg(m_oDb.driver()->lastError().text()));
			return false;
		}
		return true;
	}
	else
	{
		K_ERROR(QStringLiteral("提交事务失败:数据库没有打开"));
		return false;
	}
}

/* 回滚事务 */
bool KSQLConfig::rollbackTransaction()
{
	if (m_oDb.isOpen())
	{
		if (!m_oDb.driver()->rollbackTransaction())
		{
			K_ERROR(QStringLiteral("回滚事务失败:%1").arg(m_oDb.driver()->lastError().text()));
			return false;
		}
		return true;
	}
	else
	{
		K_ERROR(QStringLiteral("回滚事务失败:数据库没有打开"));
		return false;
	}
}

/*
	打开指定查询(select),如果有数据,则记录指针指向第一行记录,
	通过recordCount()获取查询到的记录数
@param
	sTableName	表名称
	sCondition	查询条件,不带where关键字
@return
	执行成功返回true,否则返回false
*/
bool KSQLConfig::openQuery(const QString& sTableName, const QString& sCondition)
{
	if (sTableName.isEmpty() && m_sTableName.isEmpty())
	{
		m_nRecordCount = 0;
		K_ERROR(QStringLiteral("打开查询失败:没有设置打开的表名称"));
		return false;
	}
	else if (sTableName != m_sTableName)
	{
		m_sTableName = sTableName;
	}

	QString sSQL = QString("select * from %1 ").arg(m_sTableName);
	if (!sCondition.isEmpty())
	{
		sSQL.append(" where ").append(sCondition);
	}
	// 执行查询
	return execQuery(sSQL);
}

/*
	执行指定的查询语句:select,如果有数据,则记录指针指向第一行记录,
	通过recordCount()获取查询到的记录数
	注意:不能执行非查询语句
@param
	sSql	标准的SQL查询语句
@return
	执行成功返回true,否则返回false
*/
bool KSQLConfig::execQuery(const QString& sSql)
{
	m_sLastErrorMsg.clear();

    // 数据库没有打开
	if (!m_pQuery)
	{
		K_ERROR(QStringLiteral("数据库没有打开,执行查询失败:%1").arg(sSql));
		return false;
	}

	m_nRecordCount = 0;
	if (!m_pQuery->exec(sSql))
	{
		m_sLastErrorMsg = m_pQuery->lastError().text();
		K_ERROR(QStringLiteral("执行查询<%1>失败:\n%2").arg(sSql).arg(m_sLastErrorMsg));
        return false;
	}

	// 获取当前记录数
	if (m_pQuery->driver()->hasFeature(QSqlDriver::QuerySize))
	{
		m_nRecordCount = m_pQuery->size();
	}
	else if (m_pQuery->last())
	{
		m_nRecordCount = m_pQuery->at() + 1; // 可能会返回-1
		if (m_nRecordCount < 0)
		{
			m_nRecordCount = 0;
		}
	}

	// 移动记录指针到首行
	if (m_nRecordCount > 0)
	{
		if (!m_pQuery->first())
		{
			m_nRecordCount = 0;
			m_sLastErrorMsg = m_pQuery->lastError().text();
			return false;
		}
	}

	return true;
}

/*
	执行非查询SQL,如:insert/update/delete
@param
	sSQL	待执行SQL
@return
	执行失败返回-1,成功返回影响的行数
*/
int KSQLConfig::execSQL(const QString& sSQL)
{
	m_sLastErrorMsg.clear();
	if (sSQL.isEmpty()) return -1;

    // 数据库没有打开
    if (!m_pExec) 
	{
		K_ERROR(QStringLiteral("数据库没有打开,执行查询失败:%1").arg(sSQL));
		return -1;
	}

	if (!m_pExec->exec(sSQL))
	{
		m_sLastErrorMsg = m_pExec->lastError().text();
		K_ERROR(QStringLiteral("执行SQL<%1>失败:\n%2").arg(sSQL).arg(m_sLastErrorMsg));
		return -1;
	}
	else
	{
		return m_pExec->numRowsAffected();
	}
}

/*
	返回当前查询的总记录数
*/
int KSQLConfig::recordCount() const
{
	return m_nRecordCount;
}

/*
	向下移动当前查询一行记录
*/
bool KSQLConfig::next()
{
    if (!m_pQuery || !m_pQuery->isActive())
	{
		m_sLastErrorMsg = QStringLiteral("执行<next>失败:查询没有打开");
		K_ERROR(m_sLastErrorMsg);
		return false;
	}
	if (!m_pQuery->next())
	{
		m_sLastErrorMsg = m_pQuery->lastError().text();
		return false;
	}
	return true;
}

/*
	向下移动当前查询一行记录
*/
bool KSQLConfig::previous()
{
    if (!m_pQuery || !m_pQuery->isActive())
	{
		m_sLastErrorMsg = QStringLiteral("执行<previous>失败:查询没有打开");
		K_ERROR(m_sLastErrorMsg);
		return false;
	}
	if (!m_pQuery->previous())
	{
		m_sLastErrorMsg = m_pQuery->lastError().text();
		return false;
	}
	return true;
}

/*
	移动到记录集第一行记录
*/
bool KSQLConfig::first()
{
    if (!m_pQuery || !m_pQuery->isActive())
	{
		m_sLastErrorMsg = QStringLiteral("执行<first>失败:查询没有打开");
		K_ERROR(m_sLastErrorMsg);
		return false;
	}
	if (!m_pQuery->first())
	{
		m_sLastErrorMsg = m_pQuery->lastError().text();
		return false;
	}
	return true;
}

/*
	移动到记录集最后一行记录
*/
bool KSQLConfig::last()
{
    if (!m_pQuery || !m_pQuery->isActive())
	{
		m_sLastErrorMsg = QStringLiteral("执行<last>失败:查询没有打开");
		K_ERROR(m_sLastErrorMsg);
		return false;
	}
	if (!m_pQuery->last())
	{
		m_sLastErrorMsg = m_pQuery->lastError().text();
		return false;
	}
	return true;
}

/*
	返回当前记录所在行索引,从0开始
*/
int KSQLConfig::at()
{
    if (!m_pQuery || !m_pQuery->isActive())
	{
		return QSql::BeforeFirstRow;
	}
	return m_pQuery->at();
}

/*
	取当前行指定列数据
@param
	sFieldName	表名称
*/
QVariant KSQLConfig::getValue(const QString& sFieldName)
{
    if (!m_pQuery || !m_pQuery->isValid())
	{
		return QVariant(QVariant::Invalid);
	}
	return m_pQuery->value(sFieldName);
}

QVariant KSQLConfig::getValue(const int nCol)
{
	if (!m_pQuery || !m_pQuery->isValid())
	{
		return QVariant(QVariant::Invalid);
	}
	return m_pQuery->value(nCol);
}

/*
	返回当前表名称
*/
const QString& KSQLConfig::databaseName() const
{
	return m_sDbName;
}

/*
	返回当前表名称
*/
const QString& KSQLConfig::tableName() const
{
	return m_sTableName;
}

void KSQLConfig::setTableName(const QString& sTableName)
{
	m_sTableName = sTableName;
}

/*
	检查表是否存在
*/
bool KSQLConfig::checkTableExist(const QString& sTableName, const bool bThrowExecpt)
{
	QString sSQL = QStringLiteral("select count(*) as cnt from sqlite_master where type='table' and name='%1'");
	if (execQuery(sSQL.arg(sTableName)))
	{
		if (recordCount() == 1)
		{
			if (getValue(QStringLiteral("cnt")).toInt() == 1)
				return true;
		}
	}
	if (bThrowExecpt)
	{
        //ThrowBizErr(ERR_RUN, QStringLiteral("数据表[%1]不存在!").arg(sTableName));
		qDebug() << QStringLiteral("数据表[%1]不存在!").arg(sTableName);
		throw 1;
	}
	return false;
}

/* 返回执行后的错误信息 */
QString KSQLConfig::lastError()
{
	return m_sLastErrorMsg;
}
