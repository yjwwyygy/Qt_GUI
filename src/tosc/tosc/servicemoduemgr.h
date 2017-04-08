#ifndef SERVICEMODUEMGR_H
#define SERVICEMODUEMGR_H

#include <QObject>
#include <QMap>

/*-----------------------------------------------------------------------------
* ServiceModueMgr
* 服务模块管理器,管理所有模块的加载和释放
-----------------------------------------------------------------------------*/
class IViewPage;
class ModuleItem;
class ServiceModueMgr : public QObject
{
	Q_OBJECT

public:
	ServiceModueMgr(QObject *parent);
	~ServiceModueMgr();

	/*
		加载指定模块
	@param:
		sLibName 模块名称,不带路径信息和扩展名
	*/
	IViewPage *loadModule(const QString& sLibName);

private:
	QMap<QString, ModuleItem*> m_oModules;
};

#endif // SERVICEMODUEMGR_H
