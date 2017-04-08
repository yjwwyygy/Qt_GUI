#include "loginwidget.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "winres.h"

LoginWidget::LoginWidget(QWidget *parent)
	: BasicDialog(parent)
{
	setMaximumButtonVisible(false);
	ui.setupUi(this);
	ui.lbl_userName->setObjectName("lbl-userName");
	ui.lbl_password->setObjectName("lbl-password");
	ui.le_userName->setObjectName("le-userName");
	ui.le_password->setObjectName("le-password");
	ui.pb_login->setObjectName("pb-login");
	ui.pbar_Progress->setObjectName("pbar-progress");
	ui.widget_Login->setObjectName("widget-login");
	ui.lbl_ProgressMsg->setObjectName("lbl-progressmsg");
	ui.widget_progress->setObjectName("widget-progress");

	ui.le_userName->setFavIcon(QIcon(QStringLiteral(":/tosc/img/login/user")));
	ui.le_password->setFavIcon(QIcon(QStringLiteral(":/tosc/img/login/passwd")));

	ui.widget_progress->setHidden(true);
	
	connect(ui.pb_login, &QToolButton::clicked, this, &LoginWidget::slotLoginClicked);

	QFile f(WRS_Login_Widget_Qss);
	if (f.open(QIODevice::ReadOnly))
	{
		QTextStream in(&f);
		setStyleSheet(in.readAll());
	}
}

LoginWidget::~LoginWidget()
{

}

void LoginWidget::slotLoginClicked()
{
	QString userName = ui.le_userName->text();
	QString password = ui.le_password->text();

	if ( "" == userName || "" == password)
	{
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("用户名或密码不能为空！"));
		return;
	}

	ui.widget_Login->setHidden(true);
	ui.widget_progress->setHidden(false);

	emit onLogin(userName, password);
}

void LoginWidget::doLoginReturn(int retCode, const QString &msg)
{
	if (retCode == 0)
	{
		accept();
	}
	else
	{
		ui.widget_Login->setHidden(false);
		ui.widget_progress->setHidden(true);
	}
}

void LoginWidget::doShowProgressMsg(int progress, const QString &msg)
{
	ui.pbar_Progress->setValue(progress);
	ui.lbl_ProgressMsg->setText(msg);

	qApp->processEvents();
}