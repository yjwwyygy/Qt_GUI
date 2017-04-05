#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include <QDialog>

class BasicDialog : public QDialog
{
	Q_OBJECT

public:
	BasicDialog(QWidget *parent = 0, int nShadowWidth = 0, int nBorderWidth = 3);
	~BasicDialog();

	// [��д]��С��ʾ
	virtual QSize sizeHint() const;

	// ������
	bool isResizable();
	void setResizable(bool value);

	// ��С/���/�رհ�ť״̬
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

	// �ж�����Ƿ����ڱ�������
	bool isMouseOnTitleBar(const QPoint &pos);

	// �ж�����Ƿ����ڱ߿���
	bool isMouseOnBorder(const QPoint &pos);

	// ���ϡ��ҡ��� �߿�ľ�������
	QRect leftBorderRect() const;
	QRect topBorderRect() const;
	QRect rightBorderRect() const;
	QRect bottomBorderRect() const;

	// ���ù��
	void setMouseResizeCursor(const QPoint &pos);

	// �ı��С
	void changeSize(QMouseEvent *event);

private:
	int m_nShadowWidth;						// �߿���Ӱ�Ŀ��
	int m_nBorderWidth;						// �߿�Ŀ��
	int m_nTitleBarHeight	= 33;			// �������ĸ߶�

	bool m_bResizable		= true;			// �Ƿ������
	bool m_bMinBtnVisible	= false;		// �Ƿ���ʾ��С����ť
	bool m_bMaxBtnVisible	= false;		// �Ƿ���ʾ��󻯰�ť
	bool m_bCloseBtnVisible	= false;		// �Ƿ���ʾ�رհ�ť

	bool m_bMouseOnTitleBar	= false;		// ����Ƿ����ڱ�������
	bool m_bMouseOnBorder	= false;		// ����Ƿ����ڱ߿���

	QPoint m_pMousePressPos;				// ��갴��ʱ��λ��
	enum EMousePosition
	{
		MP_Center, MP_Left, MP_TopLeft, MP_Top, MP_TopRight, MP_Right,
		MP_BottomRight, MP_Bottom, MP_BottomLeft
	} m_eMousePos = MP_Center;
};

#endif // BASICDIALOG_H
