#include <QMainWindow>
#include "ui_whole.h"
#include "ui_rssurl.h"

class RssChannel;
class TreeModel;
class ChannelModel;
class QStringListModel;
class MainWin:public QMainWindow
{
Q_OBJECT
public:
	MainWin(QWidget*p = NULL, Qt::WindowFlags f = 0);
	~MainWin();
protected slots:
	void about();
	void parseFinished();
	void downloadFinished();
	void doChannelChanged(RssChannel*newchannel);
	void addChannel(QString channel = "");

private:
	void listTitles();
	void displayContent(int id);
	Ui::MainWindow ui;
	QDialog* dAddChannel;
	Ui::AddChannelDialog uiAddChannel;
	RssChannel* currentchannel;
	ChannelModel* channelmodel;
	QStringListModel* titlemodel;
	//QMap<QString, RssChannel*> channelmap;

private slots:
	void channelAddedToModel(const RssChannel*ch) const ;
	void titleSelected(const QModelIndex& index);
	void channelSelected(const QModelIndex& index);
signals:
	void channelChanged(RssChannel*);
};
