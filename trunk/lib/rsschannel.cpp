
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
#ifdef RSS_DOCUMENT_SUPPORT
    doc = new RSSDocument;
#else
    doc = new QDomDocument();
#endif

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
#ifndef RSS_DOCUMENT_SUPPORT
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
#endif

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
#ifndef RSS_DOCUMENT_SUPPORT
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
            qDebug() << "title: " << nnn.toElement().text();
            str_list<<nnn.toElement().text();
            item_num_of_allnodes[j]=i;
            j++;
        }
    }
    return str_list;
}
#endif//RSS_DOCUMENT_SUPPORT

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
#ifndef RSS_DOCUMENT_SUPPORT
    QString errorStr;
    int errorLine;
    int errorCol;

  if (!doc->setContent(rawData,true,&errorStr,&errorLine,&errorCol))
  {
      qDebug()<<tr("error:%1 in line:%2  colummn:%3").arg(errorStr).arg(errorLine).arg(errorCol);
       return;
  }
#else
    if(!doc->setDocument(rawData))
    {
        qDebug() << "document parse error...";
        return;
    }
#endif
    else{
        qDebug()<<QString(tr("parse finish ..."));
        emit doneParse();
   }
#if 0
    qDebug()<<codecname();
    qDebug()<<get_feedlist();
    qDebug()<<getContent(13);
    qDebug()<<getContent(0);
#endif
}

const QString RssChannel::feedContent()
{
    QString titleinfo, content;
    const QList<RSSFeed*> list = doc->getFeedList();

    if( list.count() == 0)
        return content;

    RSSFeed* fd;
    QString copyright, docs, generator, language, lastbuilddate, link, managingeditor, pubdate, rating,textinput, webmaster;

    foreach(fd, list)
    {
        copyright = fd->getProperty(RSSFeed::Copyright).toString();
        docs = fd->getProperty(RSSFeed::Docs).toString();
        generator = fd->getProperty(RSSFeed::Generator).toString();
        language = fd->getProperty(RSSFeed::Language).toString();
        //lastbuilddate = fd->getProperty(RSSFeed::LastBuildDate);
        link = fd->getProperty(RSSFeed::Link).toString();
        managingeditor = fd->getProperty(RSSFeed::ManagingEditor).toString();
        pubdate = fd->getProperty(RSSFeed::PubDate).toString();
        rating = fd->getProperty(RSSFeed::Rating).toString();
        textinput = fd->getProperty(RSSFeed::TextInput).toString();
        webmaster = fd->getProperty(RSSFeed::WebMaster).toString();
        qDebug() << "BJBJ:Copyright:" << copyright;
        qDebug() << "BJBJ:Docs:" << docs;
        qDebug() << "BJBJ:Generator:" << generator;
        qDebug() << "BJBJ:Language:" << language;
        qDebug() << "BJBJ:LastBuildDate:" << lastbuilddate;
        qDebug() << "BJBJ:Link:" << link;
        qDebug() << "BJBJ:ManagingEditor:" << managingeditor;
        qDebug() << "BJBJ:PubDate:" << pubdate;
        qDebug() << "BJBJ:Rating:" << rating;
        qDebug() << "BJBJ:TextInput:" << textinput;
        qDebug() << "BJBJ:WebMaster:" << webmaster;
        titleinfo += "RSS from <a href=\"" + link + "\">" + link + "</a><br>";
        titleinfo += "<p>-------------------------------------------</p>";

        //feed items
        RSSItem* it;
        QList<RSSItem*> itemlist = fd->getItems();
        if(itemlist.count() == 0)
            continue;
        foreach(it, itemlist)
        {
            /*
    QString title;//The title of the item.  Venice Film Festival Tries to Quit Sinking
    QUrl link;//The URL of the item.    http://nytimes.com/2004/12/07FEST.html
    QString description;//The item synopsis.  Some of the most heated chatter at the Venice Film Festival this week was about the way that the arrival of the stars at the Palazzo del Cinema was being staged.
    QString author;//Email address of the author of the item. More.  oprah\@oxygen.net
    QList<RSSCategory*> categories;//Includes the item in one or more categories. More.
    QUrl comments;//URL of a page for comments relating to the item. More.  http://www.myblog.org/cgi-local/mt/mt-comments.cgi?entry_id=290
    RSSEnclosure enclosure;//Describes a media object that is attached to the item. More.
    QString guid;//A string that uniquely identifies the item. More.   http://inessential.com/2002/09/01.php#a2
    QDateTime pubDate;//Indicates when the item was published. More.    Sun, 19 May 2002 15:21:36 GMT
    QString source;//The RSS channel that the item came from. More.
              */
            qDebug() << "BJBJ:title:" << it->title;
            qDebug() << "BJBJ:link:" << it->link;
            qDebug() << "BJBJ:description:" << it->description;
            qDebug() << "BJBJ:author:" << it->author;
            qDebug() << "BJBJ:categories:" << it->categories;
            qDebug() << "BJBJ:comments:" << it->comments;
            //qDebug() << "BJBJ:enclosure:" << it->enclosure;
            qDebug() << "BJBJ:guid:" << it->guid;
            qDebug() << "BJBJ:pubDate:" << it->pubDate;
            qDebug() << "BJBJ:source:" << it->source;
            titleinfo += "<a href= \"" + it->link.path() + "\">" + it->title+ "</a><br>";
            content += "<p>" + it->description + "</p>" + it->text;
        }

    }

    content = titleinfo + content;
    return content;

}
