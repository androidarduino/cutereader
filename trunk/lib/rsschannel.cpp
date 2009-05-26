#include <QBuffer>
#include <QDomDocument>
#include <QByteArray>
#include <QDebug>

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
    QDomNode n=doc->firstChildElement("rss").firstChildElement("channel");
    QDomNodeList node_list=n.childNodes();
    QDomNode nn,nnn;
    QDomText node_title;
  
#if 0 
    for(int i=0;i<node_list.count();i++){
        nn=node_list.at(i);
        if(nn.nodeName()=="item"){
            //nnn=nn.firstChildElement("title");
            nnn=nn.firstChildElement("description");
            //node_title=nnn.firstChildElement();
            qDebug()<<nnn.toElement().text();
            //item_num_of_allnodes[j]=i;
            //j++;
        }
    }
#else
    int i=item_num_of_allnodes[titleid];
    qDebug()<<titleid<<" "<<i;    
    nn=node_list.at(i);
    nnn=nn.firstChildElement("description");
    return nnn.toElement().text();
#endif

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
	data.replace(QByteArray("&gt;"), QByteArray(">"));
	data.replace(QByteArray("&lt;"), QByteArray("<"));
	data.replace(QByteArray("&quot;"), QByteArray("\""));
	//data.replace(QByteArray("&amp;"), QByteArray("&"));
    qDebug()<<QString(data);
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
#if 0
    qDebug()<<codecname();
    qDebug()<<get_feedlist();
    qDebug()<<getContent(13);
    qDebug()<<getContent(0);
#endif
}
