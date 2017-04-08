#ifndef SERVICEMODUEMGR_H
#define SERVICEMODUEMGR_H

#include <QObject>
#include <QMap>

/*-----------------------------------------------------------------------------
* ServiceModueMgr
* ����ģ�������,��������ģ��ļ��غ��ͷ�
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
		����ָ��ģ��
	@param:
		sLibName ģ������,����·����Ϣ����չ��
	*/
	IViewPage *loadModule(const QString& sLibName);

private:
	QMap<QString, ModuleItem*> m_oModules;
};

#endif // SERVICEMODUEMGR_H
