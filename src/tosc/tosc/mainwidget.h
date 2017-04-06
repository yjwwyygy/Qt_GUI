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

class MainWidget : public BasicDialog
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

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

	/* ������ǩ����ť��� */
	void adjustTabBarWidth();

	/* ѡ��ָ����ͼΪ��ǰ��ͼ */
	void selectedTabs(TabButton* pTabButton);

	void initTestData();

private slots:
	void doOpenView(QTreeWidgetItem *item);

	/* ��ǩ��ťclicked()�źŴ���� */
	void doTabButtonClicked();

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
	int m_nTabBtnHeight = 29;			// ��������ť�߶�

	QMap<QString, QWidget*> m_oViewMap;
};

#endif // MAINWIDGET_H
