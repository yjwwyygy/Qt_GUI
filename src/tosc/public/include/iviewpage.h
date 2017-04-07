#ifndef IVIEWPAGE_H
#define IVIEWPAGE_H

#include "frame_global.h"
#include <QWidget>

class FRAME_EXPORT IViewPage : public QWidget
{
	Q_OBJECT

public:
	IViewPage(QWidget *parent);
	virtual ~IViewPage();

private:
	
};

#endif // IVIEWPAGE_H
