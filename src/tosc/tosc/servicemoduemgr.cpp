#include "servicemoduemgr.h"

#include <QLibrary>
#include <QDebug>
#include "icontext.h"
#include "iviewpage.h"

extern "C"
{
	typedef IViewPage* (*API_LoadModule)(IContext*);
}

/*-----------------------------------------------------------------------------
* ModuleItem
* ģ����
-----------------------------------------------------------------------------*/
class ModuleItem
{
public:
	~ModuleItem()
	{
		if (m_pLib)
		{
			delete m_pLib;
			m_pLib = nullptr;
		}
	}

	QString m_sLibName;								// ģ����
	QLibrary* m_pLib = nullptr;						// ģ������
	API_LoadModule m_pfLoadModule = nullptr;		// ģ���ʼ���ӿ�
};

ServiceModueMgr::ServiceModueMgr(QObject *parent)
	: QObject(parent)
{

}

ServiceModueMgr::~ServiceModueMgr()
{
	QMap<QString, ModuleItem*>::iterator itor = m_oModules.begin();
	while (itor != m_oModules.end())
	{
		delete itor.value();
		itor++;
	}
}

IViewPage * ServiceModueMgr::loadModule(const QString & sLibName)
{
	QMap<QString, ModuleItem*>::iterator itor = m_oModules.find(sLibName);
	if (itor != m_oModules.end())
	{
		return itor.value()->m_pfLoadModule(gContext);
	}

	QLibrary* pLib = new QLibrary(sLibName);
	if (!pLib->load())
	{
		qDebug() << pLib->errorString();
		return nullptr;
	}

	API_LoadModule pfLoadModule = (API_LoadModule)pLib->resolve("LoadBizModule");
	if (!pfLoadModule)
	{
		qDebug() << QStringLiteral("��������ʧ��");
		return nullptr;
	}

	ModuleItem *pModuleItem = new ModuleItem;
	pModuleItem->m_sLibName = sLibName;
	pModuleItem->m_pLib = pLib;
	pModuleItem->m_pfLoadModule = pfLoadModule;

	return pfLoadModule(gContext);
}
