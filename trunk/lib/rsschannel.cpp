
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
RssChannel::RssChannel(void)
{
    if( docref == -1)
	{
		createDocument();//the doc is shared among channels
		loadSavedFeeds();//load the feed data saved in the disk
	}
    docref ++;

    buf = new QBuffer(this);
    buf->open(QIODevice::WriteOnly|QIODevice::ReadOnly);

    getter = new WGet;

	connect(getter,SIGNAL(requestFinished()),this,SLOT(downloadFinished()));
	connect(getter,SIGNAL(networkError(const QString&)),this,SIGNAL(networkError(const QString&)));

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

bool RssChannel::connectChannel(bool force)
{
    //check the cache first
	if( !force && !currentfeedlist.isEmpty() )
    {
		emit logMessage("RssChannel::connectChannel:find the cache for:" + channelUrl.toString());
		emit doneParse();

		return true;

    }
    //not in the cache, then get the content from the site and parse it!
	buf = getter->requestUrl(channelUrl.toString());
	return true;
}

void  RssChannel::downloadFinished(){
	qDebug()<<QString(tr("download finished ..."));
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
		//parse finishes, we need to save the feeds into the cache
		cacheFeeds();//update currentfeedlist
		emit doneParse();
   }
}

void RssChannel::cacheFeeds(void)
{
	qDebug() <<"RssChannel::cacheFeeds...";
	if( !currentfeedlist.isEmpty())
		currentfeedlist.clear();
	QList<RSSFeed*> list = doc->getFeedList();
	while(!list.isEmpty())
	{
		currentfeedlist.append(list.takeFirst());
	}
}

bool RssChannel::loadSavedFeeds(void)
{
	qWarning() << "loadSavedFeeds: not implemented yet!";
	return false;
}

const QString RssChannel::feedContent(int id)
{
	QString content = doc->feedContentHint(currentfeedlist, id);
    return content;

}
