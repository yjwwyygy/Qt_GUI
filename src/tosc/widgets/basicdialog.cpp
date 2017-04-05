#include "basicdialog.h"
#include <QMouseEvent>
#include <QAction>
#include <QToolButton>

BasicDialog::BasicDialog(QWidget *parent, int nShadowWidth, int nBorderWidth)
	: QDialog(parent)
	, m_nShadowWidth(nShadowWidth)
	, m_nBorderWidth(nBorderWidth)
{
	setupUi();
}

BasicDialog::~BasicDialog()
{

}

QSize BasicDialog::sizeHint() const
{
	return QSize(400, 400);
}

QSize BasicDialog::minimumSizeHint() const
{
	return QSize(400, 400);
}

bool BasicDialog::isResizable()
{
	return m_bResizable;
}

void BasicDialog::setResizable(bool value)
{
	m_bResizable = value;
}

bool BasicDialog::isMinimumButtonVisible()
{
	return m_bMinBtnVisible;
}

void BasicDialog::setMinimumButtonVisible(bool value)
{
	m_bMinBtnVisible = value;
}

bool BasicDialog::isMaximumButtonVisible()
{
	return m_bMaxBtnVisible;
}

void BasicDialog::setMaximumButtonVisible(bool value)
{
	m_bMaxBtnVisible = value;
}

bool BasicDialog::isCloseButtonVisible()
{
	return m_bCloseBtnVisible;
}

void BasicDialog::setCloseButtonVisible(bool value)
{
	m_bCloseBtnVisible = value;
}

void BasicDialog::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (isMouseOnTitleBar(event->pos()))
		{
			m_bMouseOnTitleBar = true;
			m_bMouseOnBorder = false;
			m_pMousePressPos = event->pos();
		}
		else if (isMouseOnBorder(event->pos()))
		{
			m_bMouseOnTitleBar = false;
			m_bMouseOnBorder = true;
			m_pMousePressPos = event->pos();
			m_pPressDragGlobalPos = event->globalPos();
		}
	}
}

void BasicDialog::mouseMoveEvent(QMouseEvent * event)
{
	if (m_bMouseOnTitleBar)
	{
		event->accept();

		// ��������Ҫ�Ƴ���Ļ֮��
		QPoint p = pos() + (event->pos() - m_pMousePressPos);
		if (p.y() < 0)
		{
			p.setY(0);
		}

		move(p);
	}
	else if (m_bMouseOnBorder)
	{
		event->accept();
		changeSize(event);
	}
	else if (!isMaximized() && isResizable())
	{
		if (isMouseOnBorder(event->pos()))
		{ // �ı�����״
			event->accept();
			setMouseResizeCursor(event->pos());
		}
		else
		{ // ��ԭ�����״
			setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void BasicDialog::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMouseOnTitleBar = false;
		m_bMouseOnBorder = false;
	}
}

void BasicDialog::mouseDoubleClickEvent(QMouseEvent * event)
{
	if (isMaximumButtonVisible() && titleRect().contains(event->pos()))
	{
		doActionMaxTriggered();
	}
}

void BasicDialog::layoutTitleBarWidgets()
{
	// ��������ߵ���ʼλ��

	// �������ұߵ���ʼλ��
}

void BasicDialog::doActionCloseTriggered()
{
	close();
}

void BasicDialog::doActionMaxTriggered()
{
	if (isMaximized())
	{
		showNormal();
		m_pActMax->setIcon(QIcon(""));
		m_pActMax->setText(QStringLiteral("���"));
	}
	else
	{
		showMaximized();
		m_pActMax->setIcon(QIcon(""));
		m_pActMax->setText(QStringLiteral("��ԭ"));
	}
}

void BasicDialog::doActionMinTriggered()
{
	showMinimized();
}

void BasicDialog::setupUi()
{
	// ֻ�е��øú��������ܲ������δ����ʱ������ƶ��¼�
	setMouseTracking(true);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());

	m_pActMin = new QAction(QIcon(""), QStringLiteral("��С��"), this);
	connect(m_pActMin, SIGNAL(triggered()), this, SLOT(doActionMinTriggered()));
	m_pActMax = new QAction(QIcon(""), QStringLiteral("���"), this);
	connect(m_pActMax, SIGNAL(triggered()), this, SLOT(doActionMaxTriggered()));
	m_pActClose = new QAction(QIcon(""), QStringLiteral("�ر�"), this);
	connect(m_pActClose, SIGNAL(triggered()), this, SLOT(doActionCloseTriggered()));

	m_pSysCloseButton = new QToolButton(this);
	//m_pSysCloseButton->setMouseTracking(true);
	//m_pSysCloseButton->setObjectName("win-close");
	m_pSysCloseButton->setAutoRaise(true);
	m_pSysCloseButton->setFocusPolicy(Qt::NoFocus);
	m_pSysCloseButton->setVisible(m_bCloseBtnVisible);
	m_pSysCloseButton->setDefaultAction(m_pActClose);
	m_pSysCloseButton->resize(m_nTitleBtnWidth + 8, m_nTitleBtnHeight);
	m_pSysCloseButton->setIconSize(QSize(m_nTitleBtnWidth + 8, m_nTitleBtnHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysCloseButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMaxButton = new QToolButton(this);
	//m_pSysMaxButton->setMouseTracking(true);
	//m_pSysMaxButton->setObjectName("win-max");
	m_pSysMaxButton->setAutoRaise(true);
	m_pSysMaxButton->setFocusPolicy(Qt::NoFocus);
	m_pSysMaxButton->setVisible(m_bMaxBtnVisible);
	m_pSysMaxButton->setDefaultAction(m_pActMax);
	m_pSysMaxButton->resize(m_nTitleBtnWidth, m_nTitleBtnHeight);
	m_pSysMaxButton->setIconSize(QSize(m_nTitleBtnWidth, m_nTitleBtnHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysMaxButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMinButton = new QToolButton(this);
	//m_pSysMinButton->setMouseTracking(true);
	//m_pSysMinButton->setObjectName("win-min");
	m_pSysMinButton->setAutoRaise(true);
	m_pSysMinButton->setFocusPolicy(Qt::NoFocus);
	m_pSysMinButton->setVisible(m_bMinBtnVisible);
	m_pSysMinButton->setDefaultAction(m_pActMin);
	m_pSysMinButton->resize(m_nTitleBtnWidth, m_nTitleBtnHeight);
	m_pSysMinButton->setIconSize(QSize(m_nTitleBtnWidth, m_nTitleBtnHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysMinButton, Qt::AlignRight + Qt::AlignTop));
}

bool BasicDialog::isMouseOnTitleBar(const QPoint & pos)
{
	return titleRect().contains(pos);
}

bool BasicDialog::isMouseOnBorder(const QPoint & pos)
{
	return leftBorderRect().contains(pos) 
			|| topBorderRect().contains(pos)
			|| rightBorderRect().contains(pos)
			|| bottomBorderRect().contains(pos);
}

QRect BasicDialog::titleRect() const
{
	return QRect(m_nShadowWidth + m_nBorderWidth,
				m_nShadowWidth + m_nBorderWidth,
				width() - 2 * (m_nShadowWidth + m_nBorderWidth),
				m_nTitleBarHeight);
}

QRect BasicDialog::leftBorderRect() const
{
	return QRect(0, 0, m_nShadowWidth + m_nBorderWidth, height());
}

QRect BasicDialog::topBorderRect() const
{
	return QRect(0, 0, width(), m_nShadowWidth + m_nBorderWidth);
}

QRect BasicDialog::rightBorderRect() const
{
	return QRect(width() - (m_nShadowWidth + m_nBorderWidth), 0, m_nShadowWidth + m_nBorderWidth, height());
}

QRect BasicDialog::bottomBorderRect() const
{
	return QRect(0, height() - (m_nShadowWidth + m_nBorderWidth), width(), m_nShadowWidth + m_nBorderWidth);
}

void BasicDialog::setMouseResizeCursor(const QPoint & pos)
{
	if (rightBorderRect().contains(pos) && bottomBorderRect().contains(pos))
	{ // ���½�
		m_eMousePos = MP_BottomRight;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorderRect().contains(pos) && topBorderRect().contains(pos))
	{ // ���Ͻ�
		m_eMousePos = MP_TopRight;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorderRect().contains(pos) && bottomBorderRect().contains(pos))
	{ // ���½�
		m_eMousePos = MP_BottomLeft;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorderRect().contains(pos) && topBorderRect().contains(pos))
	{ // ���Ͻ�
		m_eMousePos = MP_TopLeft;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorderRect().contains(pos))
	{ // �ұ�
		m_eMousePos = MP_Right;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (bottomBorderRect().contains(pos))
	{ // �ױ�
		m_eMousePos = MP_Bottom;
		setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (leftBorderRect().contains(pos))
	{ // ���
		m_eMousePos = MP_Left;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (topBorderRect().contains(pos))
	{ // �ϱ�
		m_eMousePos = MP_Top;
		setCursor(QCursor(Qt::SizeVerCursor));
	}
	else
	{
		m_eMousePos = MP_Center;
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void BasicDialog::changeSize(QMouseEvent * event)
{
	QRect oldRect = geometry();
	QPoint topLeft = oldRect.topLeft();
	QPoint bottomRight = oldRect.bottomRight();

	QPoint diffPos = event->globalPos() - m_pPressDragGlobalPos;
	m_pPressDragGlobalPos = event->globalPos();

	switch (m_eMousePos)
	{
	case BasicDialog::MP_Center:
		break;
	case BasicDialog::MP_Left:
		topLeft.setX(topLeft.x() + diffPos.x());
		break;
	case BasicDialog::MP_TopLeft:
		topLeft += diffPos;
		break;
	case BasicDialog::MP_Top:
		topLeft.setY(topLeft.y() + diffPos.y());
		break;
	case BasicDialog::MP_TopRight:
		topLeft.setY(topLeft.y() + diffPos.y());
		bottomRight.setX(bottomRight.x() + diffPos.x());
		break;
	case BasicDialog::MP_Right:
		bottomRight.setX(bottomRight.x() + diffPos.x());
		break;
	case BasicDialog::MP_BottomRight:
		bottomRight += diffPos;
		break;
	case BasicDialog::MP_Bottom:
		bottomRight.setY(bottomRight.y() + diffPos.y());
		break;
	case BasicDialog::MP_BottomLeft:
		topLeft.setX(topLeft.x() + diffPos.x());
		bottomRight.setY(bottomRight.y() + diffPos.y());
		break;
	default:
		break;
	}

	QRect newRect(topLeft, bottomRight);
	if (newRect != oldRect)
	{
		setGeometry(newRect);
	}
}
