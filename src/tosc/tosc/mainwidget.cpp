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
#include "tabbutton.h"
#include "winres.h"
#include "iviewpage.h"
#include "loginwidget.h"

MainWidget::MainWidget(QWidget *parent)
	: BasicDialog(parent, 0, 3)
{
	setupUi();

	m_nTimer = startTimer(1000, Qt::PreciseTimer);

	initTestData();

	m_pLoginWidget = new LoginWidget(nullptr);
	connect(m_pLoginWidget, SIGNAL(reqLogin(QString, QString)), this, SLOT(doLogin(QString, QString)));
	m_pLoginWidget->raise();
	qApp->processEvents();
}

MainWidget::~MainWidget()
{

}

bool MainWidget::createMainWindow()
{
	return true;
}

bool MainWidget::login()
{
	//m_pLoginWidget->login();
	//if (m_pLoginForm->exec())
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

	m_pCurTabBtn = pTabButton;
	m_pCurTabBtn->setChecked(true);
	QString strViewId = m_pCurTabBtn->text();

	IViewPage* pViewPage = m_oViewMap[strViewId];

	// �л���ͼ
	if (m_pViewStack->currentWidget() != pViewPage)
	{
		m_pViewStack->setCurrentWidget(pViewPage);
	}

	m_pTabBar->setEnabled(true);
}

IViewPage * MainWidget::createViewPage(const QString & viewId)
{
	return new IViewPage(this);
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
	if (TabButton* tabBtn = qobject_cast<TabButton*>(sender()))
	{
		selectedTabs(tabBtn);
	}
}

void MainWidget::doTabCloseClicked()
{
	if (TabButton* tabBtn = qobject_cast<TabButton*>(sender()))
	{
		IViewPage *pViewPage = m_oViewMap[tabBtn->text()];

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
			{ // ���һҳ
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

				// ���رյ�ǰҳ����������ǰ��һҳ
				if (n == 0)
					selectedTabs(m_tabs[0]);
				else
					selectedTabs(m_tabs[n-1]);
			}
		}

		// �ֶ�ɾ������
		delete tabBtn;
		delete pViewPage;

		// �����˵���λ��
		adjustTabBarWidth();
	}
}

void MainWidget::doLogin(QString userId, QString userPwd)
{
	m_pLoginWidget->slotLoginReturn(0, "");
}

void MainWidget::doOpenView(QTreeWidgetItem *item)
{
	if (item->childCount() > 0)
		return;

	// ��ȡID
	QString tabId = item->text(0);

	// ͨ��ID���Ұ�ť
	for (int i = 0; i < m_tabs.size(); i++)
	{
		if (m_tabs[i]->text() == tabId)
		{
			m_pCurTabBtn = m_tabs[i];
			m_pCurTabBtn->setChecked(true);
			QWidget *pView = m_oViewMap[tabId];
			if (m_pViewStack->currentWidget() != pView)
			{
				m_pViewStack->setCurrentWidget(pView);
			}
			return;
		}
	}

	// �����µ�Tab��ť
	TabButton* pTabBtn = new TabButton(m_pTabBar);
	pTabBtn->setText(tabId);
	pTabBtn->setCheckable(true);
	pTabBtn->setAutoRaise(true);
	pTabBtn->setMaximumWidth(m_nTabBtnWidth);
	pTabBtn->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
	connect(pTabBtn, SIGNAL(clicked()), this, SLOT(doTabButtonClicked()));
	connect(pTabBtn, SIGNAL(tabCloseClicked()), this, SLOT(doTabCloseClicked()));

	// ������ͼ
	// TODO: ������
	IViewPage *pViewPage = createViewPage(tabId);
	m_oViewMap[tabId] = pViewPage;

	// ����ͼ���뵽��������
	m_pViewStack->addWidget(pViewPage);
	m_pViewStack->setCurrentWidget(pViewPage);

	// ��Tab��ť���뵽��������
	m_pTabBtnGroup->addButton(pTabBtn);
	m_tabs.append(pTabBtn);
	m_pTabLayout->addWidget(pTabBtn);
	pTabBtn->setChecked(true);

	m_pCurTabBtn = pTabBtn;

	// �����˵����Ŀ��
	adjustTabBarWidth();
}