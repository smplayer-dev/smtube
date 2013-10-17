/*  smtube, a small youtube browser.
    Copyright (C) 2012-2013 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2010 Ori Rejwan

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ytdataapi.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QDebug>
#include <QUuid>
#include "ytdialog.h"


YTDataAPI::YTDataAPI(QObject *parent) :
    QObject(parent)
{
    time = "all_time";
    country = "US";

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotReply(QNetworkReply*)));
}


int YTDataAPI::getMost(int tab, QString searchTerm)
{
    if(tab == YTDialog::Popular)
        return getXmlResponse(QString("http://gdata.youtube.com/feeds/api/standardfeeds/%1/most_popular?time=%2").arg(country).arg(time));
    else if(tab == YTDialog::Sport)
        return getXmlResponse(QString("http://gdata.youtube.com/feeds/api/standardfeeds/%1/most_popular_Sports?v=2&time=%2").arg(country).arg(time));
    else if(tab == YTDialog::Music)
        return getXmlResponse(QString("http://gdata.youtube.com/feeds/api/standardfeeds/%1/most_popular_Music?v=2&time=%2").arg(country).arg(time));
    else if(tab == YTDialog::Relevant)
        return  getXmlResponse("http://gdata.youtube.com/feeds/api/videos?v=2&start-index=1&results=25&orderby=relevance&q=" + searchTerm);
    else if(tab == YTDialog::Recent)
        return  getXmlResponse("http://gdata.youtube.com/feeds/api/videos?v=2&start-index=1&results=25&orderby=published&q=" + searchTerm);
    else if(tab == YTDialog::Viewed)
        return  getXmlResponse("http://gdata.youtube.com/feeds/api/videos?v=2&start-index=1&results=25&orderby=viewCount&q=" + searchTerm);
    else return -1;
}

int YTDataAPI::getNextResults(QString url)
{
    return getXmlResponse(url);
}

int YTDataAPI::getXmlResponse(QString url)
{
    QNetworkRequest req(url);
    QNetworkReply* r= manager->get(req);
    return (qint64)r;
}

void YTDataAPI::gotReply(QNetworkReply *reply)
{
    YTReply formattedReply;
    if(reply->error() != QNetworkReply::NoError)
    {
        formattedReply.errorString = reply->errorString();
    }
    rawReply = reply->readAll();    
    formattedReply.replyPointer = (qint64)reply;
    parseXmlReply(rawReply, formattedReply);
    reply->deleteLater();
    emit finalResults(formattedReply);
}

void YTDataAPI::parseXmlReply(QByteArray arr, YTReply &formattedReply)
{
    QString text = QUrl::fromPercentEncoding(arr);
    QDomDocument doc;
    doc.setContent(text, false);
    formattedReply.totalResultCount = doc.elementsByTagName("openSearch:totalResults").item(0).toElement().text().toInt();
    formattedReply.startIndex = doc.elementsByTagName("openSearch:startIndex").item(0).toElement().text().toInt();
    formattedReply.itemsPerPage = doc.elementsByTagName("openSearch:itemsPerPage").item(0).toElement().text().toInt();    
    QDomNodeList links = doc.elementsByTagName("link");
    for(int i = 0; i< links.count(); ++i)
    {
        if(links.item(i).toElement().attribute("rel") == "next")
        {
            formattedReply.nextUrl = unescape(links.item(i).toElement().attribute("href"));
        }
    }
    QDomNodeList entryNodes = doc.elementsByTagName("entry");
    formattedReply.thisResultCount = entryNodes.count();
    for(int i=0; i < formattedReply.thisResultCount; ++i)
    {
        QDomElement mediaElement = entryNodes.item(i).toElement().elementsByTagName("media:group").item(0).toElement();
        SingleResult res;
        res.desc = mediaElement.elementsByTagName("media:description").item(0).toElement().text();
        res.duration = mediaElement.elementsByTagName("yt:duration").item(0).toElement().attribute("seconds").toInt();
        res.thumbUrl = unescape(mediaElement.elementsByTagName("media:thumbnail").item(0).toElement().attribute("url"));
        res.title = mediaElement.elementsByTagName("media:title").item(0).toElement().text();
        QString published = entryNodes.item(i).toElement().elementsByTagName("published").item(0).toElement().text();
        published = published.left(published.indexOf("T"));
        res.publishedDate = QDate::fromString(published, "yyyy-MM-dd");
        res.videoId = entryNodes.item(i).toElement().elementsByTagName("id").item(0).toElement().text();
        res.videoId = res.videoId.mid(res.videoId.lastIndexOf('/')+1);
        if(mediaElement.elementsByTagName("yt:videoid").count() > 0)
        {
            res.videoId = mediaElement.elementsByTagName("yt:videoid").item(0).toElement().text();
        }        
        res.viewCount = entryNodes.item(i).toElement().elementsByTagName("yt:statistics").item(0).toElement().attribute("viewCount").toInt();
        formattedReply.results.append(res );
    }
}

void YTDataAPI::reset()
{
    manager->deleteLater();
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotReply(QNetworkReply*)));
}

QString YTDataAPI::unescape(QString html)
{
    html.replace("&amp;", "&");
    html.replace("&lt;", "<");
    return html.replace("&gt;", ">");
}

#include "moc_ytdataapi.cpp"
