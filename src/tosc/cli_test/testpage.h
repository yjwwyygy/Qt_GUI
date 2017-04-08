#ifndef TESTPAGE_H
#define TESTPAGE_H

#include "iviewPage.h"
#include "ui_testpage.h"

class TestPage : public IViewPage
{
	Q_OBJECT

public:
	TestPage(QWidget *parent = 0);
	~TestPage();

private:
	Ui::TestPage ui;
};

#endif // TESTPAGE_H
