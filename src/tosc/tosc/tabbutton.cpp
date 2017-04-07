#include "tabbutton.h"

TabButton::TabButton(QWidget *parent)
	: QToolButton(parent)
	, m_clearButton(new QToolButton(this))
{
	m_clearButton->setFixedSize(20, 20);
	m_clearButton->setObjectName(QStringLiteral("tab-button-close"));
	m_clearButton->setVisible(true);
	m_clearButton->setCursor(Qt::PointingHandCursor);

	setContentsMargins(0, 0, 50, 0);

	connect(m_clearButton, &QToolButton::clicked, this, &TabButton::tabCloseClicked);
}

TabButton::~TabButton()
{

}

void TabButton::resizeEvent(QResizeEvent *event)
{
	QToolButton::resizeEvent(event);
	QSize clearButtonSize(20, 20);
	m_clearButton->move(rect().right() - clearButtonSize.width(),
		(rect().height() - clearButtonSize.height()) / 2);
}

void TabButton::setId(int id)
{
	this->m_id = id;
}

int TabButton::getId()
{
	return this->m_id;
}
