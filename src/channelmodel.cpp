#include "channelmodel.h"
#include <unistd.h>
#include <stdlib.h>
#include <QDebug>
#include <stdio.h>
#include "rsschannel.h"

ChannelModel::ChannelModel(const QString dir)
:dataDir(dir)
{
}

ChannelModel::~ChannelModel()
{
}

bool ChannelModel::initialize()
{
	//mkdir if the dir doesn't exist already
	dataDir.makeAbsolute();
	
	if(! dataDir.exists())
		createDataDir(dataDir);

	readChannelData();
}

void ChannelModel::readChannelData()
{
	//fake data for debug
	addChannel("http://hi.baidu.com/myboymike/rss");
	addChannel("http://www.cuteqt.com/blog/?feed=rss");
	addChannel("http://news.baidu.com/n?cmd=1&class=civilnews&tn=rss&sub=0");
	/*
	channellist << 
	channellist <<
	*/
	return;
//////////////////////////////////////////////////////////////////
	//read data from datadir
	QString content;
	QFile file;
	file.setFileName(dataDir.path() + "/channels.db");
	if (file.open(QIODevice::ReadOnly))
	{
		content = file.readAll();
		//BJBJchannellist = content.split("\n");
	}
	else if( !file.exists())
	{
		file.open(QIODevice::WriteOnly);
	}
	file.close();
}

bool ChannelModel::createDataDir(const QDir& dir)
{
	QString command = "mkdir -p " + dir.path();
	qDebug()<<"Make dir:" << command;
	system(command.toLocal8Bit().constData());
}

QModelIndex ChannelModel::index(int row, int column, const QModelIndex& parent ) const
{
	QModelIndex id = createIndex(row,column, channellist[row]);
	return id;
}

QModelIndex ChannelModel::parent(const QModelIndex& index) const
{
	return QModelIndex();//no parent yet
}

int ChannelModel::rowCount(const QModelIndex&parent) const
{
	if( parent != QModelIndex())
		return 0;//no child

	//qDebug() << "Retrieve row count:"<< channellist.count();
	return channellist.count();
}

int ChannelModel::columnCount(const QModelIndex&parent ) const
{
	if( parent != QModelIndex())
		return 0;//no child
	return 1;
}

QVariant ChannelModel::data(const QModelIndex& index, int role) const
{
	if( role == Qt::DisplayRole || role == Qt::EditRole)
	{
	
		RssChannel* ch = channellist[index.row()];
		//qDebug() << "Retrieve data:" << ch->url().toString();
		return QVariant(ch->url().toString());
	}

	return QVariant();
}


RssChannel*ChannelModel::addChannel(const QString url)
{
	RssChannel* ch = new RssChannel;
	ch->setUrl(url);
	addChannel(ch);
	return ch;
}

void ChannelModel::addChannel(RssChannel* ch)
{
	QModelIndex index = createIndex(channellist.count(), 0, ch);
	channellist.append(ch);
	//qDebug() <<"ChannelModel:addChannel:" <<index<< ch->url().toString();
	emit channelAdded(ch);
	qDebug() <<"ChannelModel:emit signal channelAdded";
	emit dataChanged(index, index); 
}

Qt::ItemFlags ChannelModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsSelectable;
}

bool ChannelModel::contains(const QString url)
{
	register int i = 0;
	for( i = 0; i< channellist.count(); i++)
	{
		if( channellist[i]->url().toString() == url)
		{
			return true;
		}
	}
	return false;
}
