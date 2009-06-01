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
class QDomDocument;
class WGet;

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
    QString getContent(int titleid);
    QStringList getTitles();
    QString getTitle();
    QString getRawData();
    void setRawData(const QByteArray rawdata);//give me the rawcontent, I will parse it!
signals:
    void doneDownload();
    void doneParse();
    void networkError(const QString error);

protected slots:
    void download_finish();
    QString codecname();
    void rawDataChanged();
    //void parse_finish();

private:
    QMap<int,int> item_num_of_allnodes;
    QMap<int,QString> titlemap;
    QIODevice *buf;
    QByteArray rawData;
    QDomDocument *doc;
    WGet *getter;
    QUrl channelUrl;
    QTextCodec *textc;
};
#endif

