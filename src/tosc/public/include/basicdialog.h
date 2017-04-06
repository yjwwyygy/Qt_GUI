#ifndef BASICDIALOG_H
#define BASICDIALOG_H

#include "frame_global.h"
#include <QDialog>

class QToolButton;

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
class FRAME_EXPORT BasicDialog : public QDialog
{
	Q_OBJECT

public:
	BasicDialog(QWidget *parent = 0, int nShadowWidth = 0, int nBorderWidth = 3);
	~BasicDialog();

	// [��д]��С��ʾ
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	// ��ֹESC���رմ���
	bool forbidEscapeKeyClose() const;
	void setForbidEscapeKeyClose(bool value);

	// ͼ��/��������
	bool isIconVisible() const;
	void setIconVisible(bool value);
	bool isTitleVisible() const;
	void setTitleVisible(bool value);

	// ������
	bool isResizable() const;
	void setResizable(bool value);

	// ��С/���/�رհ�ť״̬
	bool isMinimumButtonVisible() const;
	void setMinimumButtonVisible(bool value);
	bool isMaximumButtonVisible() const;
	void setMaximumButtonVisible(bool value);
	bool isCloseButtonVisible() const;
	void setCloseButtonVisible(bool value);

	// ��ӱ�����
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

	// ���������������ʼλ��
	virtual int calcTitleBarLeftBeginPosition() const;

	// ���ֱ���������
	virtual void layoutTitleBarWidgets();

	// ���⣬���ϡ��ҡ��� �߿�ľ�������
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
	// ����UI
	void setupUi();

	// �ж�����Ƿ����ڱ�������
	bool isMouseOnTitleBar(const QPoint &pos);

	// �ж�����Ƿ����ڱ߿���
	bool isMouseOnBorder(const QPoint &pos);

	// ���ù��
	void setMouseResizeCursor(const QPoint &pos);

	// �ı��С
	void changeSize(QMouseEvent *event);

private:
	int m_nShadowWidth;						// �߿���Ӱ�Ŀ��
	int m_nBorderWidth;						// �߿�Ŀ��
	int m_nTitleBarHeight	= 35;			// �������ĸ߶�
	int m_nTitleBtnHeight	= 29;			// ��������ť�߶�
	int m_nTitleBtnWidth	= 35;			// ��������ť���(�رհ�ť+8)

	bool m_bIconVisible		= true;			// �Ƿ���ʾIcon
	bool m_bTitleVisible	= true;			// �Ƿ���ʾ��������
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

	QColor m_cTitleFontColor = QColor(255, 255, 255);
	QColor m_cBorderColor = QColor(29, 102, 183);
	QColor m_cBorderActiveColor = QColor(12, 145, 226);
	QColor m_cShadowColor = QColor(0, 0, 0);
	QSize m_oIconSize = QSize(16, 16);

	int m_nTitleTextRightPos = 0;			// �������ֵ�ʵ���ұ�����
	int m_nTitleTextMaxRightPos = 0;		// �������ֵ����ұ�����

	bool m_bForbidEscapeKeyClose = false;		// �Ƿ����EscapeKey,��������Esc�رմ���
};

#endif // BASICDIALOG_H
