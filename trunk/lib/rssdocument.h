#ifndef RSSDOCUMENT_H
#define RSSDOCUMENT_H
#include <QString>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QList>
#include <QUrl>
#include <QVariant>
#include <QImage>
#include <QDebug>

class RSSTag:public QObject
{
Q_OBJECT
public:
    RSSTag(){}
    static QString version;
    QString text;
    virtual bool empty(){return true;}
    virtual QString toHtml(){return "";}
signals:
    void updated();
};

class RSSCategory:public RSSTag
{
Q_OBJECT
public:
    RSSCategory(){}
    void setDocument(QDomElement e);
    QString content;
    QString domain;
    bool empty(){return content==""?true:false;}
    QString toHtml();
};

class RSSCloud:public RSSTag
{
//TODO: I don't really understand how the cloud works, can anybody tell me?
Q_OBJECT
public:
    RSSCloud(){}
    void setDocument(QDomElement e);
    //data members
    QString domain;
    int port;
    QString path;
    QString protocol;
};

class RSSImage:public RSSTag
{
Q_OBJECT
public:
    RSSImage(){}
    void setDocument(QDomElement e);
    QString toHtml();
    bool empty(){return url.isEmpty();}
    QImage image;
//data members
    QUrl url;
    QString title;
    QString link;
    QString description;
    int width;
    int height;
};

class RSSPICS:public RSSTag
{
Q_OBJECT
public:
    RSSPICS(){}
    void setDocument(QDomElement e);
//data members
};

class RSSTextInput:public RSSTag
{
Q_OBJECT
public:
    RSSTextInput(){}
    void setDocument(QDomElement e);
    QString toHtml();
    //data members
    QString title;
    QString description;
    QString name;
    QString link;//cgi to submit
};

class RSSSkipHours:public RSSTag
{
Q_OBJECT
public:
    RSSSkipHours(){}
    void setDocument(QDomElement e);
    bool inSkipHours();
    //data members
    QList<int> skiphours;
};

class RSSSkipDays:public RSSTag
{
Q_OBJECT
public:
    RSSSkipDays(){}
    void setDocument(QDomElement e);
    bool inSkipDays();
    //data members
    QList<int> skipDays;
};

class RSSEnclosure:public RSSTag
{
Q_OBJECT
public:
    RSSEnclosure(){}
    void setDocument(QDomElement e);
    bool empty(){return url.isEmpty();}
    QString toHtml();
    //data members
    QUrl url;
    long length;
    QString type;
};

class RSSItem:public RSSTag
{
Q_OBJECT
public:
    RSSItem(QDomElement e);
public:
        //data members
    QString title;//The title of the item.  Venice Film Festival Tries to Quit Sinking
    QUrl link;//The URL of the item.    http://nytimes.com/2004/12/07FEST.html
    QString description;//The item synopsis.  Some of the most heated chatter at the Venice Film Festival this week was about the way that the arrival of the stars at the Palazzo del Cinema was being staged.
    QString contentencoded;//content with format.
    QString author;//Email address of the author of the item. More.  oprah\@oxygen.net
    QList<RSSCategory*> categories;//Includes the item in one or more categories. More.
    QUrl comments;//URL of a page for comments relating to the item. More.  http://www.myblog.org/cgi-local/mt/mt-comments.cgi?entry_id=290
    RSSEnclosure enclosure;//Describes a media object that is attached to the item. More.
    QString guid;//A string that uniquely identifies the item. More.   http://inessential.com/2002/09/01.php#a2
    QDateTime pubDate;//Indicates when the item was published. More.    Sun, 19 May 2002 15:21:36 GMT
    QString source;//The RSS channel that the item came from. More.
};

class RSSFeed:public QObject
{
Q_OBJECT
public:
    typedef enum {Title, Description, Link, Language, Copyright, ManagingEditor, WebMaster, PubDate, LastBuildDate, Generator, Docs, Rating, TextInput}RSS;

    RSSFeed(QDomNode e);
	virtual const QVariant getProperty(RSS property) const;//get property listed in enum RSSChannelProperty
    QList<RSSItem*> getItems() const;//get all items in the document
    RSSItem& getItem(QString itemId) const;//get an item by guid
signals:
    void parseFinished(bool success);//emit when the parse of document finishes
    void updated();//emit when the document is changed
public:
    QDateTime lastUpdate;//last time updated
    int updateInterval;//how long should this be updated
private:
    //members
    QList<RSSItem*> m_items;
    //required data members
    QString title;   //The name of the channel. It's how people refer to your service. If you have an HTML website that contains the same information as your RSS file, the title of your channel should be the same as the title of your website.     GoUpstate.com News Headlines
    QUrl link;    //The URL to the HTML website corresponding to the channel.   http://www.goupstate.com/
    QString description;         //Phrase or sentence describing the channel.  The latest news from GoUpstate.com, a Spartanburg Herald-Journal Web site.
    //optional data members
    QString language;    //The language the channel is written in. This allows aggregators to group all Italian language sites, for example, on a single page. A list of allowable values for this element, as provided by Netscape, is here. You may also use values defined by the W3C.  en-us
    QString copyright;   //Copyright notice for content in the channel.    Copyright 2002, Spartanburg Herald-Journal
    QString managingEditor;  //Email address for person responsible for editorial content. geo@herald.com (George Matesky)
    QString webMaster;   //Email address for person responsible for technical issues relating to channel.  betty@herald.com (Betty Guernsey)
    QDateTime pubDate; //The publication date for the content in the channel. For example, the New York Times publishes on a daily basis, the publication date flips once every 24 hours. That's when the pubDate of the channel changes. All date-times in RSS conform to the Date and Time Specification of RFC 822, with the exception that the year may be expressed with two characters or four characters (four preferred).    Sat, 07 Sep 2002 00:00:01 GMT
    QDateTime lastBuildDate;   //The last time the content of the channel changed.   Sat, 07 Sep 2002 09:42:31 GMT
    QString generator;   //A string indicating the program used to generate the channel.   MightyInHouse Content System v2.3
    QString docs;    //A URL that points to the documentation for the format used in the RSS file. It's probably a pointer to this page. It's for people who might stumble across an RSS file on a Web server 25 years from now and wonder what it is. http://blogs.law.harvard.edu/tech/rss
    long ttl; //ttl stands for time to live. It's a number of minutes that indicates how long a channel can be cached before refreshing from the source. More info here.    <ttl>60</ttl>
    RSSImage image;   //Specifies a GIF, JPEG or PNG image that can be displayed with the channel. More info here.
    RSSPICS rating;  //The PICS rating for the channel.
    RSSTextInput textInput;   //Specifies a text input box that can be displayed with the channel. More info here.
    RSSSkipHours skipHours;   //A hint for aggregators telling them which hours they can skip. More info here.
    RSSSkipDays skipDays;    //A hint for aggregators telling them which days they can skip. More info here.
public:
    QList<RSSCategory*> category;    //Specify one or more categories that the channel belongs to. Follows the same rules as the <item>-level category element. More info. <category>Newspapers</category>
    RSSCloud cloud;   //Allows processes to register with a cloud to be notified of updates to the channel, implementing a lightweight publish-subscribe protocol for RSS feeds. More info here.    <cloud domain="rpc.sys.com" port="80" path="/RPC2" registerProcedure="pingMe" protocol="soap"/>
};

class RSSDocument: public QObject
{
Q_OBJECT
public:
    RSSDocument();
    bool setDocument(const QByteArray xmlSrc);
    QString getDocument();
    const QList<RSSFeed*> getFeedList(){ return m_feeds;};
	virtual const QString feedContentHint(int id = -1);

private:
    QDomDocument m_doc;
    QString m_version;
    QList<RSSFeed*> m_feeds;
};

#endif
