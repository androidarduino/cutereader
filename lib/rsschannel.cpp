
/*
	Author: zhu, vrcats, shiroki@www.cuteqt.com
	License: GPLv2
*/

#include <QBuffer>
#include <QDomDocument>
#include <QByteArray>
#include <QDebug>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include "wget.h"

#include "rsschannel.h"
#include "rssdocument.h"

//#define DEBUG_CONTENT
RSSDocument* RssChannel::doc = NULL;
int RssChannel::docref = -1;
static void LogFile(const QString& content)
{
	QFile file("rsschannel.log");
	file.open(QIODevice::Append);
	QTextStream ts(&file);
	ts << content;
	file.close();
	return;
}
RssChannel::RssChannel(void):currentfeedlist(NULL)
{
    if( docref == -1)
        createDocument();
    docref ++;

    buf = new QBuffer(this);
    buf->open(QIODevice::WriteOnly|QIODevice::ReadOnly);

    getter = new WGet;

    connect(getter,SIGNAL(requestFinished()),this,SLOT(download_finish()));
    connect(getter,SIGNAL(networkError(const QString)),this,SIGNAL(networkError(const QString)));
    connect(this, SIGNAL(doneParse()), SLOT(cacheFeeds()));
}


RssChannel::~RssChannel()
{
    docref --;
    if(docref == 0)
    {
        delete doc;
        doc = NULL;
        docref = -1;
    }
}

const RSSDocument* RssChannel::createDocument()
{
    Q_ASSERT(doc == NULL);
    //channels share a doc to parse the rss content
    doc = new RSSDocument;//singleton
    return doc;
}


int RssChannel::status()
{
	return 0;
}
const QUrl& RssChannel::url()
{
	return channelUrl;
}

void RssChannel::setUrl(const QUrl&url)
{
	channelUrl = url;
}

bool RssChannel::connectChannel()
{
    //check the cache first
    if( pcurrentfeedlist != 0 )
    {
        qWarning() << "BJBJ:find the cache for:" << channelUrl;


    }
    //not in the cache, then get the content from the site and parse it!
	buf = getter->requestUrl(channelUrl.toString());
	return true;
}

void  RssChannel::download_finish(){
    qDebug()<<QString(tr("download finish ..."));
    rawData = buf->readAll();
    buf->close();
    emit doneDownload();
    rawDataChanged();
}

//return the raw content for this channel
const QByteArray RssChannel::getRawData()
{
	return rawData;	
}

void RssChannel::setRawData(const QByteArray data)
{
	rawData = data;
}

void RssChannel::rawDataChanged()
{
    qDebug()<<"data size is"<< rawData.size();
    if(!doc->setDocument(rawData))
    {
        qDebug() << "document parse error...";
        return;
    }
    else{
        qDebug()<<QString(tr("parse finish ..."));
        emit doneParse();
   }
}

void RssChannel::cacheFeeds(void)
{
    //parsing has finished, so we can save the feeds into the cache, so that we can save some time next time to parse it again.
    currentfeedlist = doc->getFeedList());
}

const QString RssChannel::feedContent(int id)
{
	QString content = doc->feedContentHint(id);
    return content;

}
