#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>

class Widget : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = 0);
	~Widget();
};

#endif // WIDGET_H
