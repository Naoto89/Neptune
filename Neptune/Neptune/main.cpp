#include "stdafx.h"
#include "Neptune.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Neptune w;
	w.show();
	return a.exec();
}
