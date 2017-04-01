#include "mainwidget.h"
#include <QSplitter>
#include <QButtonGroup>
#include <QDateTime>

MainWidget::MainWidget(QWidget *parent)
	: CDialog(parent, 0, 3)
{
	setupUi();

	resize(500, 500);

	m_nTimer = startTimer(1000, Qt::PreciseTimer);

	initTestData();
}

MainWidget::~MainWidget()
{

}

void MainWidget::timerEvent(QTimerEvent * evt)
{
	if (evt->timerId() == m_nTimer)
	{
		// 刷新系统时间
		m_pTradeTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	}
}

void MainWidget::setupUi()
{
	//setAttribute(Qt::WA_DeleteOnClose);
	setAllowResize(true);
	setShowMaximumButton(true);
	setShowMinimumButton(true);
	setForbidEscapeKeyClose(true);
	setWinTitleBarHeight(33);
	setWinTitleBarButtonHeight(33 - 3 - 1);
	setWindowTitle(KSL("交易运营服务平台"));
	setWindowIcon(QIcon());

	// 视图标签栏
	m_pTabBar = new QWidget(this);
	m_pTabBar->setObjectName(KSL("page-tab-bar"));
	m_pTabBar->setFixedHeight(27);
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
	layout()->setContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);
	// 主布局器--客户区
	QWidget* pClientArea = new QWidget(this);
	pClientArea->setObjectName(KSL("wb-client-area"));
	//pClientArea->setFrame(QFrame::NoFrame);
	pClientArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	layout()->addWidget(pClientArea);
	// 主布局器--客户区--布局器
	QHBoxLayout* pClientHLayout = new QHBoxLayout(pClientArea);
	pClientHLayout->setContentsMargins(0, 0, 0, 0);
	pClientHLayout->setSpacing(0);
	// 主布局器--客户区--布局器--水平分裂期
	QSplitter* pSplitter = new QSplitter(pClientArea);
	pSplitter->setObjectName(KSL("splitter"));
	pSplitter->setOrientation(Qt::Horizontal);
	// 主布局器--客户区--布局器--水平分裂期--左侧视图元件树
	m_pViewTree = new QTreeWidget(this);
	m_pViewTree->setObjectName(KSL("view-list"));
	m_pViewTree->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	//m_pViewTree->setWorkbench(this, pSplitter);
	//m_pViewTree->installEventFilter(this);
	pSplitter->addWidget(m_pViewTree);
	connect(m_pViewTree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(doOpenView(QTreeWidgetItem *)));
	// 主布局器--客户区--布局器--水平分裂期--右侧视图堆栈
	m_pViewStack = new QStackedWidget(pClientArea);
	m_pViewStack->setObjectName(KSL("view-stack"));
	m_pViewStack->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	m_pViewStack->installEventFilter(this);
	pSplitter->addWidget(m_pViewStack);
	pClientHLayout->addWidget(pSplitter);
	pSplitter->setStretchFactor(1, 1);
	pSplitter->setSizes({ 200, 300 });
	pSplitter->update();

	// 主布局器-布局器--状态栏
	setupStatusBar();
}

void MainWidget::setupStatusBar()
{
	QStatusBar* pStatusBar = new QStatusBar(this);
	pStatusBar->setObjectName(KSL("wb-statusbar"));
	pStatusBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	layout()->addWidget(pStatusBar);

	// 用户代码、名称
	QLabel* label = new QLabel(pStatusBar);
	label->setObjectName(KSL("wb-user"));
	label->setText(KSL("%1 (%2)").arg(KSL("admin")).arg(KSL("管理员")));
	label->setIndent(4);
	pStatusBar->addWidget(label, 0);
	
	// 信息
	label = new QLabel(pStatusBar);
	label->setObjectName(KSL("wb-msg"));
	label->setText(KSL("提示信息..."));
	label->setIndent(4);
	label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	pStatusBar->addWidget(label, 1);

	// 交易日期时间
	m_pTradeTime = new QLabel(pStatusBar);
	m_pTradeTime->setObjectName(KSL("wb-time"));
	m_pTradeTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	pStatusBar->addPermanentWidget(m_pTradeTime, 0);
}

void MainWidget::initTestData()
{
	m_pViewTree->setHeaderHidden(true);
	m_pViewTree->setColumnCount(1);

	QList<QTreeWidgetItem *> menus;

	QTreeWidgetItem *nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("产品")));
	QList<QTreeWidgetItem *> menuItems;
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("产品信息管理"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("产品信息复核"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	menuItems.clear();
	nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("报表")));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("管理型报表"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("结构式报表"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	menuItems.clear();
	nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("风控")));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("风控参数设置"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(KSL("风控触发查询"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	m_pViewTree->insertTopLevelItems(0, menus);

	m_pViewTree->expandAll();
}

void MainWidget::doOpenView(QTreeWidgetItem *item)
{
	if (item->childCount() > 0)
		return;

	QString tabId = item->text(0);

	// 通过ID查找按钮
	for (int i = 0; i < m_tabs.size(); i++)
	{
		if (m_tabs[i]->text() == tabId)
		{
			m_tabs[i]->setChecked(true);
			return;
		}
	}

	QToolButton* pTabBtn = new QToolButton(m_pTabBar);
	pTabBtn->setText(tabId);
	pTabBtn->setCheckable(true);
	pTabBtn->setAutoRaise(true);
	pTabBtn->setMaximumWidth(160);
	pTabBtn->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	//connect(pTabBtn, SIGNAL(clicked()), this, SLOT(doTabButtonClicked()), Qt::DirectConnection);

	//pTabBtn->addInterButton(Tab_Close_Button_Id, AlignPositon::apVRightCenter, QPoint(6, 0),
	//	QIcon(RS_Tab_Close), QIcon(RS_Tab_Close_Hover));
	//pTabBtn->installEventFilter(this);
	//connect(pTabBtn, SIGNAL(onInterButtonClicked(const QString)),
	//	this, SLOT(doTabCloseButtonClicked(const QString)),
	//	Qt::DirectConnection);

	m_pTabBtnGroup->addButton(pTabBtn);
	m_tabs.append(pTabBtn);
	m_pTabLayout->addWidget(pTabBtn);
	pTabBtn->setChecked(true);

	int nWidth = (160 + 0) * m_tabs.size();
	m_pTabBar->resize(qMin(nWidth, m_pTabBar->maximumWidth()), 27);
	// 刷新标签栏,否则界面没刷新
	update(titleRect());
}