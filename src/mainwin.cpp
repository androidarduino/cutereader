#include "mainwin.h"
#include "rsschannel.h"
#include "channelmodel.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QStringListModel>

MainWin::MainWin(QWidget*p, Qt::WindowFlags f):QMainWindow(p,f)
{
	channelmodel = new ChannelModel(QDir::homePath() +"/.cutereader/");
	//bug01:should connect before add channel, otherwise the signals are emitted too early!
	connect(channelmodel, SIGNAL(channelAdded(const RssChannel*)), this, SLOT(channelAddedToModel(const RssChannel*)));
	channelmodel->initialize();

	titlemodel = new QStringListModel;
	ui.setupUi(this);
	ui.channelTree->setModel(channelmodel);
	connect(ui.channelTree, SIGNAL(activated(const QModelIndex&)), this, SLOT(channelSelected(const QModelIndex&)));

	ui.titleList->setModel(titlemodel);
	connect(ui.titleList, SIGNAL(clicked(const QModelIndex&)), this, SLOT(titleSelected(const QModelIndex&)));

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
	QStringList titlelist = currentchannel->getTitles();
	titlemodel->setStringList(titlelist);
	qDebug() << "List titles!!";
}

void MainWin::about()
{
	QMessageBox::information(this, "About CuteReader","<i>Author: shiroki, zhu @www.cuteqt.com</i>");
}

void MainWin::addChannel(QString url)
{
	if(url.isEmpty())
	{
		//url+="http://news.baidu.com/n?cmd=1&class=civilnews&tn=rss&sub=0";
		//url+="http://www.cuteqt.com/?feed=rss";
		url+="http://hi.baidu.com/myboymike/rss";
	}	
	uiAddChannel.leRssLink->setText(url);
	if( QDialog::Accepted == dAddChannel->exec())
	{
		QString newurl = uiAddChannel.leRssLink->text();
		//existing channel, show warning and reject change
		if(channelmodel->contains(newurl))
		{
			QMessageBox::information(this, "Channel Exists!", "Channel duplicates with existing one.");
			return;
		}
		qDebug() << "Add channel to map:" << newurl;
		//add this channel to channel map
		RssChannel* channel = channelmodel->addChannel(newurl);	
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
	currentchannel = ch;
	ch->connectChannel();
}

void MainWin::channelAddedToModel(const RssChannel*ch) const
{
	qDebug() <<"Connect slots to channel signals!!";
	connect(ch, SIGNAL(doneDownload()), this, SLOT(downloadFinished()));
	connect(ch, SIGNAL(doneParse()), this, SLOT(parseFinished()));
}