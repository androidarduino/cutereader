#ifndef RSSCHANNEL_H
#define RSSCHANNEL_H

#include <QHttp>
#include <QUrl>

#include "httpget.h"

class QBuffer;
class QDomDocument;

class RssChannel:public QObject
{
    Q_OBJECT
public:
    RssChannel();
    ~RssChannel();
    int status();
    const QUrl& url();


public slots:
    void setUrl(const QUrl& url);
    bool connectChannel(void);
/*
    bool fetchTitles();
    bool fetchContent(int titleid);
*/
    QString getContent(int titleid);
    QStringList getTitles();
    QString getTitle();
signals:
/*
    void titlesFetched(int count);
    void contentFetched(int titleid);
*/
    void doneDownload();
    void doneParse();

protected slots:
    void download_finish();
    QString codecname();
    //void parse_finish();

private:
    QMap<int,int> item_num_of_allnodes;
    QMap<int,QString> titlemap;
    QBuffer *buf;
    QDomDocument *doc;
    HttpGet *getter;
    QUrl channelUrl;
};
#endif
