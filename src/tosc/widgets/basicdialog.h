#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include <QDialog>

class BasicDialog : public QDialog
{
	Q_OBJECT

public:
	BasicDialog(QWidget *parent = 0, int nShadowWidth = 0, int nBorderWidth = 3);
	~BasicDialog();

	// [重写]大小提示
	virtual QSize sizeHint() const;

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


private:
	void setupUi();

	// 判断鼠标是否点击在标题栏上
	bool isMouseOnTitleBar(const QPoint &pos);

	// 判断鼠标是否点击在边框上
	bool isMouseOnBorder(const QPoint &pos);

	// 左、上、右、下 边框的矩形区域
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

	bool m_bResizable		= true;			// 是否可缩放
	bool m_bMinBtnVisible	= false;		// 是否显示最小化按钮
	bool m_bMaxBtnVisible	= false;		// 是否显示最大化按钮
	bool m_bCloseBtnVisible	= false;		// 是否显示关闭按钮

	bool m_bMouseOnTitleBar	= false;		// 鼠标是否点击在标题栏中
	bool m_bMouseOnBorder	= false;		// 鼠标是否点击在边框上

	QPoint m_pMousePressPos;				// 鼠标按下时的位置
	enum EMousePosition
	{
		MP_Center, MP_Left, MP_TopLeft, MP_Top, MP_TopRight, MP_Right,
		MP_BottomRight, MP_Bottom, MP_BottomLeft
	} m_eMousePos = MP_Center;
};

#endif // BASICDIALOG_H
