
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QDialog>
#include <QMessageBox>
#include <QStringListModel>
#include "mainwin.h"
#include "rsschannel.h"
#include "mainwin.h"
#include "rsschannel.h"
#include "channelmodel.h"
#include "rssdocument.h"

MainWin::MainWin(QWidget*p, Qt::WindowFlags f):QMainWindow(p,f)
{
	dAbout = NULL;
	channelmodel = new ChannelModel(QDir::homePath() +"/.cutereader/");
        //bug01:should connect before adding channel, otherwise the signals are emitted too early!
	connect(channelmodel, SIGNAL(channelAdded(const RssChannel*)), this, SLOT(channelAddedToModel(const RssChannel*)));
	channelmodel->initialize();

	titlemodel = new QStringListModel;
	ui.setupUi(this);
	ui.channelTree->setModel(channelmodel);
	//ui.channelTree->setAlternatingRowColors(true);
	connect(ui.channelTree, SIGNAL(activated(const QModelIndex&)), this, SLOT(channelSelected(const QModelIndex&)));

	ui.titleList->setModel(titlemodel);
	ui.titleList->setAlternatingRowColors(true);
        connect(ui.titleList, SIGNAL(clicked(const QModelIndex&)), this, SLOT(titleSelected(const QModelIndex&)));

	dAddChannel = new QDialog(this);
	uiAddChannel.setupUi(dAddChannel);
	
        connect(ui.action_Add_Channel, SIGNAL(triggered()), this, SLOT(addChannel()));
        connect(ui.action_About, SIGNAL(triggered()), this, SLOT(about()));

	QTimer::singleShot(0, this, SLOT(laterInitialize()));
}

void MainWin::laterInitialize()
{
	//fake data for debug
	addChannel("", "EmptyChannel", true);
        addChannel("http://labs.trolltech.com/blogs/feed/","TrolltechLabs",true);
        addChannel("http://www.cuteqt.com/blog/?feed=rss2", "CuteQtBlogRss2", true);
	addChannel("http://hi.baidu.com/myboymike/rss", "MyBoyMike", true);
	addChannel("http://feed.feedsky.com/CuteQt", "CuteQt", true);
        displayContent(welcomePage());
	

}
const QString MainWin::welcomePage()
{
    QFile file(":welcome");
    if( !file.open(QIODevice::ReadOnly))
        return "";

    QString page = file.readAll();
    file.close();
    return page;
}

MainWin::~MainWin()
{
}


void MainWin::displayContent(const QString& content)
{
    ui.articleView->setHtml( content);
}

void MainWin::displayContent(int id)
{
    QString content;
#ifdef RSS_DOCUMENT_SUPPORT
    content = currentchannel->feedContent();
#else
        QString channelinfo, titleinfo, titlecontent;

	qWarning("display content:%d", id);
	channelinfo = "<p><a href=" + currentchannel->getChannelLink() + ">" + currentchannel->getChannelName() + "</a></p>";
	titleinfo = "<p><a href=" + currentchannel->getTitleLink(id) + ">" + currentchannel->getTitleName(id) + "</a></p>";
	titlecontent = currentchannel->getTitleContent(id);
	content = channelinfo + titleinfo + titlecontent;
#endif
        displayContent(content);
        debugFile(content, 100+id);
        debugFile(currentchannel->getRawData(), 200+id);
        //qDebug() <<"content get:" << content;
}


void MainWin::debugFile(const QString& content, int id)
{
	QFile file;
	QTextStream st(&file);
	QString filename = QString::number(id);
	
	file.setFileName(filename);
	file.open(QIODevice::WriteOnly| QIODevice::Append);
	st << content;
	file.close();
}

void MainWin::listTitles()
{
#ifndef RSS_DOCUMENT_SUPPORT
    QStringList titlelist = currentchannel->getTitles();
	titlemodel->setStringList(titlelist);
#endif
}

void MainWin::about()
{
	if( !dAbout )
	{
		dAbout = new QDialog(this);
		uiAbout.setupUi(dAbout);
	}
	dAbout->exec();
}

void MainWin::addChannel(QString url, QString alias, bool silent)
{
	if(url.isEmpty())
	{
		if( !silent)
		{
		url+="http://www.cuteqt.com/blog/?feed=rss2";
		alias = "CuteQtFeedList";
		}
	}	

	uiAddChannel.leRssLink->setText(url);
	uiAddChannel.leRssAlias->setText(alias);
	//if not silent mode, receive url from user
	if( silent == false)//default false
	{
		if( QDialog::Accepted != dAddChannel->exec())
		{
			return;
		}
		QString newurl = uiAddChannel.leRssLink->text();
		//existing channel, show warning and reject change
		if(channelmodel->contains(newurl))
		{
                        QMessageBox::information(this, tr("Channel Exists Already!"), tr("No need to add."));
			return;
		}
		url = newurl;
		alias = uiAddChannel.leRssAlias->text();
	}

	qDebug()<< "MainWin::AddChannel" << url << alias;
	RssChannel* channel = channelmodel->addChannel(url, alias);	
	if( silent == false)
	{
		//connectToChannel(channel, silent);
	}
	return;
}

void MainWin::connectToChannel(RssChannel*channel, bool silent)
{
        showStatus(tr("Connecting to:") + channel->url().toString());
	if( silent == false)
	{
		QMessageBox msgBox(this);
		msgBox.setWindowTitle(tr("Connect to channel?"));
	 	msgBox.setInformativeText(tr("Do you wish to connect to this channel now?"));
 		msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
 		msgBox.setDefaultButton(QMessageBox::Ok);
 		int ret = msgBox.exec();
		if(ret != QMessageBox::Ok)
		{
			return;
		}
	}
	qDebug() << "connect to url" << channel->url();
	currentchannel = channel;
        channel->connectChannel();
}

void MainWin::parseFinished()
{
        showStatus(tr("Content parsing finished...."));
	listTitles();
	displayContent(0);
}

void MainWin::downloadFinished()
{
        showStatus(tr("Download finished...."));
}

void MainWin::titleSelected(const QModelIndex& index)
{
	qDebug()<< "Title:" << index.row() <<"selected...";
	displayContent(index.row());
}

//user clicked channel , please connect to it!
void MainWin::channelSelected(const QModelIndex& index)
{
	qDebug() <<"Channel selected: connect to it!";
	RssChannel* ch = static_cast<RssChannel*> (index.internalPointer());
	//currentchannel = ch;
	connectToChannel(ch, true);//silently
}

void MainWin::channelAddedToModel(const RssChannel*ch) const
{
	//qDebug() <<"Connect slots to channel signals!!";
	connect(ch, SIGNAL(doneDownload()), this, SLOT(downloadFinished()));
	connect(ch, SIGNAL(doneParse()), this, SLOT(parseFinished()));
	connect(ch, SIGNAL(networkError(const QString)), this, SLOT(reportError(const QString)));
}

void MainWin::reportError(const QString error) const
{
        showStatus(tr("Network Error: ") + error);
}

void MainWin::showStatus(const QString& msg) const
{
    ui.statusbar->showMessage(msg);
}

