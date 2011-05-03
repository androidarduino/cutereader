
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
    buf = new QBuffer(this);
    buf->open(QIODevice::WriteOnly|QIODevice::ReadOnly);

    getter = new WGet;
    //each channel holds a doc to parse the rss content
    doc = new RSSDocument;

    connect(getter,SIGNAL(requestFinished()),this,SLOT(download_finish()));
    connect(getter,SIGNAL(networkError(const QString)),this,SIGNAL(networkError(const QString)));
}


RssChannel::~RssChannel()
{
    delete doc;
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
QString RssChannel::getRawData()
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

const QString RssChannel::feedContent(int id)
{
    QString titleinfo, content;
	content = doc->feedContentHint();
    return content;

}
