#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "ui_loginwidget.h"

#include "basicdialog.h"

enum ProgressDegree
{
	PD_FIRST = 0,
	PD_SECOND = 20,
	PD_THIRD = 40,
	PD_FORTH = 60,
	PD_FIFTH = 80,
	PD_SIXED = 100,
};

class LoginWidget : public BasicDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = 0);
	~LoginWidget();

signals:
	void reqLogin(QString userName, QString password);

public slots:
	// ��¼У���
	void slotLoginReturn(int retCode, QString info);

	// ��¼��������
	void slotProgress(int progreee, QString titile);

private slots:
	void slotLoginClicked();

private:
	Ui::LoginWidget ui;
};

#endif // LOGINWIDGET_H
