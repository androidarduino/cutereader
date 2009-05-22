#include "mainwin.h"
#include "rsschannel.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>

MainWin::MainWin(QWidget*p, Qt::WindowFlags f):QMainWindow(p,f)
{
	ui.setupUi(this);
	dAddChannel = new QDialog(this);
	uiAddChannel.setupUi(dAddChannel);
	
	connect(this, SIGNAL(channelChanged(RssChannel*)), this, SLOT(doChannelChanged(RssChannel*)));
	connect(ui.action_Add_Channel, SIGNAL(activated()), this, SLOT(addChannel()));
	connect(ui.action_About, SIGNAL(activated()), this, SLOT(about()));
}

MainWin::~MainWin()
{
}


void MainWin::displayContent(int id)
{
	QString content = currentchannel->getContent(id);
	ui.articleView->setHtml(content);
//	qDebug() <<"content get:" << content;
}

void MainWin::listTitles()
{
	QMap<int, QString> map = currentchannel->getTitles();
	qDebug() << "Get Titles:" << map.count();
}

void MainWin::about()
{
}

void MainWin::addChannel(QString url)
{
	if(url.isEmpty())
	{
		//url+="http://news.baidu.com/n?cmd=1&class=civilnews&tn=rss&sub=0";
		//url+="http://www.cuteqt.com/?feed=rss";
		//url+="http://www.un.org/chinese/News/RSS/rss_chinese.asp";
                url+="http://hi.baidu.com/myboymike/rss";
	}	
	uiAddChannel.leRssLink->setText(url);
	if( QDialog::Accepted == dAddChannel->exec())
	{
		QString newurl = uiAddChannel.leRssLink->text();
		//existing channel, show warning and reject change
		if(channelmap.contains(newurl))
		{
			QMessageBox::information(this, "Channel Exists!", "Channel duplicates with existing one.");
			return;
		}
		qDebug() << "Add channel to map:" << newurl;
		//add this channel to channel map
		RssChannel* channel = new RssChannel;	
		channelmap[newurl] = channel;
		channel->setUrl(newurl);
		connect(channel, SIGNAL(doneDownload()), this, SLOT(downloadFinished()));
		connect(channel, SIGNAL(doneParse()), this, SLOT(parseFinished()));
		currentchannel = channel;
		emit channelChanged(channel);
	}
	else//cancel
	{
	}

	return;
}

void MainWin::doChannelChanged(RssChannel*channel)
{
	QMessageBox msgBox(this);
	msgBox.setText("Connect to channel?");
 	msgBox.setInformativeText("Do you wish to connect to this channel now?");
 	msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
 	msgBox.setDefaultButton(QMessageBox::Ok);
 	int ret = msgBox.exec();
	if(ret == QMessageBox::Ok)
	{
		qDebug() << "connect to url" << channel->url();
		channel->connectChannel();
	}
}

void MainWin::parseFinished()
{
	qDebug() << "Parse finished....";
	listTitles();
	displayContent(0);
}

void MainWin::downloadFinished()
{
	qDebug() << "Download finished....";
}
