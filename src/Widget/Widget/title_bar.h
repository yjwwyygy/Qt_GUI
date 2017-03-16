#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QPushButton>

class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = NULL);
	~TitleBar();

	// ˫�����������н�������/��ԭ
	virtual void mouseDoubleClickEvent(QMouseEvent *event);

	// �����������϶�
	//virtual void mousePressEvent(QMouseEvent *event);
	//virtual void mouseMoveEvent(QMouseEvent *event);

	// ���ý��������ͼ��
	virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:

	// ������С�������/��ԭ���رղ���
	void onClicked();

private:

	// ���/��ԭ
	void updateMaximize();

private:
	bool m_bPressed;
	QPoint m_point;
	QLabel *m_pIconLabel;
	QLabel *m_pTitleLabel;
	QPushButton *m_pMinimizeButton;
	QPushButton *m_pMaximizeButton;
	QPushButton *m_pCloseButton;
};

#endif // TITLE_BAR_H
