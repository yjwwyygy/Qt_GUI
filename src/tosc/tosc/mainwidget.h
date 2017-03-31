#ifndef MAINWIDGET_H
#define MAINWIDGET_H

//#include <QtWidgets/QDialog>
#include "kdialog.h"
#include "ui_mainwidget.h"

class MainWidget : public KDialog
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

private:
	Ui::MainWidgetClass ui;
};

#endif // MAINWIDGET_H
