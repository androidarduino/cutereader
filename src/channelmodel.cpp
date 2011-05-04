
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#include "channelmodel.h"
#include <unistd.h>
#include <stdlib.h>
#include <QDebug>
#include <stdio.h>
#include "rsschannel.h"


RssChannelData::RssChannelData(const QString url, const QString a)
:rssAlias(a)
{
	setUrl(url);
}

RssChannelData::~RssChannelData()
{
}

const QString RssChannelData::alias()
{
	return rssAlias;
}

void RssChannelData::setAlias(const QString a)
{
	rssAlias = a;
	//should emit signal after calling setAlias to update view
	//emit dataChanged(index, index);
}

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
	return true;
}

void ChannelModel::readChannelData()
{
	//fake data for debug
	/*
	addChannel("http://hi.baidu.com/myboymike/rss");
	addChannel("http://www.cuteqt.com/blog/?feed=rss");
	addChannel("http://news.baidu.com/n?cmd=1&class=civilnews&tn=rss&sub=0");
	channellist << 
	channellist <<
	return;
	*/

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
	return false;
}

QModelIndex ChannelModel::index(int row, int column, const QModelIndex& parent ) const
{
	if(row >= channellist.count())
		return QModelIndex();

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
	if( index.row() >= channellist.count())
		return QVariant();

	RssChannelData* ch = channellist[index.row()];
	switch(role)
	{
	case Qt::DisplayRole://return alias instead of url
		//qDebug() << "Retrieve data:" << index.row() << ch->url().toString();
		if(! ch->alias().isEmpty())
			return QVariant(ch->alias());
		else
			return QVariant(ch->url().toString());
	break;
	case Qt::EditRole:
		return QVariant(ch->url().toString());
	break;
	}

	return QVariant();
}


RssChannelData*ChannelModel::addChannel(const QString url, const QString a)
{
	RssChannelData* ch = new RssChannelData(url, a);
	ch->setUrl(url);
	addChannel(ch);
	return ch;
}

void ChannelModel::addChannel(RssChannelData* ch)
{
	beginInsertRows(QModelIndex(), channellist.count(), channellist.count());
	//QModelIndex index = createIndex(channellist.count(), 0, ch);
	qDebug() <<"ChannelModel:addChannel:" << ch->url().toString();
	channellist.append(ch);
	emit channelAdded(ch);
	endInsertRows();
}

Qt::ItemFlags ChannelModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
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

