#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "cdialog.h"
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QStatusBar>
#include <QLabel>

class MainWidget : public CDialog
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

protected:
	/* 覆盖：定秒定时器触发事件 */
	virtual void timerEvent(QTimerEvent * evt);

private:
	void setupUi();
	void setupStatusBar();

	void initTestData();

private slots:
	void doOpenView(QTreeWidgetItem *item);

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

	QList<QToolButton*> m_tabs;
};

#endif // MAINWIDGET_H
