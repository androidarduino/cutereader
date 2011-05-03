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
	qWarning() << content;
    return content;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
    QWebView view;
    RSSDocument doc;
    QString fileName;
    if(argc > 0)
    doc.setDocument(readFile(argv[1]));
    else
    doc.setDocument(readFile());

    view.setHtml(doc.feedContentHint());
    view.show();
    return app.exec();
}
