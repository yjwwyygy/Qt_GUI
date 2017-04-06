#include "basicdialog.h"
#include "winres.h"
#include <QMouseEvent>
#include <QAction>
#include <QToolButton>
#include <QPainter>
#include <QtMath>

/* 标题栏文字缩进像素数 */
static const int CWinTitleTextIndent = 8;

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
	return QSize(960, 560);
}

QSize BasicDialog::minimumSizeHint() const
{
	return QSize(400, 400);
}

bool BasicDialog::forbidEscapeKeyClose() const
{
	return m_bForbidEscapeKeyClose;
}

void BasicDialog::setForbidEscapeKeyClose(bool value)
{
	m_bForbidEscapeKeyClose = value;
}

bool BasicDialog::isIconVisible() const
{
	return m_bIconVisible;
}

void BasicDialog::setIconVisible(bool value)
{
	m_bIconVisible = value;
}

bool BasicDialog::isTitleVisible() const
{
	return m_bTitleVisible;
}

void BasicDialog::setTitleVisible(bool value)
{
	m_bTitleVisible = value;
}

bool BasicDialog::isResizable() const
{
	return m_bResizable;
}

void BasicDialog::setResizable(bool value)
{
	m_bResizable = value;
}

bool BasicDialog::isMinimumButtonVisible() const
{
	return m_bMinBtnVisible;
}

void BasicDialog::setMinimumButtonVisible(bool value)
{
	m_bMinBtnVisible = value;
}

bool BasicDialog::isMaximumButtonVisible() const
{
	return m_bMaxBtnVisible;
}

void BasicDialog::setMaximumButtonVisible(bool value)
{
	m_bMaxBtnVisible = value;
}

bool BasicDialog::isCloseButtonVisible() const
{
	return m_bCloseBtnVisible;
}

void BasicDialog::setCloseButtonVisible(bool value)
{
	m_bCloseBtnVisible = value;
}

void BasicDialog::addTitleBarWidget(QWidget * pWidget, int nAlign, int nSpacing, bool bAutoFillRest)
{
	if (pWidget->parent() != this)
	{
		pWidget->setParent(this);
	}
	m_oTitleBarWidgets.append(TitleBarWidgetItem(pWidget, nAlign, nSpacing, bAutoFillRest));

	if (isVisible())
	{
		layoutTitleBarWidgets();
	}
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

		// 窗口最大化时,先还原,再移动
		if (isMaximized())
		{
			return; // 最大化禁止移动
			doActionMaxTriggered();
		}
		else
		{
			// 标题栏不要移出屏幕之外
			QPoint p = pos() + (event->pos() - m_pMousePressPos);
			if (p.y() < 0)
			{
				p.setY(0);
			}

			move(p);
		}
	}
	else if (m_bMouseOnBorder)
	{
		event->accept();
		changeSize(event);
	}
	else if (!isMaximized() && isResizable())
	{
		if (isMouseOnBorder(event->pos()))
		{ // 改变光标形状
			event->accept();
			setMouseResizeCursor(event->pos());
		}
		else
		{ // 还原光标形状
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

void BasicDialog::showEvent(QShowEvent * event)
{
	layoutTitleBarWidgets();
}

void BasicDialog::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	drawBorder(painter);
	drawTitleBar(painter);
	drawTitleText(painter, windowTitle());
}

bool BasicDialog::event(QEvent * event)
{
	switch (event->type()) {
	case QEvent::Resize:
		//case QEvent::StyleChange:
		//case QEvent::FontChange:
	case QEvent::WindowTitleChange:
	case QEvent::WindowStateChange:
		layoutTitleBarWidgets();
	}

	// 检查光标
	//if (!m_bMousePressedOnBorder && m_eMousePos != mpCenter && evt->type() == QEvent::ToolTip)
	//{
	//	m_eMousePos = mpCenter;
	//	setCursor(Qt::ArrowCursor);
	//	return QDialog::event(evt);
	//}

	return QDialog::event(event);
}

void BasicDialog::keyPressEvent(QKeyEvent * event)
{
	if (m_bForbidEscapeKeyClose && !event->modifiers() && event->key() == Qt::Key_Escape)
	{
		event->ignore();
		return;
	}

	QDialog::keyPressEvent(event);
}

void BasicDialog::drawBorder(QPainter & painter)
{
	int nOffsetWidth = m_nBorderWidth + m_nBorderWidth % 2 + m_nShadowWidth * 2;
	QRect rtBorder = QRect(qCeil(m_nBorderWidth / 2.0) + m_nShadowWidth,
		qCeil(m_nBorderWidth / 2.0) + m_nShadowWidth,
		width() - nOffsetWidth, height() - nOffsetWidth);

	// 绘制边框和背景
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (m_nBorderWidth > 0)
	{
		QPen pen = QPen(isActiveWindow() ? borderActiveColor() : borderColor());
		pen.setWidth(m_nBorderWidth);
		painter.setPen(pen);
		painter.setBrush(palette().background());
		painter.drawRect(rtBorder);
	}
	else
	{
		painter.fillRect(rtBorder, palette().background());
	}

	// 绘制阴影
	QColor color = m_cShadowColor;
	for (int i = 0; i < m_nShadowWidth; ++i)
	{
		int nOffset = m_nShadowWidth - i;
		QPolygon polygon;
		polygon << QPoint(nOffset, nOffset)
			<< QPoint(width() - nOffset, m_nShadowWidth - i)
			<< QPoint(width() - nOffset, height() - nOffset)
			<< QPoint(nOffset, height() - nOffset)
			<< QPoint(nOffset, nOffset);
		int nAlpha = 100 - qSqrt(i) * 60;
		if (nAlpha < 0) nAlpha = 1;
		color.setAlpha(nAlpha);
		painter.setPen(color);
		painter.drawPolyline(polygon);
	}
}

void BasicDialog::drawTitleBar(QPainter & painter)
{
	if (isActiveWindow())
	{
		painter.drawPixmap(titleRect(), QPixmap(WRS_Win_Title_Background_Active));
	}
	else
	{
		painter.drawPixmap(titleRect(), QPixmap(WRS_Win_Title_Background));
	}

	if (isIconVisible())
	{
		int nOffsetX = (titleBarHeight() - m_oIconSize.width()) / 2 + borderWidth();
		int nOffsetY = (titleBarHeight() - m_oIconSize.height()) / 2 + borderWidth();
		QRect rt = QRect(nOffsetX, nOffsetY, m_oIconSize.width(), m_oIconSize.height());
		windowIcon().paint(&painter, rt);
	}
}

void BasicDialog::drawTitleText(QPainter & painter, const QString & sTitle)
{
	int nLeft = borderWidth() + CWinTitleTextIndent;
	if (isIconVisible()) nLeft += m_oIconSize.width() + CWinTitleTextIndent;
	QRect rt = QRect(nLeft, borderWidth(), m_nTitleTextMaxRightPos - nLeft, m_nTitleBarHeight);
	QTextOption to(Qt::AlignLeft | Qt::AlignVCenter);
	to.setWrapMode(QTextOption::NoWrap);

	// 设置字体大小
	//QFont ft = font();
	//ft.setBold(true);

	// 绘制文字
	//painter.setFont(ft);
	painter.setPen(titleFontColor());
	painter.drawText(rt, sTitle, to);
}

int BasicDialog::titleTextRightPos() const
{
	return m_nTitleTextRightPos;
}

int BasicDialog::titleTextMaxRightPos() const
{
	return m_nTitleTextMaxRightPos;
}

int BasicDialog::calcTitleBarLeftBeginPosition() const
{
	int nLeft = m_nShadowWidth + m_nBorderWidth;
	if (isIconVisible()) nLeft += m_oIconSize.width() + CWinTitleTextIndent;

	if (isTitleVisible())
	{
		// 字体大小
		QFont ft = font();
		//ft.setBold(true);

		// 计算文字宽度
		QFontMetrics metr(ft);
		nLeft = nLeft + metr.width(windowTitle()) + CWinTitleTextIndent + 8;
	}
	
	return nLeft;
}

void BasicDialog::layoutTitleBarWidgets()
{
	// 靠左布局起始位置
	m_nTitleTextRightPos = calcTitleBarLeftBeginPosition();
	int nLeft = m_nTitleTextRightPos;

	// 靠右布局起始位置
	int nRight = width() - borderWidth();

	// 布局
	for (int i = 0; i < m_oTitleBarWidgets.size(); ++i)
	{
		TitleBarWidgetItem item = std::move(m_oTitleBarWidgets[i]);
		if (!item.widget || !item.widget->isVisible()) continue;

		// 顶边位置
		int nTop;
		if (item.widgetAlign & Qt::AlignTop)
			nTop = borderWidth();
		else if (item.widgetAlign & Qt::AlignVCenter)
			nTop = (titleBarHeight() - item.widget->height()) / 2 + borderWidth();
		else if (item.widgetAlign & Qt::AlignBottom)
			nTop = (titleBarHeight() - item.widget->height()) + borderWidth();
		else
			nTop = (titleBarHeight() - item.widget->height()) / 2 + borderWidth();

		// 如果是自动充满则计算宽度
		int nWidth = -1;
		if (item.autoFillRest)
		{
			int nW = 0;
			for (int j = i + 1; j < m_oTitleBarWidgets.size(); ++j)
			{
				TitleBarWidgetItem item = std::move(m_oTitleBarWidgets[j]);
				if (!item.widget || !item.widget->isVisible()) continue;
				nW += item.widget->width() + item.spacing;
			}
			nWidth = nRight - nLeft - nW - item.spacing;
			if (nWidth < 0) nWidth = 0;
		}

		// 靠左显示
		if (item.widgetAlign & Qt::AlignLeft)
		{
			nLeft += item.spacing;
			if (item.widget->geometry().left() != nLeft || nWidth > -1)
			{
				if (nWidth > -1)
				{
					QRect rt = QRect(nLeft, nTop, nWidth, item.widget->height());
					item.widget->setGeometry(rt);
				}
				else
					item.widget->move(nLeft, nTop);
			}
			nLeft += item.widget->width();
			continue;
		}

		// 靠右显示
		if (nWidth < 0)
		{
			nRight -= item.widget->width() + item.spacing;
			if (item.widget->geometry().left() != nRight)
				item.widget->move(nRight, nTop);
		}
		else
		{
			nRight -= nWidth + item.spacing;
			QRect rt = QRect(nRight, nTop, nWidth, item.widget->height());
			item.widget->setGeometry(rt);
		}
	}

	m_nTitleTextMaxRightPos = nRight;
}

int BasicDialog::borderWidth() const
{
	return m_nShadowWidth + m_nBorderWidth;
}

int BasicDialog::titleBarHeight() const
{
	return m_nTitleBarHeight;
}

inline QColor BasicDialog::borderColor() const
{
	return m_cBorderColor;
}

inline QColor BasicDialog::borderActiveColor() const
{
	return m_cBorderActiveColor;
}

inline QColor BasicDialog::shadowColor() const
{
	return m_cShadowColor;
}

inline QColor BasicDialog::titleFontColor() const
{
	return m_cTitleFontColor;
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
		m_pActMax->setIcon(QIcon(WRS_Win_Button_Icon_Max));
		m_pActMax->setText(QStringLiteral("最大化"));
	}
	else
	{
		showMaximized();
		m_pActMax->setIcon(QIcon(WRS_Win_Button_Icon_Restore));
		m_pActMax->setText(QStringLiteral("还原"));
	}
}

void BasicDialog::doActionMinTriggered()
{
	showMinimized();
}

void BasicDialog::setupUi()
{
	// 只有调用该函数，才能捕获鼠标未按下时的鼠标移动事件
	setMouseTracking(true);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	setWindowIcon(QIcon(WRS_Win_Icon));

	setContentsMargins(borderWidth(),
						borderWidth() + m_nTitleBarHeight,
						borderWidth(),
						borderWidth());

	m_pActMin = new QAction(QIcon(WRS_Win_Button_Icon_Min), QStringLiteral("最小化"), this);
	connect(m_pActMin, SIGNAL(triggered()), this, SLOT(doActionMinTriggered()));
	m_pActMax = new QAction(QIcon(WRS_Win_Button_Icon_Max), QStringLiteral("最大化"), this);
	connect(m_pActMax, SIGNAL(triggered()), this, SLOT(doActionMaxTriggered()));
	m_pActClose = new QAction(QIcon(WRS_Win_Button_Icon_Close), QStringLiteral("关闭"), this);
	connect(m_pActClose, SIGNAL(triggered()), this, SLOT(doActionCloseTriggered()));

	m_pSysCloseButton = new QToolButton(this);
	m_pSysCloseButton->setMouseTracking(true);
	m_pSysCloseButton->setObjectName("win-close");
	m_pSysCloseButton->setAutoRaise(true);
	m_pSysCloseButton->setFocusPolicy(Qt::NoFocus);
	m_pSysCloseButton->setVisible(m_bCloseBtnVisible);
	m_pSysCloseButton->setDefaultAction(m_pActClose);
	m_pSysCloseButton->resize(m_nTitleBtnWidth + 8, m_nTitleBtnHeight);
	m_pSysCloseButton->setIconSize(QSize(m_nTitleBtnWidth + 8, m_nTitleBtnHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysCloseButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMaxButton = new QToolButton(this);
	m_pSysMaxButton->setMouseTracking(true);
	m_pSysMaxButton->setObjectName("win-max");
	m_pSysMaxButton->setAutoRaise(true);
	m_pSysMaxButton->setFocusPolicy(Qt::NoFocus);
	m_pSysMaxButton->setVisible(m_bMaxBtnVisible);
	m_pSysMaxButton->setDefaultAction(m_pActMax);
	m_pSysMaxButton->resize(m_nTitleBtnWidth, m_nTitleBtnHeight);
	m_pSysMaxButton->setIconSize(QSize(m_nTitleBtnWidth, m_nTitleBtnHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysMaxButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMinButton = new QToolButton(this);
	m_pSysMinButton->setMouseTracking(true);
	m_pSysMinButton->setObjectName("win-min");
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
	{ // 右下角
		m_eMousePos = MP_BottomRight;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorderRect().contains(pos) && topBorderRect().contains(pos))
	{ // 右上角
		m_eMousePos = MP_TopRight;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorderRect().contains(pos) && bottomBorderRect().contains(pos))
	{ // 左下角
		m_eMousePos = MP_BottomLeft;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorderRect().contains(pos) && topBorderRect().contains(pos))
	{ // 左上角
		m_eMousePos = MP_TopLeft;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorderRect().contains(pos))
	{ // 右边
		m_eMousePos = MP_Right;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (bottomBorderRect().contains(pos))
	{ // 底边
		m_eMousePos = MP_Bottom;
		setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (leftBorderRect().contains(pos))
	{ // 左边
		m_eMousePos = MP_Left;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (topBorderRect().contains(pos))
	{ // 上边
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
