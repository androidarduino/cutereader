
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#ifndef WGET_H
#define WGET_H
#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class WGet:public QObject 
{
Q_OBJECT
public:
	WGet();
	~WGet(){};
	QIODevice* requestUrl(const QString url);
private:
	QNetworkAccessManager* manager;
signals:
	void networkError(const QString);
	void requestFinished();

protected slots:
	void finishReply();
	void errorReply(QNetworkReply::NetworkError);
	void replyFinished(QNetworkReply*);
};

#endif

