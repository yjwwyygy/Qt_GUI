#include "mainwidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWidget w;
	if (w.createMainWindow())
	{
		if (w.login())
		{
			return a.exec();
		}
	}
	return 0;
}
