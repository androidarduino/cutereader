#include <QMainWindow>
#include "ui_whole.h"
#include "ui_rssurl.h"

class RssChannel;
class MainWin:public QMainWindow
{
Q_OBJECT
public:
	MainWin(QWidget*p = NULL, Qt::WindowFlags f = 0);
	~MainWin();
protected slots:
	void displayContent(int id);
	void listTitles();
	void about();
	void parseFinished();
	void downloadFinished();
	void addChannel(QString channel = "");
	void doChannelChanged(RssChannel*newchannel);

private:
	Ui::MainWindow ui;
	QDialog* dAddChannel;
	Ui::AddChannelDialog uiAddChannel;
	QMap<QString,RssChannel*> channelmap;
	RssChannel* currentchannel;

signals:
	void channelChanged(RssChannel*);
};
