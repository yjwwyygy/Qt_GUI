#include "icolineedit.h"

IcoLineEdit::IcoLineEdit(QWidget *parent)
	: QLineEdit(parent)
	, m_favButton(new QToolButton(this))
{
	/*QString style(QStringLiteral("QToolButton { border: none; padding: 1px; }"));
	m_favButton->setStyleSheet(style);*/
	m_favButton->setObjectName("pb-icoButton");

	//setStyleSheet(QStringLiteral("QLineEdit { padding-left: %1px;  } ")
	//	.arg(m_favButton->sizeHint().width()));
}

IcoLineEdit::~IcoLineEdit()
{

}

void IcoLineEdit::setFavIcon(const QIcon &icon)
{
	QPixmap pixmap = icon.pixmap(16, 16);
	m_favButton->setIcon(pixmap);
}

void IcoLineEdit::resizeEvent(QResizeEvent *event)
{
	QLineEdit::resizeEvent(event);
	m_favButton->move(rect().left(), ( rect().bottom() - (rect().top() - rect().bottom()) / 4 - m_favButton->sizeHint().height()) / 2);
}