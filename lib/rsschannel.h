/*
	Author: zhu, vrcats, shiroki@www.cuteqt.com
	License: GPLv2
*/

#ifndef RSSCHANNEL_H
#define RSSCHANNEL_H

#include <QTextCodec>
#include <QObject>
#include <QUrl>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QStringList>

class QIODevice;
class WGet;
class RSSDocument;
class RSSFeed;

class RssChannel:public QObject
{
    Q_OBJECT
public:
    RssChannel();
    ~RssChannel();
    int status();
    const QUrl& url();
    const QString feedContent(int id = -1);

public slots:
    void setUrl(const QUrl& url);
    bool connectChannel(void);
    RSSDocument* rssDocument(){return doc;};
	const QByteArray getRawData();
    void setRawData(const QByteArray rawdata);//give me the rawcontent, I will parse it!
signals:
    void doneDownload();
    void doneParse();
    void networkError(const QString error);

protected slots:
    void download_finish();
    void rawDataChanged();
    //void parse_finish();

private:
    const RSSDocument* createDocument(void);
    void cacheFeeds(void);
    static RSSDocument* doc;//single instance shared among all channels
    static int docref; //reference count for the doc
    QByteArray rawData;
    QIODevice *buf;
    WGet *getter;
    QUrl channelUrl;
    QList<RSSFeed*> currentfeedlist;//this is the cache for the feeds content
    int cid;//current title id we'r working on
};
#endif

