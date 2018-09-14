#include "stdafx.h"
#include "Neptune.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CNeptune w;
	w.show();
	return a.exec();
}
