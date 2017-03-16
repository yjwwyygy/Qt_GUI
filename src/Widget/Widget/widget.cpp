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
	pHelper->activateOn(this);  //���ǰ����
	pHelper->setTitleHeight(pTitleBar->height());  //���ô���ı������߶�
	pHelper->setWidgetMovable(true);  //���ô�����ƶ�
	pHelper->setWidgetResizable(true);  //���ô��������
	pHelper->setRubberBandOnMove(true);  //������Ƥ��Ч��-���ƶ�
	pHelper->setRubberBandOnResize(true);  //������Ƥ��Ч��-������

	//showInformation(this, QStringLiteral("��ʾ"), QStringLiteral("����һ����ͨ����ʾ��-Information��"), QMessageBox::Ok, QMessageBox::Ok);
}

Widget::~Widget()
{

}
