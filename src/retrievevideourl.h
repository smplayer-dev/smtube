/*  umplayer, GUI front-end for mplayer.
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

#ifndef RETRIEVEVIDEOURL_H
#define RETRIEVEVIDEOURL_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QPersistentModelIndex>

class RetrieveVideoUrl : public QObject
{
Q_OBJECT
public:
    enum Quality
    {
        FullHD = 37,
        HD = 22,
        Normal = 18,
        NormalFlv = 35,
        BelowNormalFlv = 34,
        LowFlv = 5
    };


    explicit RetrieveVideoUrl(QObject *parent = 0);
    void fetchYTVideoPage(QString videoId, QString title = QString());    
    void cancel();
    QString sanitizeForUnicodePoint(QString string);
    void htmlDecode(QString& string);

private:

    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QMap<int, QString> urlMap;
    QString id;
    QString m_title;       


signals:
    void gotUrls(const QMap<int, QString>&, QString title, QString id);
    void errorOcurred(QString, int);
    void canceled();

public slots:
    void gotVideoPage(QNetworkReply* reply);

};

#endif // RETRIEVEVIDEOURL_H
