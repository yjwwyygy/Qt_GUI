#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "ui_loginwidget.h"

#include "basicdialog.h"

class LoginWidget : public BasicDialog
{
	Q_OBJECT

public:
	LoginWidget(QWidget *parent = 0);
	~LoginWidget();

signals:
	void onLogin(const QString &userName, const QString &password);

public slots:
	// ��¼У���
	void doLoginReturn(int retCode, const QString &info);

	// ��¼��������
	void doShowProgressMsg(int progress, const QString &titile);

private slots:
	void slotLoginClicked();

private:
	Ui::LoginWidget ui;
};

#endif // LOGINWIDGET_H
