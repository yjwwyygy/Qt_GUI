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
#include "tabbutton.h"

MainWidget::MainWidget(QWidget *parent)
	: BasicDialog(parent, 0, 3)
{
	setupUi();

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
		// ˢ��ϵͳʱ��
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

	// ������ǩ����ǩ���
	if (m_pTabBar)
	{
		// ������ǩ������
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
	//setAttribute(Qt::WA_DeleteOnClose);
	setResizable(true);
	setMaximumButtonVisible(true);
	setMinimumButtonVisible(true);
	setForbidEscapeKeyClose(true);
	setWindowTitle(QStringLiteral("������Ӫ����ƽ̨"));

	// ��ͼ��ǩ��
	m_pTabBar = new QWidget(this);
	m_pTabBar->setObjectName(QStringLiteral("page-tab-bar"));
	m_pTabBar->setFixedHeight(m_nTabBtnHeight);
	m_pTabBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	addTitleBarWidget(m_pTabBar, Qt::AlignLeft | Qt::AlignBottom, 0, false);

	// ��ͼ��ǩ��--ˮƽ������
	m_pTabLayout = new QHBoxLayout(m_pTabBar);
	m_pTabLayout->setContentsMargins(0, 0, 0, 0);
	m_pTabLayout->setSpacing(0);

	// ��ǩ����ť��
	m_pTabBtnGroup = new QButtonGroup(this);
	m_pTabBtnGroup->setExclusive(true);

	// ��������
	setLayout(new QVBoxLayout(this));
	layout()->setContentsMargins(0, 0, 0, 0);
	layout()->setSpacing(0);
	// ��������--�ͻ���
	QFrame* pClientArea = new QFrame(this);
	pClientArea->setObjectName(QStringLiteral("main-client-area"));
	pClientArea->setFrameShape(QFrame::NoFrame);
	pClientArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	layout()->addWidget(pClientArea);
	// ��������--�ͻ���--������
	QHBoxLayout* pClientHLayout = new QHBoxLayout(pClientArea);
	pClientHLayout->setContentsMargins(0, 0, 0, 0);
	pClientHLayout->setSpacing(0);
	// ��������--�ͻ���--������--ˮƽ������
	QSplitter* pSplitter = new QSplitter(pClientArea);
	pSplitter->setObjectName(QStringLiteral("splitter"));
	pSplitter->setOrientation(Qt::Horizontal);
	// ��������--�ͻ���--������--ˮƽ������--�����ͼԪ����
	m_pViewTree = new QTreeWidget(this);
	m_pViewTree->setObjectName(QStringLiteral("view-list"));
	m_pViewTree->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	m_pViewTree->installEventFilter(this);
	pSplitter->addWidget(m_pViewTree);
	connect(m_pViewTree, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(doOpenView(QTreeWidgetItem *)));
	// ��������--�ͻ���--������--ˮƽ������--�Ҳ���ͼ��ջ
	m_pViewStack = new QStackedWidget(pClientArea);
	m_pViewStack->setObjectName(QStringLiteral("view-stack"));
	m_pViewStack->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	m_pViewStack->installEventFilter(this);
	pSplitter->addWidget(m_pViewStack);
	pClientHLayout->addWidget(pSplitter);
	pSplitter->setStretchFactor(1, 1);
	pSplitter->setSizes({ 180, 300 });
	pSplitter->update();

	// ��������-������--״̬��
	setupStatusBar();
}

void MainWidget::setupStatusBar()
{
	QStatusBar* pStatusBar = new QStatusBar(this);
	pStatusBar->setObjectName(QStringLiteral("main-statusbar"));
	pStatusBar->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	layout()->addWidget(pStatusBar);

	// �û����롢����
	QLabel* label = new QLabel(pStatusBar);
	label->setObjectName(QStringLiteral("main-user"));
	label->setText(QStringLiteral("%1 (%2)").arg(QStringLiteral("admin")).arg(QStringLiteral("����Ա")));
	label->setIndent(4);
	pStatusBar->addWidget(label, 0);
	
	// ��Ϣ
	label = new QLabel(pStatusBar);
	label->setObjectName(QStringLiteral("main-msg"));
	label->setText(QStringLiteral("��ʾ��Ϣ..."));
	label->setIndent(4);
	label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
	pStatusBar->addWidget(label, 1);

	// ��������ʱ��
	m_pTradeTime = new QLabel(pStatusBar);
	m_pTradeTime->setObjectName(QStringLiteral("main-time"));
	m_pTradeTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
	pStatusBar->addPermanentWidget(m_pTradeTime, 0);
}

void MainWidget::adjustTabBarWidth()
{
	int nWidth = m_nTabBtnWidth * m_tabs.size();
	m_pTabBar->resize(qMin(nWidth, m_pTabBar->maximumWidth()), m_nTabBtnHeight);
	// ˢ�±�ǩ��,�������ûˢ��
	update(titleRect());
}

void MainWidget::selectedTabs(TabButton * pTabButton)
{
	m_pTabBar->setEnabled(false);
	ON_SCOPE_EXIT([&]() {
		m_pTabBar->setEnabled(true);
	});

	m_pCurrentTab = pTabButton;
	m_pCurrentTab->setChecked(true);

	// ���û�д������ȴ���
	WorkView* pViewWin = nullptr;
	if (m_pCurrentTab->userObject() == nullptr)
	{
		int nViewId = m_pCurrentTab->tabId().toInt();
		WorkViewCfg* pViewCfg = m_pWorkbenchCfg->find(nViewId);
		if (nullptr == pViewCfg)
		{
			KMsgDlg::warning(KSL("��ͼ����δ�ҵ����޷�������"));
			return;
		}
		pViewWin = createViewWindow(pViewCfg);
		m_pCurrentTab->setUserObject(pViewWin);
	}
	else
	{
		pViewWin = qobject_cast<WorkView*>(m_pCurrentTab->userObject());
		K_ASSERT(pViewWin);
	}
	// �л���ͼ
	if (m_pViewStack->currentWidget() != pViewWin)
	{
		m_pViewStack->setCurrentWidget(pViewWin);
	}
	pViewWin->activeWorkView();
}

void MainWidget::initTestData()
{
	m_pViewTree->setHeaderHidden(true);
	m_pViewTree->setColumnCount(1);

	QList<QTreeWidgetItem *> menus;

	QTreeWidgetItem *nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("��Ʒ")));
	QList<QTreeWidgetItem *> menuItems;
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("��Ʒ��Ϣ����"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("��Ʒ��Ϣ����"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	menuItems.clear();
	nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("����")));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("�����ͱ���"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("�ṹʽ����"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	menuItems.clear();
	nemu = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("���")));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("��ز�������"))));
	menuItems.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QStringLiteral("��ش�����ѯ"))));
	nemu->addChildren(menuItems);
	menus.append(nemu);

	m_pViewTree->insertTopLevelItems(0, menus);

	m_pViewTree->expandAll();
}

void MainWidget::doTabButtonClicked()
{
	if (TabButton* tab = qobject_cast<TabButton*>(sender()))
	{
		selectedTabs(tab);
	}
}

void MainWidget::doOpenView(QTreeWidgetItem *item)
{
	if (item->childCount() > 0)
		return;

	QString tabId = item->text(0);

	// ͨ��ID���Ұ�ť
	for (int i = 0; i < m_tabs.size(); i++)
	{
		if (m_tabs[i]->text() == tabId)
		{
			m_tabs[i]->setChecked(true);
			return;
		}
	}

	TabButton* pTabBtn = new TabButton(m_pTabBar);
	pTabBtn->setText(tabId);
	pTabBtn->setCheckable(true);
	pTabBtn->setAutoRaise(true);
	pTabBtn->setMaximumWidth(m_nTabBtnWidth);
	pTabBtn->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	connect(pTabBtn, SIGNAL(clicked()), this, SLOT(doTabButtonClicked()), Qt::DirectConnection);

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

	int nWidth = m_nTabBtnWidth * m_tabs.size();
	m_pTabBar->resize(qMin(nWidth, m_pTabBar->maximumWidth()), m_nTabBtnHeight);
	// ˢ�±�ǩ��,�������ûˢ��
	update(titleRect());
}