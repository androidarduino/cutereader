#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QTranslator>

#include "mainwin.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	qDebug() << QLocale::system().name();
	QTranslator tr, qttr;
	tr.load("i18n/cutereader-"+QLocale::system().name());
	qttr.load("qt-"+QLocale::system().name());
	app.installTranslator(&tr);
	app.installTranslator(&qttr);
	

	MainWin mw;
	mw.show();
	return app.exec();
}
