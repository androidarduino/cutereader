
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include "ui_whole.h"
#include "ui_rssurl.h"
#include "ui_about.h"

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
	void laterInitialize();
	void reportError(const QString)const;
	void downloadFinished();
	void connectToChannel(RssChannel*newchannel, bool silent = false, bool forceupdate = false);
	void switchPageMode(bool mode);
	const QString welcomePage();
	const RssChannel* addChannel(QString channel = "", QString a = "", bool silent = false);

private:
	void listTitles();
		void displayContent(int id = -1);
        void displayContent(const QString&);
        void debugFile(const QString& content, int id = 0);//output file content to a log file with id+uuid
	Ui::MainWindow ui;
	Ui::AddChannelDialog uiAddChannel;
	Ui::AboutDialog uiAbout;
	QDialog* dAddChannel, *dAbout;
	RssChannel* currentchannel;
	ChannelModel* channelmodel;
	QStringListModel* titlemodel;
	//QMap<QString, RssChannel*> channelmap;

private slots:
	void channelAddedToModel(const RssChannel*ch) const ;
	void titleSelected(const QModelIndex& index);
	void channelSelected(const QModelIndex& index);
	void forceRefreshChannel(const QModelIndex&index);
		void showStatus(const QString& msg)const;
signals:
};

#endif
