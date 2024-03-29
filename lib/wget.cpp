
/*
	Author: shiroki@www.cuteqt.com
	License: GPLv2
*/

#include <QNetworkAccessManager>
#include <QDebug>
#include <QFile>
#include "wget.h"

WGet::WGet()
{
	manager = new QNetworkAccessManager;
	//connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));	
}

QIODevice* WGet::requestUrl(const QString url)
{
	QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(url)));
	//connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorReply(QNetworkReply::NetworkError)));
	connect(reply, SIGNAL(finished()), this, SLOT(finishReply()));
	return reply;
}

void WGet::replyFinished(QNetworkReply* reply)
{
	qDebug() << "Request Finished!" << reply;
}

void WGet::finishReply()
{
	qDebug() << "Finished!";
	QObject* obj = sender();
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(obj);

	if( reply->error())
	{
		emit networkError(parseError(reply->error()));
		return;
	}

	/*
	QFile temp("tmp.xml");
	temp.open(QIODevice::WriteOnly);
	temp.write(reply->readAll());
	temp.close();
	*/

	emit requestFinished();
}

void WGet::errorReply(QNetworkReply::NetworkError error)
{
	QString errstr = parseError(error);
	qDebug() << "Error!" << error;
	emit networkError(errstr);
}

const QString WGet::parseError(QNetworkReply::NetworkError error)
{
	QString errstr = tr("No Error!");
        switch(error)
	{
		case QNetworkReply::ConnectionRefusedError:
		errstr = tr("Connection refused!");
		break;
		case QNetworkReply::HostNotFoundError:
		errstr = tr("Host not found!");
		break;
		case QNetworkReply::RemoteHostClosedError:
		errstr = tr("Remote host closed!");
		break;
		case QNetworkReply::TimeoutError:
		errstr = tr("Timeout!");
		break;
		case QNetworkReply::ContentAccessDenied:
		errstr = tr("Content access denied!");
		break;
		case QNetworkReply::ProtocolFailure:
		errstr = tr("Protocol failure!");
		break;
		case QNetworkReply::ContentNotFoundError:
		errstr = tr("Content not found!");
		break;
		default:
		break;
	}
	qDebug() << "error" << errstr;
	return errstr;
}
