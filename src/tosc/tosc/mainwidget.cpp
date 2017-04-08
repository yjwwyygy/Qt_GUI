#include "mainwidget.h"
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QStatusBar>
#include <QLabel>
#include <QSplitter>
#include <QButtonGroup>
#include <QDateTime>
#include <QToolButton>
#include <QTimerEvent>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include "tabbutton.h"
#include "winres.h"
#include "iviewpage.h"
#include "loginwidget.h"
#include "servicemoduemgr.h"
#include "ksqlconfig.h"

MainWidget::MainWidget(QWidget *parent)
	: BasicDialog(parent, 0, 3)
{
	setupUi();

	m_nTimer = startTimer(1000, Qt::PreciseTimer);

	m_pServiceModueMgr = new ServiceModueMgr(this);

	m_pLoginWidget = new LoginWidget(nullptr);
	connect(m_pLoginWidget, SIGNAL(onLogin(const QString&, const QString&)), this, SLOT(doLogin(const QString&, const QString&)));
	m_pLoginWidget->raise();
	qApp->processEvents();
}

MainWidget::~MainWidget()
{

}

bool MainWidget::createMainWindow()
{
	// 加载菜单
	LoadMenus();

	return true;
}

bool MainWidget::login()
{
	if (m_pLoginWidget->exec())
	{
		show();
		activateWindow();

		return true;
	}

	return false;
}

void MainWidget::timerEvent(QTimerEvent * evt)
{
	if (evt->timerId() == m_nTimer)
	{
		// 刷新系统时间
		m_pTradeTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	}
}

bool MainWidget::eventFilter(QObject * watched, QEvent * event)
{
	if (event->type() == QEvent::Enter && (m_pViewStack == watched || m_pViewTree == watched))
	{
		setCursor(QCursor(Qt::ArrowCursor));
	}

	return BasicDialog::eventFilter(watched, event);;
}

void MainWidget::layoutTitleBarWidgets()
{
	BasicDialog::layoutTitleBarWidgets();

	// 调整标签栏标签宽度
	if (m_pTabBar)
	{
		// 调整标签栏长度
		int nMaxWidth = titleTextMaxRightPos() - titleTextRightPos() - 8;
		if (m_pTabBar->maximumWidth() != nMaxWidth)
		{
			m_pTabBar->setMaximumWidth(nMaxWidth);
			adjustTabBarWidth();
		}
	}
}

void MainWidget::setupUi()
{
	QFile f(WRS_Main_Widget_Qss);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		setStyleSheet(in.readAll());
	}

	//setAttribute(Qt::WA_DeleteOnClose);
	setResizable(true);
	setMaximumButtonVisible(true);
	setMinimumButtonVisible(true);
	setForbidEscapeKeyClose(true);
	setWindowTitle(QStringLiteral("交易运营服务平台"));

	// 视图标签栏
	m_pTabBar = new QWidget(this);
	m_pTabBar->setObjectName(QStringLiteral("page-tab-bar"));
	m_pTabBar->setFixedHeight(m_nTabBtnHeight);
	m_pTabBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	addTitleBarWidget(m_pTabBar, Qt::AlignLeft | Qt::AlignBottom, 0, false);

	// 视图标签栏--水平布局器
	m_pTabLayout = new QHBoxLayout(m_pTabBar);
	m_pTabLayout->setContentsMargins(0, 0, 0, 0);
	m_pTabLayout->setSpacing(0);

	// 标签栏按钮组
	m_pTabBtnGroup = new QButtonGroup(this);
	m_pTabBtnGroup->setExclusive(true);

	// 主布局器
	setLayout(new QVBoxLayout(this));
	layout()->setContentsMargins(1, 1, 0, 0);
	layout()->setSpacing(0);
	// 主布局器--客户区
	QFrame* pClientArea = new QFrame(this);
	pClientArea->setObjectName(QStringLiteral("main-client-area"));
	pClientArea->setFrameShape(QFrame::NoFrame);
	pClientArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	layout()->addWidget(pClientArea);
	// 主布局器--客户区--布局器
	QHBoxLayout* pClientHLayout = new QHBoxLayout(pClientArea);
	pClientHLayout->setContentsMargins(0, 0, 0, 0);
	pClientHLayout->setSpacing(0);
	// 主布局器--客户区--布局器--水平分裂期
	QSplitter* pSplitter = new QSplitter(pClientArea);
	pSplitter->setObjectName(QStringLiteral("splitter"));
	pSplitter->setOrientation(Qt::Horizontal);
	// 主布局器--客户区--布局器--水平分裂期--左侧视图元件树
	m_pViewTree = new QTreeWidget(this);
	m_pViewTree->setObjectName(QStringLiteral("view-list"));
	m_pViewTree->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	m_pViewTree->installEventFilter(this);
	m_pViewTree->setHeaderHidden(true);
	m_pViewTree->setColumnCount(1);
	pSplitter->addWidget(m_pViewTree);
	connect(m_pViewTree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(doOpenView(QTreeWidgetItem *)));
	// 主布局器--客户区--布局器--水平分裂期--右侧视图堆栈
	m_pViewStack = new QStackedWidget(pClientArea);
	m_pViewStack->setObjectName(QStringLiteral("view-stack"));
	m_pViewStack->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	m_pViewStack->installEventFilter(this);
	pSplitter->addWidget(m_pViewStack);
	pClientHLayout->addWidget(pSplitter);
	pSplitter->setStretchFactor(1, 1);
	pSplitter->setSizes({ 180, 300 });
	pSplitter->update();

	// 主布局器-布局器--状态栏
	setupStatusBar();
}

void MainWidget::setupStatusBar()
{
	QStatusBar* pStatusBar = new QStatusBar(this);
	pStatusBar->setObjectName(QStringLiteral("main-statusbar"));
	pStatusBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	layout()->addWidget(pStatusBar);

	// 用户代码、名称
	QLabel* label = new QLabel(pStatusBar);
	label->setObjectName(QStringLiteral("main-user"));
	label->setText(QStringLiteral("%1 (%2)").arg(QStringLiteral("admin")).arg(QStringLiteral("管理员")));
	label->setIndent(4);
	pStatusBar->addWidget(label, 0);
	
	// 信息
	label = new QLabel(pStatusBar);
	label->setObjectName(QStringLiteral("main-msg"));
	label->setText(QStringLiteral("提示信息..."));
	label->setIndent(4);
	label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	pStatusBar->addWidget(label, 1);

	// 交易日期时间
	m_pTradeTime = new QLabel(pStatusBar);
	m_pTradeTime->setObjectName(QStringLiteral("main-time"));
	m_pTradeTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	pStatusBar->addPermanentWidget(m_pTradeTime, 0);
}

void MainWidget::adjustTabBarWidth()
{
	int nWidth = m_nTabBtnWidth * m_tabs.size();
	m_pTabBar->resize(qMin(nWidth, m_pTabBar->maximumWidth()), m_nTabBtnHeight);
	// 刷新标签栏,否则界面没刷新
	update(titleRect());
}

void MainWidget::selectedTabs(TabButton * pTabButton)
{
	m_pTabBar->setEnabled(false);

	m_pCurTabBtn = pTabButton;
	m_pCurTabBtn->setChecked(true);
	int nViewId = m_pCurTabBtn->getId();

	IViewPage* pViewPage = m_oViewMap[nViewId];

	// 切换视图
	if (m_pViewStack->currentWidget() != pViewPage)
	{
		m_pViewStack->setCurrentWidget(pViewPage);
	}

	m_pTabBar->setEnabled(true);
}

IViewPage * MainWidget::createViewPage(int menuId)
{
	return m_pServiceModueMgr->loadModule(m_oMenuItems[menuId]->m_sLibName);
}

void MainWidget::LoadMenus()
{
	// 从本地配置中加载菜单项
	KSQLConfig *pSqlConfig = new KSQLConfig("data");
	if (!pSqlConfig->openDatabase("systemdb.db"))
	{
		qDebug() << "openDatabase error." << pSqlConfig->lastError();
		return;
	}

	if (!pSqlConfig->execQuery("select * from sys_menu order by show_no"))
	{
		qDebug() << "execQuery error.";
		return;
	}

	QTreeWidgetItem *menu, *menuItem;
	QMap<QString, QTreeWidgetItem*> parentMap;
	QMap<QString, QTreeWidgetItem*>::iterator itorParentMap;

	int nMenuId;
	QString sMenuName, sMenuGroup, sIconName, sLibName;
	for (size_t i = 0; i < pSqlConfig->recordCount(); i++)
	{
		nMenuId = pSqlConfig->getValue("menu_id").toInt();
		sMenuName = pSqlConfig->getValue("menu_name").toString();
		sMenuGroup = pSqlConfig->getValue("menu_group").toString();
		sIconName = pSqlConfig->getValue("icon_name").toString();
		sLibName = pSqlConfig->getValue("lib_name").toString();

		// 验证是否有此菜单权限
		//if (checkMenuRight(nMenuId))
		//{
		//	pSqlConfig->next();
		//	continue;
		//}

		// 创建树型菜单
		if ((itorParentMap = parentMap.find(sMenuGroup)) == parentMap.end())
		{
			menu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(sMenuGroup));
			m_pViewTree->addTopLevelItem(menu);

			parentMap[sMenuGroup] = menu;
		}
		else
		{
			menu = itorParentMap.value();
		}

		menuItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(sMenuName));
		menuItem->setData(0, Qt::UserRole, nMenuId);
		menu->addChild(menuItem);

		MenuItem *pMenuItem = new MenuItem;
		pMenuItem->m_nMenuId = nMenuId;
		pMenuItem->m_sMenuName = sMenuName;
		pMenuItem->m_sIconName = sIconName;
		pMenuItem->m_sLibName = sLibName;

		m_oMenuItems[nMenuId] = pMenuItem;

		pSqlConfig->next();
	}

	m_pViewTree->expandAll();
}

void MainWidget::doTabButtonClicked()
{
	if (TabButton* tabBtn = qobject_cast<TabButton*>(sender()))
	{
		selectedTabs(tabBtn);
	}
}

void MainWidget::doTabCloseClicked()
{
	if (TabButton* tabBtn = qobject_cast<TabButton*>(sender()))
	{
		IViewPage *pViewPage = m_oViewMap[tabBtn->getId()];

		if (tabBtn != m_pCurTabBtn)
		{
			m_pViewStack->removeWidget(pViewPage);
			m_tabs.removeOne(tabBtn);
			m_pTabBtnGroup->removeButton(tabBtn);
			m_pTabLayout->removeWidget(tabBtn);
		}
		else
		{
			if (m_tabs.size() == 1)
			{ // 最后一页
				m_pViewStack->removeWidget(pViewPage);
				m_tabs.removeOne(tabBtn);
				m_pTabBtnGroup->removeButton(tabBtn);
				m_pTabLayout->removeWidget(tabBtn);
			}
			else
			{
				int n = m_tabs.indexOf(tabBtn);
				m_pViewStack->removeWidget(pViewPage);
				m_tabs.removeOne(tabBtn);
				m_pTabBtnGroup->removeButton(tabBtn);
				m_pTabLayout->removeWidget(tabBtn);

				// 若关闭当前页，总是跳到前面一页
				if (n == 0)
					selectedTabs(m_tabs[0]);
				else
					selectedTabs(m_tabs[n-1]);
			}
		}

		// 手动删除对象
		delete tabBtn;
		delete pViewPage;

		// 调整菜单条位置
		adjustTabBarWidth();
	}
}

void MainWidget::doLogin(const QString &userId, const QString &userPwd)
{
	// 模拟执行登录
	// ...

	m_pLoginWidget->doLoginReturn(0, "");
}

void MainWidget::doOpenView(QTreeWidgetItem *item)
{
	if (item->childCount() > 0)
		return;

	// 获取ID
	int menuId = item->data(0, Qt::UserRole).toInt();

	// 通过ID查找按钮
	for (int i = 0; i < m_tabs.size(); i++)
	{
		if (m_tabs[i]->getId() == menuId)
		{
			m_pCurTabBtn = m_tabs[i];
			m_pCurTabBtn->setChecked(true);
			QWidget *pView = m_oViewMap[menuId];
			if (m_pViewStack->currentWidget() != pView)
			{
				m_pViewStack->setCurrentWidget(pView);
			}
			return;
		}
	}

	// 创建视图
	IViewPage *pViewPage = createViewPage(menuId);
	if (!pViewPage)
	{
		qDebug() << QStringLiteral("createViewPage error");
		return;
	}
	m_oViewMap[menuId] = pViewPage;

	// 将视图加入到主窗口区
	m_pViewStack->addWidget(pViewPage);
	m_pViewStack->setCurrentWidget(pViewPage);

	// 创建新的Tab按钮
	TabButton* pTabBtn = new TabButton(m_pTabBar);
	pTabBtn->setId(menuId);
	pTabBtn->setText(m_oMenuItems[menuId]->m_sMenuName);
	pTabBtn->setCheckable(true);
	pTabBtn->setAutoRaise(true);
	pTabBtn->setMaximumWidth(m_nTabBtnWidth);
	pTabBtn->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	connect(pTabBtn, SIGNAL(clicked()), this, SLOT(doTabButtonClicked()));
	connect(pTabBtn, SIGNAL(tabCloseClicked()), this, SLOT(doTabCloseClicked()));

	// 将Tab按钮加入到标题条中
	m_pTabBtnGroup->addButton(pTabBtn);
	m_tabs.append(pTabBtn);
	m_pTabLayout->addWidget(pTabBtn);
	pTabBtn->setChecked(true);

	m_pCurTabBtn = pTabBtn;

	// 调整菜单条的宽度
	adjustTabBarWidth();
}