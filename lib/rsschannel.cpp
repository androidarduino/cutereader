
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
#include <QRegExp>
#include "wget.h"

#include "rsschannel.h"

#define DEBUG_CONTENT
RssChannel::RssChannel(void)
{
    buf = new QBuffer(this);
    buf->open(QIODevice::WriteOnly|QIODevice::ReadOnly);

    getter = new WGet;
    doc = new QDomDocument();

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

QString RssChannel::codecname()
{
    QDomProcessingInstruction qdominst=doc->firstChild().toProcessingInstruction();
    QString str=qdominst.data();
    qDebug()<<str;
    QRegExp rxlen("encoding=\'(.*)\'");
    int pos = rxlen.indexIn(str);
    if (pos > -1) {
        return rxlen.cap(1);
    }else
    {
        return QString();
    }
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

QString RssChannel::getChannelName()
{
    QDomNode node=doc->firstChildElement("rss").firstChildElement("channel").firstChildElement("title");
    QString title=node.toElement().text();
    qDebug()<<"title is "<<title;
    return title;
}

QString RssChannel::getChannelLink()
{
    QDomNode node=doc->firstChildElement("rss").firstChildElement("channel").firstChildElement("link");
    QString text =node.toElement().text();
    qDebug()<<"link is "<< text;
    return text;
}

QString RssChannel::getTitle(int titleid)
{
	if( cid == titleid)
		return ctitlename;

    QDomNode channel_node=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList list_in_channel=channel_node.childNodes();
    int i=item_num_of_allnodes[titleid];
#ifdef DEBUG_CONTENT
    qDebug() << "title:" << titleid << " " << i;    
#endif
    QDomNode item_node=list_in_channel.at(i);

    QDomNodeList list_in_item;
    list_in_item=item_node.childNodes();

    int rss2_format=0;
    int node_no=0;
    QDomNode content_node;
    for(int j=0;j<list_in_item.count();j++){
        content_node=list_in_item.at(j);
#ifdef DEBUG_CONTENT
        qDebug()<<"node name:" << content_node.nodeName();
#endif
        if(content_node.nodeName()=="content:encoded"){
            rss2_format=1;
            break;
        }
        else if(content_node.nodeName()=="title"){
	    node_no=j;
		ctitlename = content_node.toElement().text();
        }
        else if(content_node.nodeName()=="link"){
	    node_no=j;
		ctitlelink = content_node.toElement().text();
        }
        else if(content_node.nodeName()=="description"){
	    node_no=j;
		ctitlecontent= content_node.toElement().text();
        }
    }
    if(1!=rss2_format){
	content_node=list_in_item.at(node_no);
    }
    cid = titleid;
    return ctitlename;
}

QString RssChannel::getTitleName(int titleid)
{
	if( cid == titleid)
		return ctitlename;

    QDomNode channel_node=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList list_in_channel=channel_node.childNodes();
    int i=item_num_of_allnodes[titleid];
#ifdef DEBUG_CONTENT
    qDebug()<<titleid<<" "<<i;    
#endif
    QDomNode item_node=list_in_channel.at(i);

    QDomNodeList list_in_item;
    list_in_item=item_node.childNodes();

    int rss2_format=0;
    int node_no=0;
    QDomNode content_node;
    for(int j=0;j<list_in_item.count();j++){
        content_node=list_in_item.at(j);
#ifdef DEBUG_CONTENT
        qDebug()<<"node name is __"<<content_node.nodeName();
#endif
        if(content_node.nodeName()=="content:encoded"){
            rss2_format=1;
            break;
        }
        if(content_node.nodeName()=="title"){
	    node_no=j;
        }
    }
    if(1!=rss2_format){
	content_node=list_in_item.at(node_no);
    }
    QString ss= content_node.toElement().text();
    ctitlename = ss;
    return ctitlename;
	
}

QString RssChannel::getTitleLink(int titleid)
{

	if( cid == titleid)
		return ctitlelink;

    QDomNode channel_node=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList list_in_channel=channel_node.childNodes();
    int i=item_num_of_allnodes[titleid];
#ifdef DEBUG_CONTENT
    qDebug()<<titleid<<" "<<i;    
#endif
    QDomNode item_node=list_in_channel.at(i);

    QDomNodeList list_in_item;
    list_in_item=item_node.childNodes();

    int rss2_format=0;
    int node_no=0;
    QDomNode content_node;
    for(int j=0;j<list_in_item.count();j++){
        content_node=list_in_item.at(j);
#ifdef DEBUG_CONTENT
        qDebug()<<"node name is __"<<content_node.nodeName();
#endif
        if(content_node.nodeName()=="content:encoded"){
            rss2_format=1;
            break;
        }
        if(content_node.nodeName()=="link"){
	    node_no=j;
        }
    }
    if(1!=rss2_format){
	content_node=list_in_item.at(node_no);
    }
    QString ss= content_node.toElement().text();
    ctitlelink = ss;
    return ctitlelink;
}


QString RssChannel::getTitleContent(int titleid)
{

	if( cid == titleid)
		return ctitlecontent;

    QDomNode channel_node=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList list_in_channel=channel_node.childNodes();
    int i=item_num_of_allnodes[titleid];
#ifdef DEBUG_CONTENT
    qDebug()<<titleid<<" "<<i;    
#endif
    QDomNode item_node=list_in_channel.at(i);

    QDomNodeList list_in_item;
    list_in_item=item_node.childNodes();

    int rss2_format=0;
    int description_no=0;
    QDomNode content_node;
    for(int j=0;j<list_in_item.count();j++){
        content_node=list_in_item.at(j);
#ifdef DEBUG_CONTENT
        qDebug()<<"node type is __"<<content_node.nodeType();
        qDebug()<<"node name is __"<<content_node.nodeName();
#endif
        if(content_node.nodeName()=="content:encoded"){
            rss2_format=1;
            break;
        }
        if(content_node.nodeName()=="description"){
	    description_no=j;
        }
    }
    if(1!=rss2_format){
	content_node=list_in_item.at(description_no);
    }
    QString ss= content_node.toElement().text();
    return ss;
}


QStringList RssChannel::getTitles()
{
    QStringList str_list;
    QDomNode n=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList node_list=n.childNodes();
    QDomNode nn,nnn;
    QDomText node_title;

    int j=0;
    for(int i=0;i<node_list.count();i++){
        nn=node_list.at(i);
#ifdef DEBUG_TITLE
        qDebug()<<nn.nodeName();
        if (nn.isElement()) {
            QDomElement e = nn.toElement();
            qDebug() << "Element name: " << e.tagName() << endl;
        }
#endif
        if(nn.nodeName()=="item"){
            nnn=nn.firstChildElement("title");
            //node_title=nnn.firstChildElement();
            str_list<<nnn.toElement().text();
            item_num_of_allnodes[j]=i;
            j++;
        }
    }
    return str_list;
}

void  RssChannel::download_finish(){
    qDebug()<<QString(tr("download finish ..."));
    rawData = buf->readAll();
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
    emit doneDownload();
    qDebug()<<"data size is"<< rawData.size();
    QString errorStr;
    int errorLine;
    int errorCol;
  if (!doc->setContent(rawData,true,&errorStr,&errorLine,&errorCol))
   {
       qDebug()<<tr("error:%1 in line:%2  colummn:%3").arg(errorStr).arg(errorLine).arg(errorCol);
	return;
   }else{
        qDebug()<<QString(tr("parse finish ..."));
        emit doneParse();
   }
  buf->close();
#if 0
    qDebug()<<codecname();
    qDebug()<<get_feedlist();
    qDebug()<<getContent(13);
    qDebug()<<getContent(0);
#endif
}
