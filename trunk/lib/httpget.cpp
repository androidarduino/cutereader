#include <QtCore>
#include <QtNetwork>
#include <iostream>

#include "httpget.h"

using namespace std;

HttpGet::HttpGet(QObject *parent)
    : QObject(parent)
{
    connect(&http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
}

bool HttpGet::getFile(const QUrl &url,QBuffer *buf)
{
    if (!url.isValid()) {
        cerr << "Error: Invalid URL" << endl;
        return false;
    }

    if (url.scheme() != "http") {
        cerr << "Error: URL must start with 'http:'" << endl;
        return false;
    }

    if (url.path().isEmpty()) {
        cerr << "Error: URL has no path" << endl;
        return false;
    }

#if 0
    QString localFileName = QFileInfo(url.path()).fileName();
    if (localFileName.isEmpty())
        localFileName = "httpget.out";

    file.setFileName(localFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        cerr << "Error: Cannot open " << qPrintable(file.fileName())
             << " for writing: " << qPrintable(file.errorString())
             << endl;
        return false;
    }
#endif

    http.setHost(url.host(), url.port(80));
    http.get(url.path(), buf);
    http.close();
    return true;
}

void HttpGet::httpDone(bool error)
{
    if (error) {
        cerr << "Error: " << qPrintable(http.errorString()) << endl;
    } else {
        cerr << "File downloaded as " << qPrintable(file.fileName())
             << endl;
    }
//    file.close();
    emit done();
}
