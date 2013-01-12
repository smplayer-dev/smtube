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

#ifndef YTDATAAPI_H
#define YTDATAAPI_H

#include <QObject>
#include <QDate>

class QNetworkAccessManager;
class QNetworkReply;

struct SingleResult
{
    QString desc;
    QString thumbUrl;
    QString title;
    int duration;
    QDate publishedDate;
    QString videoId;
    int viewCount;
    int favCount;
};

struct YTReply
{
    int totalResultCount;
    int thisResultCount;
    int startIndex;
    int itemsPerPage;
    QString queryString;
    QString nextUrl;
    QList<SingleResult> results;
    int replyPointer;
    QString errorString;

};

class YTDataAPI : public QObject
{
Q_OBJECT
public:

    explicit YTDataAPI(QObject *parent = 0);

    int getMost(int tab, QString searchTerm = QString());
    int getNextResults(QString url);
    void reset();
    static QString unescape(QString html);

private:

    int getXmlResponse(QString url);
    QNetworkAccessManager* manager;
    QByteArray rawReply;
    void parseXmlReply(QByteArray arr, YTReply& formattedReply);

signals:

    void finalResults(const YTReply&);

public slots:
    void gotReply(QNetworkReply* reply);

};

#endif // YTDATAAPI_H
