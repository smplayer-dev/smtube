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

#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QQueue>
#include <QFile>
#include <QPair>
#include <QTime>
#include <QNetworkReply>


class QNetworkAccessManager;

class DownloadFile : public QObject
{
        Q_OBJECT
public:
        explicit DownloadFile(QString url, QFile* fileToDownload, QObject *parent = 0);
        qint64 fileSize() { return totalSize; }
        void cancel();
        QString fileName() { return file->fileName();}
private:
        qint64 totalSize;
        qint64 completed;
        qint64 speed;
        QString url;
        QQueue< QPair<QTime, qint64> > lastReceivedBytes;
        QFile* file;
        QNetworkAccessManager* manager;
        QNetworkReply* reply;


        void updateFooterText();
        void getRequest(QString url);
        void getRequest(QUrl url);





signals:
        void errorOcurred(int);
        void downloadStatus(QString status);
        void downloadFinished(bool error);
        void progress(int percent, qint64 size);

public slots:
        void downloaded(qint64 bytesReceived, qint64 total);
        void gotMetaData();
        void finished(QNetworkReply* reply);
        void emitError(QNetworkReply::NetworkError);

};



#endif // DOWNLOADFILE_H
