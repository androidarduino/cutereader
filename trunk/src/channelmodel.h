
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#ifndef CHANNEL_MODEL_H
#define CHANNEL_MODEL_H
#include <QAbstractItemModel>
#include <QDir>
#include <QStringList>
#include <QList>
#include "rsschannel.h"

//we should store more than rsschannel, for example, we need the alias to show in the view!
class RssChannelData: public RssChannel
{
public:
	RssChannelData(const QString url, const QString a = "");
	~RssChannelData();
	const QString alias();
	void setAlias(const QString newalias);
private:
	QString rssAlias;
};

class ChannelModel:public QAbstractItemModel
{
Q_OBJECT
public:
	ChannelModel(QString datadir);//dir to save channel data
	~ChannelModel();
	RssChannelData* addChannel(const QString url, const QString alias = "");
	bool initialize();
	bool contains(const QString url);
signals:
	void channelAdded(const RssChannel*);
	//void channelAdded(const RssChannel*);
protected:
	void addChannel(RssChannelData*ch);
	void readChannelData();
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& index) const;
	int rowCount(const QModelIndex&parent = QModelIndex()) const;
	int columnCount(const QModelIndex&parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
/*
	hasChildren();
	setData();
	flags();
*/
private:
	QDir dataDir;
	QList<RssChannelData*> channellist;
	bool createDataDir(const QDir& dir);
};


#endif
