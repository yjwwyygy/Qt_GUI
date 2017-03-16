#ifndef FRAMELESS_HELPER_H
#define FRAMELESS_HELPER_H

#include <QObject>
#include <QWidget>

class FramelessHelperPrivate;

class FramelessHelper : public QObject
{
	Q_OBJECT

public:
	FramelessHelper(QObject *parent);
	~FramelessHelper();

	// �����
	void activateOn(QWidget *topLevelWidget);
	// �Ƴ�����
	void removeFrom(QWidget *topLevelWidget);
	// ���ô����ƶ�
	void setWidgetMovable(bool movable);
	// ���ô�������
	void setWidgetResizable(bool resizable);
	// ������Ƥ���ƶ�
	void setRubberBandOnMove(bool movable);
	// ������Ƥ������
	void setRubberBandOnResize(bool resizable);
	// ���ñ߿�Ŀ��
	void setBorderWidth(uint width);
	// ���ñ������߶�
	void setTitleHeight(uint height);
	bool widgetResizable();
	bool widgetMovable();
	bool rubberBandOnMove();
	bool rubberBandOnResisze();
	uint borderWidth();
	uint titleHeight();

protected:
	// �¼����ˣ������ƶ������ŵ�
	virtual bool eventFilter(QObject *obj, QEvent *event);

private:
	FramelessHelperPrivate *d;
};

#endif // FRAMELESS_HELPER_H
