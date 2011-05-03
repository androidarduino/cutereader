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
#include <QByteArray>
#include <QStringList>

class QIODevice;
class WGet;
#ifndef RSS_DOCUMENT_SUPPORT
class QDomDocument;
#else
class RSSDocument;
#endif
class RssChannel:public QObject
{
    Q_OBJECT
public:
    RssChannel();
    ~RssChannel();
    int status();
    const QUrl& url();
    const QString feedContent();

public slots:
    void setUrl(const QUrl& url);
    bool connectChannel(void);
#ifndef RSS_DOCUMENT_SUPPORT
    QStringList getTitles();
    QString getChannelName();
    QString getChannelLink();
    QString getTitle(int);
    QString getTitleName(int titleid);
    QString getTitleLink(int titleid);
    QString getTitleContent(int titleid);
#else
    RSSDocument* rssDocument(){return doc;};
#endif
    QString getRawData();
    void setRawData(const QByteArray rawdata);//give me the rawcontent, I will parse it!
signals:
    void doneDownload();
    void doneParse();
    void networkError(const QString error);

protected slots:
    void download_finish();
#ifndef RSS_DOCUMENT_SUPPORT
    QString codecname();
#endif
    void rawDataChanged();
    //void parse_finish();

private:
    QMap<int,int> item_num_of_allnodes;
    QMap<int,QString> titlemap;
#ifdef RSS_DOCUMENT_SUPPORT
    RSSDocument* doc;
#else
    QDomDocument *doc;
#endif
    QByteArray rawData;
    QIODevice *buf;
    WGet *getter;
    QUrl channelUrl;
    QTextCodec *textc;
    int cid;//current title id we'r working on
    QString ctitlename, ctitlelink, ctitlecontent;
};
#endif

