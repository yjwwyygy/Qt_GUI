#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include <QDialog>

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
class BasicDialog : public QDialog
{
	Q_OBJECT

public:
	BasicDialog(QWidget *parent = 0, int nShadowWidth = 0, int nBorderWidth = 3);
	~BasicDialog();

	// [重写]大小提示
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	// 可缩放
	bool isResizable();
	void setResizable(bool value);

	// 最小/最大/关闭按钮状态
	bool isMinimumButtonVisible();
	void setMinimumButtonVisible(bool value);
	bool isMaximumButtonVisible();
	void setMaximumButtonVisible(bool value);
	bool isCloseButtonVisible();
	void setCloseButtonVisible(bool value);


protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent * event);

	// 布局标题栏部件
	virtual void layoutTitleBarWidgets();

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

	// 标题，左、上、右、下 边框的矩形区域
	QRect titleRect() const;
	QRect leftBorderRect() const;
	QRect topBorderRect() const;
	QRect rightBorderRect() const;
	QRect bottomBorderRect() const;

	// 设置光标
	void setMouseResizeCursor(const QPoint &pos);

	// 改变大小
	void changeSize(QMouseEvent *event);

private:
	int m_nShadowWidth;						// 边框阴影的宽度
	int m_nBorderWidth;						// 边框的宽度
	int m_nTitleBarHeight	= 33;			// 标题栏的高度
	int m_nTitleBtnHeight	= 29;			// 标题栏按钮宽度
	int m_nTitleBtnWidth	= 35;			// 标题栏按钮高度(关闭按钮+8)

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
};

#endif // BASICDIALOG_H
