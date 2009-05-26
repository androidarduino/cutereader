#include <QtCore>
#include <QtNetwork>
#include <iostream>

#include "httpget.h"

using namespace std;

HttpGet::HttpGet(QObject *parent)
    : QObject(parent)
{
    connect(&http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    connect(&http, SIGNAL(requestFinished(int,bool)), this, SLOT(httpReqDone(int,bool)));
}

bool HttpGet::getFile(const QUrl &url,QBuffer *buf)
{
    if (!url.isValid()) {
        cerr << "Error: Invalid URL" << endl;
        return false;
    }

    if (url.scheme() != "http") {
        cerr << "Error: URL must start with 'http:'" << endl;
        return false;
    }

    if (url.path().isEmpty()) {
        cerr << "Error: URL has no path" << endl;
        return false;
    }

    QList<QPair<QByteArray, QByteArray> > querylist;
    querylist=url.encodedQueryItems();
    if(querylist.size()!=0) 
    {
        QByteArray querydata; 
        querydata.append(querylist[0].first);
        querydata.append('=');
        querydata.append(querylist[0].second);
        for (int i = 1; i < querylist.size(); ++i) {
            querydata.append('&');
            querydata.append(querylist[i].first);
            querydata.append('=');
            querydata.append(querylist[i].second);
        }
        QHttpRequestHeader header("POST", url.path());
        header.setValue("Content-Type", "application/x-www-form-urlencoded");
        header.setValue("Host", url.host());
        http.setHost(url.host());
        //http.request(header,url.encodedFragment(),buf);
        http_request_id=http.request(header,querydata,buf);
        http.close();
    }
    else
    {
        http.setHost(url.host(), url.port(80));
        http.get(url.path(), buf);
        http_request_id=0;
        http.close();
    }

    return true;
}

void HttpGet::httpReqDone(int id,bool error)
{
    if (error) {
        cerr << "id:"<<id<<"Error: " << qPrintable(http.errorString()) << endl;
	emit httpError(http.errorString());
    } 
    if(id==http_request_id)
    {
        emit done();
        cout<<"id is"<<id<<"\n";
        cout<<"hello request done\n";
    }
}

void HttpGet::httpDone(bool error)
{
    if (error) {
        cerr << "Error: " << qPrintable(http.errorString()) << endl;
	emit httpError(http.errorString());
    } 
    if(http_request_id==0){
        cout<<"hello http::get() done\n";
        emit done();
    }
}
