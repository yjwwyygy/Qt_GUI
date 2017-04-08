#include "mainwidget.h"
#include <QtWidgets/QApplication>
#include "icontext.h"

IContext* gContext = nullptr;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	gContext = new IContext();

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
