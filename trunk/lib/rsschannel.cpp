#include <QBuffer>
#include <QDomDocument>
#include <QByteArray>
#include <QDebug>
#include <QTextCodec>

#include <QFile>

#include "rsschannel.h"

RssChannel::RssChannel(void)
{
    buf=new QBuffer(this);
    buf->open(QIODevice::WriteOnly|QIODevice::ReadOnly);

    getter=new HttpGet();
    doc =new QDomDocument();

    connect(getter,SIGNAL(done()),this,SLOT(download_finish()));
    connect(getter,SIGNAL(httpError(const QString)),this,SIGNAL(networkError(const QString)));
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
#if 1
    if(!getter->getFile(channelUrl,buf))
    {
        return false;
    }
#endif
	return true;
}

QString RssChannel::getContent(int titleid)
{
    QDomNode channel_node=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList list_in_channel=channel_node.childNodes();
  
    int i=item_num_of_allnodes[titleid];
    //qDebug()<<titleid<<" "<<i;    
    QDomNode item_node=list_in_channel.at(i);

    QDomNodeList list_in_item;
    list_in_item=item_node.childNodes();

    int rss2_format=0;
    int description_no=0;
    QDomNode content_node;
    for(int j=0;j<list_in_item.count();j++){
        content_node=list_in_item.at(j);
        //qDebug()<<"node name is __"<<content_node.nodeName();
        //qDebug()<<"node type is __"<<content_node.nodeType();
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

QString  RssChannel::codecname()
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

QString RssChannel::getTitle()
{
    QDomNode node=doc->firstChildElement("rss").firstChildElement("channel").firstChildElement("title");
    QString title=node.toElement().text();
    qDebug()<<"title is "<<title;
    return title;
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
#if 0
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
    emit doneDownload();
    QByteArray data;
    data=buf->data();
#if 0
	data.replace(QByteArray("&gt;"), QByteArray(">"));
	data.replace(QByteArray("&lt;"), QByteArray("<"));
	data.replace(QByteArray("&quot;"), QByteArray("\""));
	//data.replace(QByteArray("&amp;"), QByteArray("&"));
    qDebug()<<QString(data);
#endif
    qDebug()<<"data size is"<<data.size();
    QString errorStr;
    int errorLine;
    int errorCol;
  if (!doc->setContent(data,true,&errorStr,&errorLine,&errorCol))
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
