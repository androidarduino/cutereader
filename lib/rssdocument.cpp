#include <QFile>
#include <QTextStream>

#include "rssdocument.h"
#define RFC822 "ddd, dd MMM yyyy hh:mm:ss"

void LogFile(const QString& content)
{
	QFile file("rssdocument.log");
	file.open(QIODevice::WriteOnly);
	QTextStream ts(&file);
	ts << content;
	file.close();
	return;
}

RSSItem::RSSItem(QDomElement e)
{
    QDomNodeList properties=e.childNodes();
    for(int i=0;i<properties.count();i++)
    {
        QDomElement c=properties.at(i).toElement();
        QString tagName=c.nodeName().toLower();//use nodeName instead of tagName
        if( tagName == "content:encoded")//"content:encoded"
	{
		contentencoded = c.text();
            //qDebug() << "content:encoded:" << contentencoded;
	}
        else if(tagName=="title")
            qDebug()<<tagName+": "<<(title=c.text());
        else if(tagName=="link")
            qDebug()<<tagName+": "<<(link=c.text());
        else if(tagName=="description")
	{
		description = c.text();
        //    qDebug() << "description:" << description;
	}
        else if(tagName=="author")
            qDebug()<<tagName+": "<<(author=c.text());
        else if(tagName=="category")//rsscategory
        {
            RSSCategory* ct=new RSSCategory();
            ct->setDocument(c);
            categories<<ct;
        }
        else if(tagName=="comments")
            qDebug()<<tagName+": "<<(comments=c.text());
        else if(tagName=="enclosure")//rssenclosure
        {
            enclosure.setDocument(c);
            //qDebug()<<tagName+ ": " + enclosure.toHtml();
        }
        else if(tagName=="guid")
            qDebug()<<tagName+": "<<(guid=c.text());
        else if(tagName=="pubdate")
            qDebug()<<tagName+": "<<(pubDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        else if(tagName=="source")
            qDebug()<<tagName+": "<<(source=c.text());
	else
	{
        qWarning() << "unknown itemtagname:" << tagName;
	}
    }
}

RSSDocument::RSSDocument()
{

}

const QString RSSDocument::feedContentHint(const QList<RSSFeed*> feedlist,int id /*= -1*/)
{
    QString titleinfo, content;	
	QList<RSSFeed*> list;
	if( feedlist.count() != 0) list = feedlist;
	else
		list = getFeedList();

    if( list.count() == 0)
        return content;

    RSSFeed* fd;
    QString title,description,copyright,docs,generator, language, lastbuilddate, link, managingeditor, pubdate, rating,textinput, webmaster;
    QString titlecontent;
    foreach(fd, list)
    {
        title = fd->getProperty(RSSFeed::Title).toString();
        description = fd->getProperty(RSSFeed::Description).toString();
        copyright = fd->getProperty(RSSFeed::Copyright).toString();
        docs = fd->getProperty(RSSFeed::Docs).toString();
        generator = fd->getProperty(RSSFeed::Generator).toString();
        language = fd->getProperty(RSSFeed::Language).toString();
        //lastbuilddate = fd->getProperty(RSSFeed::LastBuildDate);
        link = fd->getProperty(RSSFeed::Link).toUrl().toString();
        managingeditor = fd->getProperty(RSSFeed::ManagingEditor).toString();
        pubdate = fd->getProperty(RSSFeed::PubDate).toString();
        rating = fd->getProperty(RSSFeed::Rating).toString();
        textinput = fd->getProperty(RSSFeed::TextInput).toString();
        webmaster = fd->getProperty(RSSFeed::WebMaster).toString();
		/*
        qDebug() << "BJBJ:Title:" << title;
        qDebug() << "BJBJ:Description:" << description;
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
		*/
		titleinfo += "RSS from <a href=\"" + link + "\">" + title + "</a><br><p>" + description + "</p>";
		titleinfo += "<p>-------------------------------------------</p>";

        //feed items
        RSSItem* it;
        QList<RSSItem*> itemlist = fd->getItems();
        if(itemlist.count() == 0)
            continue;

    	int currentid = -1;
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
    		currentid ++;
		if(currentid != id && id != -1)
			continue;//skip this item
		/*
            qDebug() << "BJBJ:title:" << it->title;
            qDebug() << "BJBJ:description:" << it->description;
            qDebug() << "BJBJ:author:" << it->author;
            qDebug() << "BJBJ:categories:" << it->categories;
            qDebug() << "BJBJ:comments:" << it->comments;
            //qDebug() << "BJBJ:enclosure:" << it->enclosure;
            qDebug() << "BJBJ:link:" << it->link;
            qDebug() << "BJBJ:pubDate:" << it->pubDate;
            qDebug() << "BJBJ:source:" << it->source;
			*/
		qDebug() << "BJBJ:guid:" << it->guid;
			if( id == -1 && !it->link.toString().isEmpty())//index
	    titleinfo += "<a href= \"" + it->link.toString() + "\">" + it->title+ "</a><br>";
	    titlecontent = it->contentencoded.isEmpty()? it->source: it->contentencoded;
	    if(titlecontent.isEmpty()) titlecontent = it->description;
	    
            content += "<p><h2><a href= \"" + it->link.toString() + "\">" + it->title+ "</a></h2></p>" + titlecontent + "<br><br>";
        }
    }

    content = titleinfo + content;
    LogFile(content);
	return content;
}
const QVariant RSSFeed::getProperty(RSS property) const
//get property listed in enum RSSChannelProperty
{
    switch(property)
    {
    case Title: return title;
    case Description: return description;
    case Copyright: return copyright;
    case Link: return link;
    case Docs: return docs;
    case Generator: return generator;
    case Language: return language;
    case LastBuildDate: return lastBuildDate;
    case ManagingEditor: return managingEditor;
    case PubDate: return pubDate;
    //case Rating: return rating;
    //case TextInput: return textInput;
    case WebMaster: return webMaster;
            default: return "";
    }
    return QVariant();
}

QList<RSSItem*> RSSFeed::getItems() const
//get all items in the document
{
    return m_items;
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
//bug: passing QString causes decoding problem
bool RSSDocument::setDocument(const QByteArray xmlSrc)
{
    bool result = m_doc.setContent(xmlSrc, true);
    if( ! result) return result;
    //find <rss> root
    m_feeds.clear();
    QDomElement rssRoot=m_doc.elementsByTagName("rss").at(0).toElement();//in atom this root doesn't exist
    QDomNodeList feeds=rssRoot.elementsByTagName("channel");//or <feed>
    for(uint i=0;i<feeds.length();i++)
        m_feeds<<new RSSFeed(feeds.at(i));
    return result;
}

RSSFeed::RSSFeed(QDomNode e)
{
    QDomNodeList children=e.childNodes();
    for(int i=0;i<children.count();i++)
    {
        QDomElement c=children.at(i).toElement();
        QString tagName=c.nodeName().toLower();

        if(tagName=="item")
        {
            m_items<<new RSSItem(c);
        }
        else if(tagName=="title")
            qDebug()<<"TITLE: "<<(title=c.text());
        else if(tagName=="link")
	{
            if(!c.text().isEmpty()) link=c.text();
            qDebug()<<"LINK: "<< link;
	}
        else if(tagName=="description")
            qDebug()<<"DESCRIPTION: "<<(description=c.text());
        else if(tagName=="language")
            qDebug()<<"LANGUAGE: "<<(language=c.text());
        else if(tagName=="copyright")
            qDebug()<<"COPYRIGHT: "<<(copyright=c.text());
        else if(tagName=="managingeditor")
            qDebug()<<"MANAGINGEDITOR: "<<(managingEditor=c.text());
        else if(tagName=="webmaster")
            qDebug()<<"WEBMASTER: "<<(webMaster=c.text());
        else if(tagName=="pubdate")//datetime
            qDebug()<<"PUBDATE: "<<(pubDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        else if(tagName=="lastBuildDate")//datetime
            qDebug()<<"LASTBUILDDATE: "<<(lastBuildDate=QDateTime::fromString(c.text().left(QString(RFC822).length()), RFC822));
        else if(tagName=="generator")
            qDebug()<<"GENERATOR: "<<(generator=c.text());
        else if(tagName=="docs")
            qDebug()<<"DOCS: "<<(docs=c.text());
        else if(tagName=="ttl")//int
            qDebug()<<"TTL: "<<(ttl=c.text().toInt());
        else if(tagName=="image")//rssimage
            qDebug()<<"IMAGE: "<<c.text();//TODO
        else if(tagName=="pics")//rsspics
            qDebug()<<"RATING: "<<c.text();//TODO
        else if(tagName=="textinput")//rssTextInput
            qDebug()<<"TEXTINPUT: "<<c.text();
        else if(tagName=="skiphours")//rssSkipHOurs
            qDebug()<<"SKIPHOURS: "<<c.text();
        else if(tagName=="skipdays")//rssskipdays
            qDebug()<<"SKIPDAYS: "<<c.text();
        else if(tagName=="category")//rsscategory
            qDebug()<<"CATEGORY: "<<c.text();
        else if(tagName=="cloud")//rsscloud
            qDebug()<<"CLOUD: "<<c.text();
	else
	{
        qWarning() << "unknown feedtagname:" << tagName;
	}
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


