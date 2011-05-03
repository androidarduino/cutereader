#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QWebView>
#include "rssdocument.h"

QByteArray readFile(QString fileName = "")
{
	QByteArray content;
	if(fileName.simplified().isEmpty())
        	fileName="test.rss";

	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{	
	content=file.readAll();
	file.close();
	}
	qWarning() << content;
    return content;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	QTextCodec* codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForTr(codec);
    QWebView view;
    RSSDocument doc;
    QString fileName;
    if(argc > 0)
    doc.setDocument(readFile(argv[1]));
    else
    doc.setDocument(readFile());

    QString titleinfo, content;
    const QList<RSSFeed*> list = doc.getFeedList();
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
        titleinfo += "RSS from <a href=\"" + link + "\">" + "</a><br>";
        titleinfo+= "<a href=\"" + link + "\">" + link + "</a><br>";
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
        }

        titleinfo += "<a href= \"" + it->link.path() + "\">" + it->title+ "</a><br>";
        content += "<p>" + it->description + "</p>" + it->source;
    }

    content = titleinfo + content;
    view.setHtml(content);
    view.show();
    return app.exec();
}
