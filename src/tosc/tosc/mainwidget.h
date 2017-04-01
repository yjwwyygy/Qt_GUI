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
	/* ���ǣ����붨ʱ�������¼� */
	virtual void timerEvent(QTimerEvent * evt);

private:
	void setupUi();
	void setupStatusBar();

	void initTestData();

private slots:
	void doOpenView(QTreeWidgetItem *item);

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

	QList<QToolButton*> m_tabs;
};

#endif // MAINWIDGET_H
