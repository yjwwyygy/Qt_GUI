#include "cdialog.h"
#include "winres.h"
#include <QPixmap>
#include <QPainter>
#include <QWindow>
#include <QtMath>
#include <QLayout>
#include <QApplication>
#include <QDesktopWidget>

/* 窗口标题边框索引 */
static const int CWBI_Title = 0;
static const int CWBI_Border = 1;
static const int CWBI_Conner = 2;

/* 标题栏文字缩进像素数 */
static const int CWinTitleTextIndent = 8;

CDialog::CDialog(QWidget *parent, int nShadowWidth, int nBorderWidth)
	: QDialog(parent)
{
	setWinShadowWidth(nShadowWidth);
	setWinBorderWidth(nBorderWidth);
	setupUi();
	doSystemStyleChanged();
	m_pFocusSwitchHelper = new CFocusSwitchHelper<CDialog>(this);
}

CDialog::~CDialog()
{
	delete m_pFocusSwitchHelper;
	delete actQuit;
	delete actMax;
	delete actMin;

	delete m_pSysMinButton;
	delete m_pSysMaxButton;
	delete m_pSysQuitButton;
}

void CDialog::setupUi()
{
	setMouseTracking(true);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint 
					| Qt::WindowSystemMenuHint
					| Qt::WindowMinimizeButtonHint
					| Qt::WindowMaximizeButtonHint
					| Qt::WindowMinMaxButtonsHint);
	updateContentsMargins();

	// 系统窗口操作
	actQuit = new QAction(QIcon(WRS_Win_Button_Icon_Close), KSL("关闭"), this);
	connect(actQuit, SIGNAL(triggered()), this, SLOT(doActionCloseTriggered()));
	actMax = new QAction(QIcon(WRS_Win_Button_Icon_Max), KSL("最大化"), this);
	connect(actMax, SIGNAL(triggered()), this, SLOT(doActionMaxTriggered()));
	actMin = new QAction(QIcon(WRS_Win_Button_Icon_Min), KSL("最小化"), this);
	connect(actMin, SIGNAL(triggered()), this, SLOT(doActionMinTriggered()));

	m_pSysQuitButton = new QToolButton(this);
	m_pSysQuitButton->setMouseTracking(true);
	m_pSysQuitButton->setObjectName("win-close");
	m_pSysQuitButton->setAutoRaise(true);
	m_pSysQuitButton->setFocusPolicy(Qt::NoFocus);
	m_pSysQuitButton->setVisible(m_bShowQuitButton);
	m_pSysQuitButton->setDefaultAction(actQuit);
	m_pSysQuitButton->resize(m_nWinTitleButtonWidth + 8, m_nWinTitleButtonHeight);
	m_pSysQuitButton->setIconSize(QSize(m_nWinTitleButtonWidth + 8, m_nWinTitleButtonHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysQuitButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMaxButton = new QToolButton(this);
	m_pSysMaxButton->setMouseTracking(true);
	m_pSysMaxButton->setObjectName("win-max");
	m_pSysMaxButton->setAutoRaise(true);
	m_pSysMaxButton->setFocusPolicy(Qt::NoFocus);
	m_pSysMaxButton->setVisible(m_bShowMaxButton);
	m_pSysMaxButton->setDefaultAction(actMax);
	m_pSysMaxButton->resize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight);
	m_pSysMaxButton->setIconSize(QSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysMaxButton, Qt::AlignRight + Qt::AlignTop));

	m_pSysMinButton = new QToolButton(this);
	m_pSysMinButton->setMouseTracking(true);
	m_pSysMinButton->setObjectName("win-min");
	m_pSysMinButton->setAutoRaise(true);
	m_pSysMinButton->setFocusPolicy(Qt::NoFocus);
	m_pSysMinButton->setVisible(m_bShowMinButton);
	m_pSysMinButton->setDefaultAction(actMin);
	m_pSysMinButton->resize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight);
	m_pSysMinButton->setIconSize(QSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight));
	m_oTitleBarWidgets.append(TitleBarWidgetItem(m_pSysMinButton, Qt::AlignRight + Qt::AlignTop));

	//connect(gContext->styleMgr(), SIGNAL(onSystemStyleChanged()), 
	//		this, SLOT(doSystemStyleChanged()));
}

void CDialog::adjustPosition()
{
	// 获取父窗口
	QWidget *pParentWidget = nullptr; // kf::getTopParentWindow(this);
	if (nullptr == pParentWidget)
	{
		return;
	}
	// 全局的有效几何图形
	QRect oGlobalAvlGeometry = qApp->desktop()->availableGeometry();	
	// 获取父窗口在全局有效几何图形中显示的几何图形
	QRect oParentAvlGeometry = oGlobalAvlGeometry.intersected(pParentWidget->geometry());

	// 计算显示位置
	QPoint oPos = QPoint(oParentAvlGeometry.x() + (oParentAvlGeometry.width() - width()) / 2,
						 oParentAvlGeometry.y() + (oParentAvlGeometry.height() - height()) / 2);

	// 判断对话框是否在可显示区域内显示,如果不在则显示在屏幕中间
	QRect g = geometry();
	if (!oParentAvlGeometry.contains(g))
	{// 不在全局有效显示区域内则调整位置在全局有效显示区域居中显示
		oPos = QPoint((oGlobalAvlGeometry.width() - width()) / 2, (oGlobalAvlGeometry.height() - height()) / 2);
	}
	move(oPos);
}

/* 系统样式改变信号处理槽 */
void CDialog::doSystemStyleChanged()
{
	actQuit->setIcon(QIcon(WRS_Win_Button_Icon_Close));
	actMax->setIcon(QIcon(WRS_Win_Button_Icon_Max));
	actMin->setIcon(QIcon(WRS_Win_Button_Icon_Min));

	ensurePolished();
	update();
}

int CDialog::titleTextRightPos() const
{
	return m_nTitleTextRightPos;
}

int CDialog::titleTextMaxRightPos() const
{
	return m_nTitleTextMaxRightPos;
}

int CDialog::calcTitleBarLeftBeginPosition() const
{
	int nLeft = m_nActualBorderWidth + CWinTitleTextIndent;
	if (!windowIcon().isNull()) nLeft += m_oWinIconSize.width() + CWinTitleTextIndent;

	// 字体大小
	QFont ft = font();
	//ft.setBold(true);

	// 计算文字宽度
	QFontMetrics metr(ft);
	nLeft = metr.width(windowTitle()) + nLeft + CWinTitleTextIndent;
	return nLeft;
}

void CDialog::layoutTitleBarWidgets()
{
	// 靠左布局起始位置
	m_nTitleTextRightPos = calcTitleBarLeftBeginPosition();
	int nLeft = m_nTitleTextRightPos;
	// 靠右布局起始位置
	int nRight = width() - winBorderWidth();

	// 布局
	for (int i = 0; i < m_oTitleBarWidgets.size(); ++i)
	{
		TitleBarWidgetItem item = std::move(m_oTitleBarWidgets[i]);
		if (!item.widget || !item.widget->isVisible()) continue;

		// 顶边位置
		int nTop;	
		if (item.widgetAlign & Qt::AlignTop)
			nTop = winBorderWidth();
		else if (item.widgetAlign & Qt::AlignVCenter)
			nTop = (winTitleBarHeight() - item.widget->height()) / 2 + winBorderWidth();
		else if (item.widgetAlign & Qt::AlignBottom)
			nTop = (winTitleBarHeight() - item.widget->height());
		else
			nTop = (winTitleBarHeight() - item.widget->height()) / 2 + winBorderWidth();

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

void CDialog::doActionCloseTriggered()
{
	close();
}

void CDialog::doActionMaxTriggered()
{
	if (isMaximized())
	{
		showNormal();
		actMax->setIcon(QIcon(WRS_Win_Button_Icon_Max));
		actMax->setText(QStringLiteral("最大化"));
	}
	else
	{
		showMaximized();
		actMax->setIcon(QIcon(WRS_Win_Button_Icon_Restore));
		actMax->setText(QStringLiteral("还原"));
	}
}

void CDialog::doActionMinTriggered()
{
	showMinimized();
}

void CDialog::keyPressEvent(QKeyEvent *e)
{
	if (m_bForbidEscapeKeyClose && !e->modifiers() && e->key() == Qt::Key_Escape)
	{
		e->ignore();
		return;
	}

	QDialog::keyPressEvent(e);
}

void CDialog::mouseMoveEvent(QMouseEvent * evt)
{
	if (m_bMousePressedOnTitle)
	{// 移动窗口
		evt->accept();
		// 窗口最大化时,先还原,再移动
		if (isMaximized())
		{
			doActionMaxTriggered();
		}
		else
		{
			QPoint pos = mapToGlobal(rect().topLeft()) + evt->pos() - m_pDragPosition;
			if (m_bForbidTopBorderOutScreen && pos.y() < 0)
			{
				pos.setY(0);
			}
			move(pos);
		}
	}
	else if (m_bMousePressedOnBorder)
	{// 改变大小
		evt->accept();
		changeSize(evt);
	}
	else if (!isMaximized() && allowResize())
	{
		if (mouseOnWinBorder(evt->pos()))
		{// 改变光标形状
			evt->accept();
			setMouseResizeCursor(evt->pos());
		}
		else
		{// 还原光标形状
			setCursor(QCursor(Qt::ArrowCursor));
		}
	}
}

void CDialog::mousePressEvent(QMouseEvent * evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		if (mouseOnWinTitle(evt->pos()))
		{
			evt->accept();
			m_bMousePressedOnTitle = true;
			m_bMousePressedOnBorder = false;
			m_pDragPosition = evt->pos();
		}
        else if (allowResize() && mouseOnWinBorder(evt->pos()))
		{
			evt->accept();
			m_bMousePressedOnTitle = false;
			m_bMousePressedOnBorder = true;
			m_pDragPosition = evt->pos();
			m_pDragMoveOffset = rect().bottomRight() - m_pDragPosition;
		}
	}
}

void CDialog::mouseReleaseEvent(QMouseEvent * evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		m_bMousePressedOnTitle = false;
		m_bMousePressedOnBorder = false;
	}
}

void CDialog::mouseDoubleClickEvent(QMouseEvent * evt)
{
	if (!m_bMovingWin && showMaximumButton() && titleRect().contains(evt->pos()))
	{
		doActionMaxTriggered();
	};
}

void CDialog::paintEvent(QPaintEvent * evt)
{
	Q_UNUSED(evt);
	QPainter painter(this);
	drawWindowBorder(painter);
	drawSystemTitleBar(painter);
	drawSystemTitleText(painter, windowTitle());
}

void CDialog::showEvent(QShowEvent *evt)
{
	ensurePolished();
	adjustPosition();
	return QDialog::showEvent(evt);
}

bool CDialog::event(QEvent* evt)
{
	switch (evt->type()) {
	case QEvent::Resize:
	//case QEvent::StyleChange:
	//case QEvent::FontChange:
	case QEvent::WindowTitleChange:
	case QEvent::WindowStateChange:
		layoutTitleBarWidgets();
	}

	// 检查光标
	if (!m_bMousePressedOnBorder && m_eMousePos != mpCenter && evt->type() == QEvent::ToolTip)
	{
		m_eMousePos = mpCenter;
		setCursor(Qt::ArrowCursor);
		return QDialog::event(evt);
	}

	return QDialog::event(evt);
}

QRect CDialog::titleRect() const
{
	return QRect(m_nActualBorderWidth,
				m_nActualBorderWidth,
				width() - m_nActualBorderWidth * 2,
				m_nWinTitleBarHeight);
}

QRect CDialog::leftBorder() const
{
	return QRect(0, 0, m_nActualBorderWidth, height());
}

QRect CDialog::rightBorder() const
{
	return QRect(width() - m_nActualBorderWidth, 0, m_nActualBorderWidth, height());
}

QRect CDialog::topBorder() const
{
	return QRect(0, 0, width(), m_nActualBorderWidth);
}

QRect CDialog::bottomBorder() const
{
	return QRect(0, height() - m_nActualBorderWidth, width(), m_nActualBorderWidth);
}

bool CDialog::mouseOnWinTitle(const QPoint& pt)
{
	return titleRect().contains(pt);
}

bool CDialog::mouseOnWinBorder(const QPoint& pt)
{
	return (rightBorder().contains(pt)
		|| bottomBorder().contains(pt)
		|| leftBorder().contains(pt)
		|| topBorder().contains(pt));
}

void CDialog::setMouseResizeCursor(const QPoint& pt)
{
	if (rightBorder().contains(pt) && bottomBorder().contains(pt))
	{// 右下角
		m_eMousePos = mpRightBottom;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorder().contains(pt) && topBorder().contains(pt))
	{// 右上角
		m_eMousePos = mpRightTop;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorder().contains(pt) && bottomBorder().contains(pt))
	{// 左下角
		m_eMousePos = mpLeftBottom;
		setCursor(QCursor(Qt::SizeBDiagCursor));
	}
	else if (leftBorder().contains(pt) && topBorder().contains(pt))
	{// 左上角
		m_eMousePos = mpLeftTop;
		setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (rightBorder().contains(pt))
	{// 右边
		m_eMousePos = mpRight;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (bottomBorder().contains(pt))
	{// 底边
		m_eMousePos = mpBottom;
		setCursor(QCursor(Qt::SizeVerCursor));
	}
	else if (leftBorder().contains(pt))
	{// 左边
		m_eMousePos = mpLeft;
		setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (topBorder().contains(pt))
	{// 上边
		m_eMousePos = mpTop;
		setCursor(QCursor(Qt::SizeVerCursor));
	}
	else
	{
		m_eMousePos = mpCenter;
		setCursor(QCursor(Qt::ArrowCursor));
	}
}

void CDialog::changeSize(QMouseEvent* evt)
{
	QPoint p = evt->globalPos() + m_pDragMoveOffset; // 右下点的参考位置
	QPoint pp = evt->globalPos() - m_pDragPosition; // 左上点的参考位置

	QSize ms = calcMinSize(window()->sizeHint(), window()->minimumSizeHint(),
		window()->minimumSize(), window()->maximumSize(), window()->sizePolicy());
	int mw = ms.width();
	int mh = ms.height();

	QSize maxsize(window()->maximumSize());
	QSize mpsize(window()->geometry().right() - pp.x() + 1,
		window()->geometry().bottom() - pp.y() + 1);
	mpsize = mpsize.expandedTo(window()->minimumSize()).expandedTo(QSize(mw, mh));
	QPoint mp(window()->geometry().right() - mpsize.width() + 1,
		window()->geometry().bottom() - mpsize.height() + 1);

	QRect geom = window()->geometry();
	switch (m_eMousePos) {
	case mpLeftTop:
		geom = QRect(mp, window()->geometry().bottomRight());
		break;
	case mpRightBottom:
		geom = QRect(window()->geometry().topLeft(), p);
		break;
	case mpLeftBottom:
		geom = QRect(QPoint(mp.x(), window()->geometry().y()), QPoint(window()->geometry().right(), p.y()));
		break;
	case mpRightTop:
		geom = QRect(QPoint(window()->geometry().x(), mp.y()), QPoint(p.x(), window()->geometry().bottom()));
		break;
	case mpTop:
		geom = QRect(QPoint(window()->geometry().left(), mp.y()), window()->geometry().bottomRight());
		break;
	case mpBottom:
		geom = QRect(window()->geometry().topLeft(), QPoint(window()->geometry().right(), p.y()));
		break;
	case mpLeft:
		geom = QRect(QPoint(mp.x(), window()->geometry().top()), window()->geometry().bottomRight());
		break;
	case mpRight:
		geom = QRect(window()->geometry().topLeft(), QPoint(p.x(), window()->geometry().bottom()));
		break;
	case mpCenter:
		geom.moveTopLeft(pp);
		break;
	default:
		break;
	}

	geom = QRect(geom.topLeft(),
				 geom.size().expandedTo(window()->minimumSize()).expandedTo(QSize(mw, mh)).boundedTo(maxsize));

	if (geom != window()->geometry())
	{
		window()->setGeometry(geom);
	}
}

/* 计算改变的最小宽和高 */
QSize CDialog::calcMinSize(const QSize &sizeHint, const QSize &minSizeHint,
	const QSize &minSize, const QSize &maxSize,
	const QSizePolicy &sizePolicy)
{
	QSize s(0, 0);
	if (sizePolicy.horizontalPolicy() != QSizePolicy::Ignored)
	{
		if (sizePolicy.horizontalPolicy() & QSizePolicy::ShrinkFlag)
			s.setWidth(minSizeHint.width());
		else
			s.setWidth(qMax(sizeHint.width(), minSizeHint.width()));
	}

	if (sizePolicy.verticalPolicy() != QSizePolicy::Ignored)
	{
		if (sizePolicy.verticalPolicy() & QSizePolicy::ShrinkFlag)
			s.setHeight(minSizeHint.height());
		else
			s.setHeight(qMax(sizeHint.height(), minSizeHint.height()));
	}

	s = s.boundedTo(maxSize);
	if (minSize.width() > 0)   s.setWidth(minSize.width());
	if (minSize.height() > 0)  s.setHeight(minSize.height());

	return s.expandedTo(QSize(0, 0));
}

void CDialog::drawWindowBorder(QPainter& painter)
{
	int nOffsetWidth = m_nWinBorderWidth + m_nWinBorderWidth % 2 + m_nWinShadowWidth * 2;
	QRect rtBorder = QRect(qCeil(m_nWinBorderWidth / 2.0) + m_nWinShadowWidth,
							qCeil(m_nWinBorderWidth / 2.0) + m_nWinShadowWidth,
							width() - nOffsetWidth,	height() - nOffsetWidth);

	// 绘制边框和背景
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (m_nWinBorderWidth > 0)
	{
		QPen pen = QPen(isActiveWindow() ? winBorderActiveColor() : winBorderColor());
		pen.setWidth(m_nWinBorderWidth);
		painter.setPen(pen);
		painter.setBrush(palette().background());
		painter.drawRect(rtBorder);
	}
	else
	{
		painter.fillRect(rtBorder, palette().background());
	}

	// 绘制阴影
	QColor color = m_cWinShadowColor;
	for (int i = 0; i < m_nWinShadowWidth; ++i)
	{
		int nOffset = m_nWinShadowWidth - i;
		QPolygon polygon;
		polygon << QPoint(nOffset, nOffset)
				<< QPoint(width() - nOffset, m_nWinShadowWidth - i)
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

void CDialog::drawSystemTitleBar(QPainter& painter)
{
	if (isActiveWindow())
	{
		painter.drawPixmap(titleRect(), QPixmap(WRS_Win_Title_Background_Active)); 
	}
	else
	{
		painter.drawPixmap(titleRect(), QPixmap(WRS_Win_Title_Background));
	}

	if (!windowIcon().isNull())
	{
		int nOffsetX = (winTitleBarHeight() - m_oWinIconSize.width()) / 2 + winBorderWidth();
		int nOffsetY = (winTitleBarHeight() - m_oWinIconSize.height()) / 2 + winBorderWidth();
		QRect rt = QRect(nOffsetX, nOffsetY, m_oWinIconSize.width(), m_oWinIconSize.height());
		windowIcon().paint(&painter, rt);
	}
}

void CDialog::drawSystemTitleText(QPainter& painter, const QString& sTitle)
{
	int nLeft = m_nActualBorderWidth + CWinTitleTextIndent;
	if (!windowIcon().isNull()) nLeft += m_oWinIconSize.width() + CWinTitleTextIndent;
	QRect rt = QRect(nLeft, m_nActualBorderWidth, m_nTitleTextMaxRightPos - nLeft, m_nWinTitleBarHeight);
	QTextOption to(Qt::AlignLeft | Qt::AlignVCenter);
	to.setWrapMode(QTextOption::NoWrap);

	// 设置字体大小
	//QFont ft = font();
	//ft.setBold(true);
	
	// 绘制文字
	//painter.setFont(ft);
	painter.setPen(winTitleFontColor());
	painter.drawText(rt, sTitle, to);
}

bool CDialog::forbidEscapeKeyClose() const
{
	return m_bForbidEscapeKeyClose;
}

void CDialog::setForbidEscapeKeyClose(bool value)
{
	m_bForbidEscapeKeyClose = value;
}

bool CDialog::allowResize() const
{
	return m_bAllowResize;
}

void CDialog::setAllowResize(bool value)
{
	if (m_bAllowResize != value)
	{
		m_bAllowResize = value;
	}
}

bool CDialog::forbidTopBorderOutScreen() const
{
	return m_bForbidTopBorderOutScreen;
}

void CDialog::setForbidTopBorderOutScreen(bool value)
{
	m_bForbidTopBorderOutScreen = value;
}

bool CDialog::showMinimumButton() const
{
	return m_bShowMinButton;
}

void CDialog::setShowMinimumButton(bool value)
{
	if (m_bShowMinButton != value)
	{
		m_bShowMinButton = value;
		m_pSysMinButton->setVisible(m_bShowMinButton);
		actMin->setVisible(m_bShowMinButton);
		layoutTitleBarWidgets();
	}
}

bool CDialog::showMaximumButton() const
{
	return m_bShowMaxButton;
}

void CDialog::setShowMaximumButton(bool value)
{
	if (m_bShowMaxButton != value)
	{
		m_bShowMaxButton = value;
		m_pSysMaxButton->setVisible(m_bShowMaxButton);
		actMax->setVisible(m_bShowMaxButton);
		layoutTitleBarWidgets();
	}
}

bool CDialog::showCloseButton() const
{
	return m_bShowQuitButton;
}

void CDialog::setShowCloseButton(bool value)
{
	if (m_bShowQuitButton != value)
	{
		m_bShowQuitButton = value;
		m_pSysQuitButton->setVisible(m_bShowQuitButton);
		actQuit->setVisible(m_bShowQuitButton);
		layoutTitleBarWidgets();
	}
}

void CDialog::addTitleBarWidget(QWidget* pWidget, int nAlign, int nSpacing, bool bAutoFillRest)
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

int CDialog::winTitleBarHeight() const
{
	return m_nWinTitleBarHeight;
}

void CDialog::setWinTitleBarHeight(int value)
{
	if (m_nWinTitleBarHeight != value && value > 16)
	{
		m_nWinTitleBarHeight = value;
		updateContentsMargins();
		if (m_nWinTitleBarHeight < winTitleBarButtonHeight())
		{
			setWinTitleBarButtonHeight(m_nWinTitleBarHeight);
		}
	}
}

int CDialog::winTitleBarButtonHeight() const
{
	return m_nWinTitleButtonHeight;
}

void CDialog::setWinTitleBarButtonHeight(int value)
{
	if (m_nWinTitleButtonHeight != value && value > 16)
	{
		if (value > winTitleBarHeight())
			m_nWinTitleButtonHeight = winTitleBarHeight();
		else
			m_nWinTitleButtonHeight = value;

		adjustSysButtonSize();
	}
}

int CDialog::winTitleBarButtonWidth() const
{
	return m_nWinTitleButtonWidth;
}

void CDialog::setWinTitleBarButtonWidth(int value)
{
	if (m_nWinTitleButtonWidth != value && value > 16)
	{
		m_nWinTitleButtonWidth = value;
		adjustSysButtonSize();
	}
}

int CDialog::winBorderWidth() const
{
	return m_nActualBorderWidth;
}

void CDialog::setWinBorderWidth(int value)
{
	if (m_nWinBorderWidth != value && value >= 0)
	{
		m_nWinBorderWidth = value;
		m_nActualBorderWidth = m_nWinBorderWidth + m_nWinShadowWidth;
		updateContentsMargins();
		layoutTitleBarWidgets();
	}
}

QColor CDialog::winBorderColor() const
{
	return m_cWinBorderColor;
}

void CDialog::setWinBorderColor(QColor color)
{
	m_cWinBorderColor = color;
}

QColor CDialog::winBorderActiveColor() const
{
	return m_cWinBorderActiveColor;
}

void CDialog::setWinBorderActiveColor(QColor color)
{
	m_cWinBorderActiveColor = color;
}

int CDialog::winShadowWidth() const
{
	return m_nWinShadowWidth;
}

void CDialog::setWinShadowWidth(int value)
{
	if (m_nWinShadowWidth != value && value >= 0)
	{
		m_nWinShadowWidth = value;
		m_nActualBorderWidth = m_nWinBorderWidth + m_nWinShadowWidth;
		setAttribute(Qt::WA_TranslucentBackground, m_nWinShadowWidth);
		layoutTitleBarWidgets();
	}
}

QColor CDialog::winShadowColor() const
{
	return m_cWinShadowColor;
}

void CDialog::setWinShadowColor(QColor color)
{
	m_cWinShadowColor = color;
}

QColor CDialog::winTitleFontColor() const
{
	return m_cWinTitleFontColor;
}

void CDialog::setWinTitleFontColor(QColor color)
{
	m_cWinTitleFontColor = color;
}

CFocusSwitchHelper<CDialog>* CDialog::iFSHelper()
{
	return m_pFocusSwitchHelper;
}

QSize CDialog::sizeHint() const
{
	return minimumSizeHint();
}

QSize CDialog::minimumSizeHint() const
{
	// 确认应用样式
	if (isVisible()) ensurePolished();

	// Calculate window decoration.
	int nMargin, nMinWidth;
	sizeParameters(&nMargin, &nMinWidth);
	int nMinHeight = winTitleBarHeight() + nMargin * 2;

	// Minimized window.
	if (isMinimized())
		return QSize(nMinWidth, nMinHeight);

	// Content
	if (layout())
	{
		QSize minLayoutSize = layout()->minimumSize();
		if (minLayoutSize.isValid())
		{
			nMinWidth = qMax(nMinWidth, minLayoutSize.width() + 2 * nMargin);
			nMinHeight += minLayoutSize.height();
		}
	}
	
	return QSize(nMinWidth, nMinHeight).expandedTo(QApplication::globalStrut());
}

/* 计算窗口边的宽度和窗口最小宽度 */
void CDialog::sizeParameters(int * nMargin, int * nMinWidth) const
{
	*nMargin = winBorderWidth();
	
	int nW = titleTextRightPos() + winBorderWidth();
	for (int i = 0; i < m_oTitleBarWidgets.size(); ++i)
	{
		if (!m_oTitleBarWidgets[i].autoFillRest)
		{
			nW += m_oTitleBarWidgets[i].widget->width();
		}
	}
	*nMinWidth = nW;
}

void CDialog::updateContentsMargins()
{
	setContentsMargins(m_nActualBorderWidth,
						m_nActualBorderWidth + m_nWinTitleBarHeight,
						m_nActualBorderWidth,
						m_nActualBorderWidth);
}

void CDialog::adjustSysButtonSize()
{
	m_pSysQuitButton->setFixedSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight);
	m_pSysQuitButton->setIconSize(QSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight));

	m_pSysMaxButton->setFixedSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight);
	m_pSysMaxButton->setIconSize(QSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight));

	m_pSysMinButton->setFixedSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight);
	m_pSysMinButton->setIconSize(QSize(m_nWinTitleButtonWidth, m_nWinTitleButtonHeight));

	layoutTitleBarWidgets();
}
