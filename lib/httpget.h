#ifndef HTTPGET_H
#define HTTPGET_H

#include <QFile>
#include <QHttp>
#include <QString>

class QBuffer;

class QUrl;

class HttpGet : public QObject
{
    Q_OBJECT

public:
    HttpGet(QObject *parent = 0);

    bool getFile(const QUrl &url,QBuffer *buf);

signals:
    void done();
    void httpError(const QString msg);


private slots:
    void httpDone(bool error);
    void httpReqDone(int,bool);

private:
    int http_request_id;
    QHttp http;
    QFile file;
};

#endif
