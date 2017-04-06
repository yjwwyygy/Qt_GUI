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
	/* 覆盖：定秒定时器触发事件 */
	virtual void timerEvent(QTimerEvent * evt);

	/* 覆盖：重置光标 */
	virtual bool eventFilter(QObject *watched, QEvent *event);

	// 布局标题栏部件
	virtual void layoutTitleBarWidgets();

private:
	// 设置UI
	void setupUi();

	// 设置状态条
	void setupStatusBar();

	/* 调整标签栏按钮宽度 */
	void adjustTabBarWidth();

	/* 选择指定视图为当前视图 */
	void selectedTabs(TabButton* pTabButton);

	void initTestData();

private slots:
	void doOpenView(QTreeWidgetItem *item);

	/* 标签按钮clicked()信号处理槽 */
	void doTabButtonClicked();

private:
	// Tap条
	QWidget* m_pTabBar = nullptr;
	QHBoxLayout* m_pTabLayout = nullptr;
	QButtonGroup* m_pTabBtnGroup = nullptr;

	QToolButton* m_pSysDrop = nullptr;			// 设置按钮

	// 树型菜单 | 视图堆栈区
	QTreeWidget* m_pViewTree = nullptr;
	QStackedWidget* m_pViewStack = nullptr;

	// 状态条
	QStatusBar* m_pStatusBar;
	QLabel* m_pTradeTime = nullptr;

	int m_nTimer;

	QList<TabButton*> m_tabs;

	int m_nTabBtnWidth = 160;
	int m_nTabBtnHeight = 29;			// 标题栏按钮高度

	QMap<QString, QWidget*> m_oViewMap;
};

#endif // MAINWIDGET_H
