#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include <QDialog>

/* ���ڱ���������� */
struct TitleBarWidgetItem
{
	TitleBarWidgetItem(QWidget* pWidget, int nAlign = Qt::AlignRight,
		int nSpacing = 0, bool bAutoFillRest = false)
		: widget(pWidget), widgetAlign(nAlign), spacing(nSpacing), autoFillRest(bAutoFillRest)
	{}

	QWidget* widget;
	int		widgetAlign;
	int		spacing = 0;
	bool	autoFillRest;	// �Ƿ��Զ�ռ��ʣ��ռ�,ע��:ֻ����һ���������Ϊtrue
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

	// [��д]��С��ʾ
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

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
	virtual void mouseDoubleClickEvent(QMouseEvent * event);

	// ���ֱ���������
	virtual void layoutTitleBarWidgets();

protected slots:
	virtual void doActionCloseTriggered();
	virtual void doActionMaxTriggered();
	virtual void doActionMinTriggered();

private:
	// ����UI
	void setupUi();

	// �ж�����Ƿ����ڱ�������
	bool isMouseOnTitleBar(const QPoint &pos);

	// �ж�����Ƿ����ڱ߿���
	bool isMouseOnBorder(const QPoint &pos);

	// ���⣬���ϡ��ҡ��� �߿�ľ�������
	QRect titleRect() const;
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
	int m_nTitleBtnHeight	= 29;			// ��������ť���
	int m_nTitleBtnWidth	= 35;			// ��������ť�߶�(�رհ�ť+8)

	bool m_bResizable		= true;			// �Ƿ������
	bool m_bMinBtnVisible	= true;			// �Ƿ���ʾ��С����ť
	bool m_bMaxBtnVisible	= true;			// �Ƿ���ʾ��󻯰�ť
	bool m_bCloseBtnVisible	= true;			// �Ƿ���ʾ�رհ�ť

	bool m_bMouseOnTitleBar	= false;		// ����Ƿ����ڱ�������
	bool m_bMouseOnBorder	= false;		// ����Ƿ����ڱ߿���

	QPoint m_pMousePressPos;				// ��갴��ʱ�����λ��
	QPoint m_pPressDragGlobalPos;			// �����קʱ����ƶ��¼��ĵ����Ļλ��
	enum EMousePosition
	{
		MP_Center, MP_Left, MP_TopLeft, MP_Top, MP_TopRight, MP_Right,
		MP_BottomRight, MP_Bottom, MP_BottomLeft
	} m_eMousePos = MP_Center;				// ����ڰ���ʱ������λ��

	QAction *m_pActMin;						// ��С������
	QAction *m_pActMax;						// ��󻯶���
	QAction *m_pActClose;					// �رն���

	QToolButton* m_pSysMinButton;			// ��С����ť
	QToolButton* m_pSysMaxButton;			// ��󻯰�ť
	QToolButton* m_pSysCloseButton;			// �رհ�ť

	TitleBarWidgetList m_oTitleBarWidgets;	// �������Ĳ����б�
};

#endif // BASICDIALOG_H
