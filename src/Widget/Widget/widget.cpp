#include "widget.h"
#include "title_bar.h"
#include <QVBoxLayout>
#include "frameless_helper.h"
#include "message_box.h"

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());

	TitleBar *pTitleBar = new TitleBar(this);
	installEventFilter(pTitleBar);

	resize(400, 300);
	setWindowTitle(tr("Custom Window"));
	setWindowIcon(QIcon(":/Images/logo"));

	QPalette pal(palette());
	pal.setColor(QPalette::Background, QColor(50, 50, 50));
	setAutoFillBackground(true);
	setPalette(pal);

	QVBoxLayout *pLayout = new QVBoxLayout();
	pLayout->addWidget(pTitleBar);
	pLayout->addStretch();
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(pLayout);

	FramelessHelper *pHelper = new FramelessHelper(this);
	pHelper->activateOn(this);  //激活当前窗体
	pHelper->setTitleHeight(pTitleBar->height());  //设置窗体的标题栏高度
	pHelper->setWidgetMovable(true);  //设置窗体可移动
	pHelper->setWidgetResizable(true);  //设置窗体可缩放
	pHelper->setRubberBandOnMove(true);  //设置橡皮筋效果-可移动
	pHelper->setRubberBandOnResize(true);  //设置橡皮筋效果-可缩放

	//showInformation(this, QStringLiteral("提示"), QStringLiteral("这是一个普通的提示框-Information！"), QMessageBox::Ok, QMessageBox::Ok);
}

Widget::~Widget()
{

}
