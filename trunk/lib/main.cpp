#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QWebView>
#include "rssdocument.h"

QByteArray readFile(QString fileName = "")
{
	QByteArray content;
	if(fileName.simplified().isEmpty())
        	fileName="test.rss";

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{	
	content=file.readAll();
	file.close();
	}
    return content;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QWebView view;
    RSSDocument doc;
    QString fileName;
    qWarning() << argc;
    if(argc > 1)
    doc.setDocument(readFile(argv[1]));
    else
    doc.setDocument(readFile());

    if(argc > 2)
    view.setHtml(doc.feedContentHint(atoi(argv[2])));
    else
    view.setHtml(doc.feedContentHint());
    view.show();
    return app.exec();
}
