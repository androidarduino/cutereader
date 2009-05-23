#ifndef CHANNEL_MODEL_H
#define CHANNEL_MODEL_H
#include <QAbstractItemModel>
#include <QDir>
#include <QStringList>
#include <QList>

class RssChannel;
class ChannelModel:public QAbstractItemModel
{
Q_OBJECT
public:
	ChannelModel(QString datadir);//dir to save channel data
	~ChannelModel();
	RssChannel* addChannel(const QString url);
	bool initialize();
	bool contains(const QString url);
signals:
	void channelAdded(const RssChannel*);
	//void channelAdded(const RssChannel*);
protected:
	void addChannel(RssChannel*ch);
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
	QList<RssChannel*> channellist;
	bool createDataDir(const QDir& dir);
};


#endif
