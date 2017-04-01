#include "custom_window.h"
#include <QHBoxLayout>
#include <QEvent>

CustomWindow::CustomWindow(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());

	m_pIconLabel = new QLabel(this);
	m_pTitleLabel = new QLabel(this);
	m_pCloseButton = new QPushButton(this);

	m_pIconLabel->setFixedSize(20, 20);
	m_pIconLabel->setScaledContents(true);

	m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	m_pCloseButton->setFixedSize(27, 22);

	m_pTitleLabel->setObjectName("whiteLabel");
	m_pCloseButton->setObjectName("closeButton");

	m_pCloseButton->setToolTip(tr("Close"));

	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->addWidget(m_pIconLabel);
	pLayout->addSpacing(5);
	pLayout->addWidget(m_pTitleLabel);
	pLayout->addWidget(m_pCloseButton);
	pLayout->setSpacing(0);
	pLayout->setContentsMargins(5, 0, 5, 0);

	QWidget *pTitleBar = new QWidget(this);
	pTitleBar->setLayout(pLayout);

	m_pLayout = new QVBoxLayout(this);
	m_pLayout->addWidget(pTitleBar);
	m_pLayout->addStretch();
	m_pLayout->setSpacing(0);
	m_pLayout->setContentsMargins(0, 0, 0, 0);

	setLayout(m_pLayout);

	connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

	this->installEventFilter(this);
}

CustomWindow::~CustomWindow()
{

}

void CustomWindow::onClicked()
{
	this->window()->close();
}

bool CustomWindow::eventFilter(QObject * obj, QEvent * event)
{
	switch (event->type())
	{
	case QEvent::WindowTitleChange:
	{
		QWidget *pWidget = qobject_cast<QWidget *>(obj);
		if (pWidget)
		{
			m_pTitleLabel->setText(pWidget->windowTitle());
			return true;
		}
	}
	case QEvent::WindowIconChange:
	{
		QWidget *pWidget = qobject_cast<QWidget *>(obj);
		if (pWidget)
		{
			QIcon icon = pWidget->windowIcon();
			m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
			return true;
		}
	}
	}

	return QWidget::eventFilter(obj, event);
}
