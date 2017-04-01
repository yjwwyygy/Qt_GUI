#ifndef CDIALOG_H
#define CDIALOG_H

#include "macro.h"
#include "wintools.h"
#include <QRect>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QToolButton>
#include <QDialog>
#include "cfocusswitchhelper.h"

/*
	CDialog
*/
class CDialog : public QDialog
{
    Q_OBJECT
	friend class CFocusSwitchHelper<CDialog>;
public:
	explicit CDialog(QWidget *parent = nullptr, int nShadowWidth = 0, int nBorderWidth = 2);
	~CDialog();

	// [重写]定义窗体的大小提示
	QSize sizeHint() const;
	QSize minimumSizeHint() const;

	// 禁止ESC键关闭窗体
	bool forbidEscapeKeyClose() const;
	void setForbidEscapeKeyClose(bool value);

	// 可缩放
	bool allowResize() const;
	void setAllowResize(bool value);

	// 窗口顶部不超出屏幕
	bool forbidTopBorderOutScreen() const;
	void setForbidTopBorderOutScreen(bool value);

	// 最小化/最大化/关闭按钮
	bool showMinimumButton() const;
	void setShowMinimumButton(bool value);
	bool showMaximumButton() const;
	void setShowMaximumButton(bool value);
	bool showCloseButton() const;
	void setShowCloseButton(bool value);

	// 添加标题栏
	void addTitleBarWidget(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false);

	/* 输入组件焦点切换辅助类 */
	CFocusSwitchHelper<CDialog>* iFSHelper();

public slots:
	/* 系统样式改变信号处理槽 */
	void doSystemStyleChanged();

protected:
	inline int winTitleBarHeight() const;
	inline int winTitleBarButtonHeight() const;
	inline int winTitleBarButtonWidth() const;
	inline int winBorderWidth() const;
	//inline int winCornerWidth() const;
	inline int winShadowWidth() const;
	inline QColor winBorderColor() const;
	inline QColor winBorderActiveColor() const;
	inline QColor winShadowColor() const;
	inline QColor winTitleFontColor() const;

	void setWinTitleBarHeight(int value);
	void setWinTitleBarButtonHeight(int value);
	void setWinTitleBarButtonWidth(int value);
	void setWinBorderWidth(int value);
	void setWinBorderColor(QColor color);
	void setWinBorderActiveColor(QColor color);
	void setWinShadowWidth(int value);
	void setWinShadowColor(QColor color);
	void setWinTitleFontColor(QColor color);
	
	virtual void keyPressEvent(QKeyEvent * evt);
	virtual void mouseMoveEvent(QMouseEvent * evt);
	virtual void mousePressEvent(QMouseEvent * evt);
	virtual void mouseReleaseEvent(QMouseEvent * evt);
	virtual void mouseDoubleClickEvent(QMouseEvent * evt);
	virtual void paintEvent(QPaintEvent * evt);
	virtual void showEvent(QShowEvent *evt);
	virtual bool event(QEvent* evt);

	inline bool mouseOnWinTitle(const QPoint& pt);
	inline bool mouseOnWinBorder(const QPoint& pt);
	inline void setMouseResizeCursor(const QPoint& pt);
	inline void changeSize(QMouseEvent* evt);
	inline QSize calcMinSize(const QSize &sizeHint, const QSize &minSizeHint,
		const QSize &minSize, const QSize &maxSize,
		const QSizePolicy &sizePolicy);

	inline QRect titleRect() const;
	inline QRect leftBorder() const;
	inline QRect rightBorder() const;
	inline QRect topBorder() const;
	inline QRect bottomBorder() const;

	inline void updateContentsMargins();
	inline void adjustSysButtonSize(); 
	inline void sizeParameters(int * nMargin, int * nMinWidth) const;

	virtual void drawWindowBorder(QPainter& painter);
	virtual void drawSystemTitleText(QPainter& painter, const QString& sTitle);
	virtual void drawSystemTitleBar(QPainter& painter);

	inline int titleTextRightPos() const;
	inline int titleTextMaxRightPos() const;
	virtual int calcTitleBarLeftBeginPosition() const;
	virtual void layoutTitleBarWidgets();

	QToolButton* m_pSysMinButton;
	QToolButton* m_pSysMaxButton;
	QToolButton* m_pSysQuitButton;

protected slots :
	virtual void doActionCloseTriggered();
	virtual void doActionMaxTriggered();
	virtual void doActionMinTriggered();

private:
    inline void setupUi();
	inline void adjustPosition();
    
private:
	QAction* actQuit;
	QAction* actMax;
	QAction* actMin;

	int m_nWinShadowWidth		= 0;		// 阴影宽度
	int m_nWinBorderWidth		= 2;		// 窗口边的宽度
	int m_nActualBorderWidth	= 2;		// 实际窗口边的宽度
	int m_nWinTitleBarHeight	= 29;		// 标题栏高度
	int m_nWinTitleButtonHeight = 29;		// 标题栏按钮宽度
	int m_nWinTitleButtonWidth	= 35;		// 标题栏按钮高度(关闭按钮+8)
	
	int m_nTitleTextRightPos	= 0;		// 标题文字的实际右边坐标
	int m_nTitleTextMaxRightPos = 0;		// 标题文字的最右边坐标
	
	QColor m_cWinTitleFontColor = QColor(255, 255, 255);
	QColor m_cWinBorderColor = QColor(29, 102, 183);
	QColor m_cWinBorderActiveColor = QColor(12, 145, 226);
	QColor m_cWinShadowColor = QColor(0, 0, 0);
	QSize m_oWinIconSize = QSize(16, 16);

	bool m_bAllowResize = true;
	bool m_bForbidTopBorderOutScreen = true;

	bool m_bShowMinButton = true;
	bool m_bShowMaxButton = true;
	bool m_bShowQuitButton = true;

	bool m_bMovingWin = false;
	bool m_bMousePressedOnTitle = false;
	bool m_bMousePressedOnBorder = false;
    QPoint m_pDragPosition;					// 在窗口边上鼠标按下时的屏幕坐标位置
    QPoint m_pDragMoveOffset;				// 记录鼠标按下时与窗口右下角的偏移坐标
	QPoint m_ptRightBottom;

	enum MousePosition
	{
		mpCenter, mpLeft, mpTop, mpRight, mpBottom,
		mpLeftTop, mpLeftBottom, mpRightTop, mpRightBottom
	} m_eMousePos = { mpCenter };

	bool m_bForbidEscapeKeyClose = false;		// 是否忽略EscapeKey,不忽略则按Esc关闭窗口

	// 存放手动添加的标题栏组件
	TitleBarWidgetList m_oTitleBarWidgets;
	// 输入组件焦点切换辅助类
	CFocusSwitchHelper<CDialog>* m_pFocusSwitchHelper = nullptr;
};

#endif // CDIALOG_H
