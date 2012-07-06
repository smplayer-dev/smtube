/*  smtube, a small youtube browser.
    Copyright (C) 2012 Ricardo Villalba <rvm@users.sourceforge.net>
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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTemporaryFile>
#include <QDebug>
#include "downloadfile.h"

#define MOD_TIME(x) ((x+86400000)%86400000)

DownloadFile::DownloadFile(QString site, QFile *fileToDownload, QObject *parent):
QObject(parent), speed(0)
{
        url = site;
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
        file = fileToDownload;
        getRequest(url);

}

void DownloadFile::getRequest(QString url)
{
        QUrl realUrl = QUrl::fromEncoded(url.toAscii(), QUrl::StrictMode);
        getRequest(realUrl);
}


void DownloadFile::getRequest( QUrl url )
{
        QNetworkRequest req(url);
        if(!file->isOpen())
        {
                file->open(QFile::ReadWrite);
        }
        reply = manager->get(req);
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloaded(qint64,qint64)));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(emitError(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(metaDataChanged()), this, SLOT(gotMetaData()));

}

void DownloadFile::downloaded(qint64 bytesReceived, qint64 total)
{
        static unsigned int count = 0;
        count++;

        if (count % 50 == 0) 
        {
            totalSize = total;
            completed = bytesReceived;
            QTime current = QTime::currentTime();
            QPair<QTime, qint64> head = lastReceivedBytes.head();
            if(MOD_TIME(lastReceivedBytes.last().first.msecsTo(current)) >= 1000)
            {
                lastReceivedBytes.enqueue(qMakePair(current, bytesReceived));
                speed = (bytesReceived - head.second)*1000/ MOD_TIME(head.first.msecsTo(current));
            }
            if(MOD_TIME(lastReceivedBytes.head().first.msecsTo(current)) >= 5000)
            {
                lastReceivedBytes.dequeue();
            }
            emit progress(qRound(bytesReceived*100.0/total), total);
            file->write(static_cast<QNetworkReply*>(sender())->readAll());

            updateFooterText();
            count = 0;
        }

        if(bytesReceived == total)
        {
                if(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl().isEmpty())
                {
                        file->close();
                        emit downloadFinished(reply->error() != QNetworkReply::NoError);
                        deleteLater();
                }
        }
}

void DownloadFile::updateFooterText()
{
        qint64 remainingTime = -1;
        QString remainingTimeString;
        if(speed > 0 )
                remainingTime = (totalSize - completed)/speed + 1 ;
        if(remainingTime == -1)
                remainingTimeString = tr("unknown time remaining") + " - ";
        else
        {
                QString rem;
                bool flag = false; // if hour is there but not minutes
                if(remainingTime > 3600)
                {
                        rem += tr("%1 hours").arg(remainingTime / 3600) + ", ";
                        remainingTime = remainingTime % 3600;
                        flag = true;
                }
                if(remainingTime > 60 || flag)
                {
                        rem += tr("%1 minutes").arg(remainingTime / 60 /*, 2, 10, QChar('0')*/) + ", ";
                        remainingTime = remainingTime % 60;
                }
                rem += tr("%1 seconds").arg(remainingTime /*, 2, 10, QChar('0')*/);
                remainingTimeString = tr("%1 remaining").arg(rem) + " - ";
        }

        if(totalSize > 1048576 )
        {
                remainingTimeString += tr("%1 of %2 MB")
                                          .arg(completed / (qreal)1048576, 0, 'f', 1)
                                          .arg(totalSize / (qreal)1048576, 0, 'f', 1);
        }
        else if(totalSize > 1024 )
        {
                remainingTimeString += tr("%1 of %2 KB")
                                          .arg(completed / (qreal)1024, 0, 'f', 1)
                                          .arg(totalSize / (qreal)1024, 0, 'f', 1);
        }
        else if(totalSize >= 0)
        {
                remainingTimeString += tr("%1 of %2 bytes").arg(completed).arg(totalSize);
        }
        else
        {
                remainingTimeString += tr("%1 of unknown size").arg(completed);
        }
        remainingTimeString += " (";
        if(speed > 1048576 )
        {
                remainingTimeString += tr("%1 MB/sec").arg(speed/ (qreal)1048576, 0, 'f', 1);
        }
        else if(totalSize > 1024 )
        {
                remainingTimeString += tr("%1 KB/sec").arg(speed/ (qreal)1024, 0, 'f', 1);
        }
        else
        {
                remainingTimeString += tr("%1 bytes/sec").arg(speed);
        }
        remainingTimeString += ")";
        emit downloadStatus(remainingTimeString);
}

void DownloadFile::gotMetaData()
{
        if(reply->header(QNetworkRequest::ContentTypeHeader).isValid())
        {
                if( lastReceivedBytes.isEmpty())
                        lastReceivedBytes.enqueue(qMakePair(QTime::currentTime(), 0LL));
        }
}

void DownloadFile::finished(QNetworkReply *rep)
{
        qDebug() << reply->error() << reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if(reply->error() != QNetworkReply::NoError  && reply->error() != QNetworkReply::ContentOperationNotPermittedError ) return;
        file->close();
        QUrl url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if(!url.isEmpty() && url != reply->request().url() )
        {
                reply->deleteLater();
                reply = 0;
                getRequest(url);
        }
        else
        {
                reply->deleteLater();
                reply = 0;

        }
}

void DownloadFile::cancel()
{
        reply->disconnect(this, SLOT(downloaded(qint64,qint64)));
        reply->disconnect(this, SLOT(gotMetaData()));
        reply->abort();
        if(file->isOpen()) file->close();
        file->remove();
}

void DownloadFile::emitError(QNetworkReply::NetworkError error)
{
        if(error != QNetworkReply::ContentOperationNotPermittedError)
        {
                file->close();
                emit errorOcurred((int)error);
        }
        /*
        else
                qDebug() << "Hello Darling";
        */
}

#include "moc_downloadfile.cpp"
