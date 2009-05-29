#include "rssdocument.h"
#define RFC822 "ddd, dd MMM yyyy hh:mm:ss"

RSSItem::RSSItem(QDomElement e)
{
    QDomNodeList properties=e.childNodes();
    for(int i=0;i<properties.count();i++)
    {
        QDomElement c=properties.at(i).toElement();
        QString tagName=c.tagName().toLower();
        if(tagName=="title")
            qDebug()<<tagName+": "<<(title=c.text());
        if(tagName=="link")
            qDebug()<<tagName+": "<<(link=c.text());
        if(tagName=="description")
            qDebug()<<tagName+": "<<(description=c.text());
        if(tagName=="author")
            qDebug()<<tagName+": "<<(author=c.text());
        if(tagName=="category")//rsscategory
        {
            RSSCategory* ct=new RSSCategory();
            ct->setDocument(c);
            categories<<ct;
        }
        if(tagName=="comments")
            qDebug()<<tagName+": "<<(comments=c.text());
        if(tagName=="enclosure")//rssenclosure
        {
            enclosure.setDocument(c);
            qDebug()<<tagName+ ": " + enclosure.toHtml();
        }
        if(tagName=="guid")
            qDebug()<<tagName+": "<<(guid=c.text());
        if(tagName=="pubdate")
            qDebug()<<tagName+": "<<(pubDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        if(tagName=="source")
            qDebug()<<tagName+": "<<(source=c.text());
    }
}

RSSDocument::RSSDocument()
{

}

const QVariant RSSFeed::getProperty(RSS /*property*/) const
//get property listed in enum RSSChannelProperty
{

}

QList<RSSItem*> RSSFeed::getItems() const
//get all items in the document
{
    return QList<RSSItem*>();
}

/*QDateTime RSSFeed::RFC822toQDateTime(QString rfc822)
{
    QRegExp rx("[\\w*,]?\\s*(\\d*)\\s*(\\w*)\\s*(\\d*)\\s*(\\d*:\\d*:\\d*)\\s*(\\w*)");
    rx.indexOf(rfc822);
    QMap<QString, QString> map;
    map["Jan"]="1";map["Feb"]="2";map["Mar"]="3";map["Apr"]="4";map["May"]="5";map["Jun"]="6";map["Jul"]="7";map["Aug"]="8";map["Sep"]="9";map["Oct"]="10";map["Nov"]="11";map["Dec"]="12";
    QString ret="%1.";
    rx.cap(1);//day
    rx.cap(2);//month
    rx.cap(3);//year
    rx.cap(4);//time
}*/

/*
RSSItem& RSSFeed::getItem(QString itemId) const
//get an item by guid
{
    return *(new RSSItem());
}
*/

void RSSDocument::setDocument(QString xmlSrc)
{
    //qDebug()<<xmlSrc;
    m_doc.setContent(xmlSrc);
    //find <rss> root
    QDomElement rssRoot=m_doc.elementsByTagName("rss").at(0).toElement();//in atom this root doesn't exist
    QDomNodeList feeds=rssRoot.elementsByTagName("channel");//or <feed>
    for(uint i=0;i<feeds.length();i++)
        m_feeds<<new RSSFeed(feeds.at(i));
}

RSSFeed::RSSFeed(QDomNode e)
{
    QDomNodeList children=e.childNodes();
    for(int i=0;i<children.count();i++)
    {
        QDomElement c=children.at(i).toElement();
        QString tagName=c.tagName().toLower();

        if(tagName=="item")
        {
            m_items<<new RSSItem(c);
        }

        if(tagName=="title")
            qDebug()<<"TITLE: "<<(title=c.text());
        if(tagName=="link")
            qDebug()<<"LINK: "<<(link=c.text());
        if(tagName=="description")
            qDebug()<<"DESCRIPTION: "<<(description=c.text());
        if(tagName=="language")
            qDebug()<<"LANGUAGE: "<<(language=c.text());
        if(tagName=="copyright")
            qDebug()<<"COPYRIGHT: "<<(copyright=c.text());
        if(tagName=="managingeditor")
            qDebug()<<"MANAGINGEDITOR: "<<(managingEditor=c.text());
        if(tagName=="webmaster")
            qDebug()<<"WEBMASTER: "<<(webMaster=c.text());
        if(tagName=="pubdate")//datetime
            qDebug()<<"PUBDATE: "<<(pubDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        if(tagName=="lastBuildDate")//datetime
            qDebug()<<"LASTBUILDDATE: "<<(lastBuildDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        if(tagName=="generator")
            qDebug()<<"GENERATOR: "<<(generator=c.text());
        if(tagName=="docs")
            qDebug()<<"DOCS: "<<(docs=c.text());
        if(tagName=="ttl")//int
            qDebug()<<"TTL: "<<(ttl=c.text().toInt());
        if(tagName=="image")//rssimage
            qDebug()<<"IMAGE: "<<c.text();//TODO
        if(tagName=="pics")//rsspics
            qDebug()<<"RATING: "<<c.text();//TODO
        if(tagName=="textinput")//rssTextInput
            qDebug()<<"TEXTINPUT: "<<c.text();
        if(tagName=="skiphours")//rssSkipHOurs
            qDebug()<<"SKIPHOURS: "<<c.text();
        if(tagName=="skipdays")//rssskipdays
            qDebug()<<"SKIPDAYS: "<<c.text();
        if(tagName=="category")//rsscategory
            qDebug()<<"CATEGORY: "<<c.text();
        if(tagName=="cloud")//rsscloud
            qDebug()<<"CLOUD: "<<c.text();
    }
}

QString RSSImage::toHtml()
{
    if(empty())
        return "";
    QString ret="<img width=%1 height=%2 src=""%3"" alt=""%4""></img>";
    return ret.arg(width).arg(height).arg(url.toString()).arg(description);
    //note: link is not very useful, so the html didn't include it.
}

QString RSSEnclosure::toHtml()
{
     if(empty())
        return "";
    QString ret="<a href=""%1"">Attachment(%3): %2k</a>";
    return ret.arg(url.toString()).arg(length/1024).arg(type);
}

QString RSSTextInput::toHtml()
{
    //TODO
    return "";
}

QString RSSCategory::toHtml()
{
    return content;
}

void RSSCategory::setDocument(QDomElement e)
{
    content=e.text();
    domain=e.attribute("domain");
}

void RSSImage::setDocument(QDomElement e)
{
    QDomNodeList children=e.childNodes();
    for(int i=0;i<children.count();i++)
    {
        QDomElement c=children.at(i).toElement();
        QString tagName=c.tagName().toLower();
        if(tagName=="url")
        {
            url=QUrl(c.text());
        }
        if(tagName=="title")
        {
            title=c.text();
        }
        if(tagName=="link")
        {
            link=c.text();
        }
        if(tagName=="description")
        {
            description=c.text();
        }
    }
}

void RSSEnclosure::setDocument(QDomElement e)
{
    url=QUrl(e.attribute("url"));
    length=e.attribute("length").toLong();
    type=e.attribute("type");
}


