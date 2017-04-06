#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include "frame_global.h"
#include <QDialog>

class QToolButton;

/* 窗口标题栏组件项 */
struct TitleBarWidgetItem
{
	TitleBarWidgetItem(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false)
		: widget(pWidget), widgetAlign(nAlign), spacing(nSpacing), autoFillRest(bAutoFillRest)
	{}

	QWidget* widget;
	int		widgetAlign;
	int		spacing = 0;
	bool	autoFillRest;	// 是否自动占满剩余空间,注意:只能有一个组件设置为true
};
typedef QList<TitleBarWidgetItem> TitleBarWidgetList;

/*
	BasicDialog
*/
class FRAME_EXPORT BasicDialog : public QDialog
{
	Q_OBJECT

public:
	BasicDialog(QWidget *parent = 0, int nShadowWidth = 0, int nBorderWidth = 3);
	~BasicDialog();

	// [重写]大小提示
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	// 禁止ESC键关闭窗体
	bool forbidEscapeKeyClose() const;
	void setForbidEscapeKeyClose(bool value);

	// 图标/标题文字
	bool isIconVisible() const;
	void setIconVisible(bool value);
	bool isTitleVisible() const;
	void setTitleVisible(bool value);

	// 可缩放
	bool isResizable() const;
	void setResizable(bool value);

	// 最小/最大/关闭按钮状态
	bool isMinimumButtonVisible() const;
	void setMinimumButtonVisible(bool value);
	bool isMaximumButtonVisible() const;
	void setMaximumButtonVisible(bool value);
	bool isCloseButtonVisible() const;
	void setCloseButtonVisible(bool value);

	// 添加标题栏
	void addTitleBarWidget(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);
	virtual void showEvent(QShowEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual bool event(QEvent* event);
	virtual void keyPressEvent(QKeyEvent * event);

	virtual void drawBorder(QPainter& painter);
	virtual void drawTitleBar(QPainter& painter);
	virtual void drawTitleText(QPainter& painter, const QString& sTitle);

	int titleTextRightPos() const;
	int titleTextMaxRightPos() const;

	// 计算标题栏的左起始位置
	virtual int calcTitleBarLeftBeginPosition() const;

	// 布局标题栏部件
	virtual void layoutTitleBarWidgets();

	// 标题，左、上、右、下 边框的矩形区域
	QRect titleRect() const;
	QRect leftBorderRect() const;
	QRect topBorderRect() const;
	QRect rightBorderRect() const;
	QRect bottomBorderRect() const;

	int borderWidth() const;
	int titleBarHeight() const;
	inline QColor borderColor() const;
	inline QColor borderActiveColor() const;
	inline QColor shadowColor() const;
	inline QColor titleFontColor() const;

protected slots:
	virtual void doActionCloseTriggered();
	virtual void doActionMaxTriggered();
	virtual void doActionMinTriggered();

private:
	// 设置UI
	void setupUi();

	// 判断鼠标是否点击在标题栏上
	bool isMouseOnTitleBar(const QPoint &pos);

	// 判断鼠标是否点击在边框上
	bool isMouseOnBorder(const QPoint &pos);

	// 设置光标
	void setMouseResizeCursor(const QPoint &pos);

	// 改变大小
	void changeSize(QMouseEvent *event);

private:
	int m_nShadowWidth;						// 边框阴影的宽度
	int m_nBorderWidth;						// 边框的宽度
	int m_nTitleBarHeight	= 35;			// 标题栏的高度
	int m_nTitleBtnHeight	= 29;			// 标题栏按钮高度
	int m_nTitleBtnWidth	= 35;			// 标题栏按钮宽度(关闭按钮+8)

	bool m_bIconVisible		= true;			// 是否显示Icon
	bool m_bTitleVisible	= true;			// 是否显示标题文字
	bool m_bResizable		= true;			// 是否可缩放
	bool m_bMinBtnVisible	= true;			// 是否显示最小化按钮
	bool m_bMaxBtnVisible	= true;			// 是否显示最大化按钮
	bool m_bCloseBtnVisible	= true;			// 是否显示关闭按钮

	bool m_bMouseOnTitleBar	= false;		// 鼠标是否点击在标题栏中
	bool m_bMouseOnBorder	= false;		// 鼠标是否点击在边框上

	QPoint m_pMousePressPos;				// 鼠标按下时的相对位置
	QPoint m_pPressDragGlobalPos;			// 鼠标拖拽时鼠标移动事件的点的屏幕位置
	enum EMousePosition
	{
		MP_Center, MP_Left, MP_TopLeft, MP_Top, MP_TopRight, MP_Right,
		MP_BottomRight, MP_Bottom, MP_BottomLeft
	} m_eMousePos = MP_Center;				// 鼠标在按下时所处的位置

	QAction *m_pActMin;						// 最小化动作
	QAction *m_pActMax;						// 最大化动作
	QAction *m_pActClose;					// 关闭动作

	QToolButton* m_pSysMinButton;			// 最小化按钮
	QToolButton* m_pSysMaxButton;			// 最大化按钮
	QToolButton* m_pSysCloseButton;			// 关闭按钮

	TitleBarWidgetList m_oTitleBarWidgets;	// 标题栏的部件列表

	QColor m_cTitleFontColor = QColor(255, 255, 255);
	QColor m_cBorderColor = QColor(29, 102, 183);
	QColor m_cBorderActiveColor = QColor(12, 145, 226);
	QColor m_cShadowColor = QColor(0, 0, 0);
	QSize m_oIconSize = QSize(16, 16);

	int m_nTitleTextRightPos = 0;			// 标题文字的实际右边坐标
	int m_nTitleTextMaxRightPos = 0;		// 标题文字的最右边坐标

	bool m_bForbidEscapeKeyClose = false;		// 是否忽略EscapeKey,不忽略则按Esc关闭窗口
};

#endif // BASICDIALOG_H
