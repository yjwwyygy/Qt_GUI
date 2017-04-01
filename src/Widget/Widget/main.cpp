#include "widget.h"
#include <QtWidgets/QApplication>

#include "custom_window.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Widget w;
	//w.show();

	CustomWindow w(NULL);
	w.resize(500, 500);
	w.show();

	return a.exec();
}
