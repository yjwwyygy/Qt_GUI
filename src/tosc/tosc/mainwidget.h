#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "basicdialog.h"
#include <QMap>

class QHBoxLayout;
class QButtonGroup;
class QTreeWidget;
class QStackedWidget;
class QStatusBar;
class QLabel;
class QToolButton;
class QTreeWidgetItem;
class TabButton;
class IViewPage;
class LoginWidget;
class ServiceModueMgr;

struct MenuItem
{
	int m_nMenuId;
	QString m_sMenuName;
	QString m_sIconName;
	QString m_sLibName;
};

class MainWidget : public BasicDialog
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

	/* ����������UI������,��Main�е��� */
	bool createMainWindow();

	/* ��¼ϵͳ */
	bool login();

protected:
	/* ���ǣ����붨ʱ�������¼� */
	virtual void timerEvent(QTimerEvent * evt);

	/* ���ǣ����ù�� */
	virtual bool eventFilter(QObject *watched, QEvent *event);

	// ���ֱ���������
	virtual void layoutTitleBarWidgets();

private:
	// ����UI
	void setupUi();

	// ����״̬��
	void setupStatusBar();

	// ������ǩ����ť���
	void adjustTabBarWidth();

	// ѡ��ָ����ͼΪ��ǰ��ͼ
	void selectedTabs(TabButton* pTabButton);

	// ������ͼҳ��
	IViewPage *createViewPage(int menuId);

	// ���ز˵�
	void LoadMenus();

private slots:
	void doOpenView(QTreeWidgetItem *item);

	/* ��ǩ��ťclicked()�źŴ���� */
	void doTabButtonClicked();

	/* ��ǩ�رհ�ť����źŴ���� */
	void doTabCloseClicked();

	// TODO: test
	void doLogin(const QString &userId, const QString &userPwd);

private:
	// Tap��
	QWidget* m_pTabBar = nullptr;
	QHBoxLayout* m_pTabLayout = nullptr;
	QButtonGroup* m_pTabBtnGroup = nullptr;

	QToolButton* m_pSysDrop = nullptr;			// ���ð�ť

	// ���Ͳ˵� | ��ͼ��ջ��
	QTreeWidget* m_pViewTree = nullptr;
	QStackedWidget* m_pViewStack = nullptr;

	// ״̬��
	QStatusBar* m_pStatusBar;
	QLabel* m_pTradeTime = nullptr;

	int m_nTimer;

	QList<TabButton*> m_tabs;

	int m_nTabBtnWidth = 160;
	int m_nTabBtnHeight = 25;			// ��������ť�߶�

	QMap<int, IViewPage*> m_oViewMap;

	TabButton *m_pCurTabBtn;			// ��ǰѡ�е�Tap��ť

	LoginWidget *m_pLoginWidget;		// ��¼����

	ServiceModueMgr *m_pServiceModueMgr;	// ģ�������

	QMap<int, MenuItem*> m_oMenuItems;
};

#endif // MAINWIDGET_H
