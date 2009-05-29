#include "rssdocument.h"
#include <QApplication>
#include <QFile>

QString readFile(QString fileName)
{
    if(fileName=="")
        fileName="test.rss";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";
    QString content=file.readAll();
    file.close();
    return content;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    RSSDocument doc;
    QString fileName;
    doc.setDocument(readFile(argv[1]));
    return 0;
}
