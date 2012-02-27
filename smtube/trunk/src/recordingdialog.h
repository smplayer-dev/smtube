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

#ifndef RECORDINGDIALOG_H
#define RECORDINGDIALOG_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include <QBrush>
#include "myicon.h"

class QListWidget;
class QPushButton;
class QNetworkAccessManager;
class DownloadFile;
class RetrieveVideoUrl;
class QListWidgetItem;


class DownloadData
{
public :
        DownloadData()
        {
            fileSize = -1;
            buttonState = MyIcon::Normal;
            downloadProgressPercent = 0;
            videoDuration = 0;
        }
        enum State
        {
            Progressing,
            Canceled,
            Completed,
            Error
        };
        State downloadState;
        MyIcon::Mode buttonState;
        QString downloadProgress;
        int downloadProgressPercent;
        QDateTime completionTime;
        qint64 fileSize;
        QString videoId;
        int videoDuration;
        QString title;
        QRect buttonRect;
        QString filePath;


};

Q_DECLARE_METATYPE(DownloadData*)

class RecordingDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public :
        RecordingDelegate(QObject* parent = 0);
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void setWidth(int w) { width = w; }

private:
        MyIcon playIcon;
        MyIcon retryIcon;
        MyIcon cancelIcon;
        MyIcon getIcon(QPixmap pix);
        int width;
        QBrush selectedBrush;
};

class RecordingDialog : public QWidget
{
Q_OBJECT
public:    
    enum ItemDataRole
    {
        DownloadDataRole = Qt::UserRole + 1,        
        emitDataChangedRole // hack to make sure DataChanged signal is emitted
    };

    RecordingDialog(QWidget *parent = 0);    
    ~RecordingDialog();

    void downloadVideoId(QString videoId, QString title, double duration = 0);
    void download(QString url, QString title, QString id, double duration);
    QString getUniqueFileName(QString name);
    bool eventFilter(QObject *watched, QEvent *event);

    void setRecordingsDirectory(const QString & dir) { recordings_directory = dir; }
    QString recordingsDirectory() { return recordings_directory; }

    void setRecordingQuality(int q) { recording_quality = q; }
    int recordingQuality() { return recording_quality; }

    void setRecordingFormat(int f) { recording_format = f; }
    int recordingFormat() { return recording_format; }

private:
    QPushButton* clearListButton;
    QPushButton* openFolderButton;

    QListWidget* downloadList;
    QListWidgetItem* pressedItemLeftButton;
    QListWidgetItem* pressedItemRightButton;

    QMap<DownloadFile*, QListWidgetItem*> itemDownloadMap;
    QMap<RetrieveVideoUrl*, QListWidgetItem*> itemRVUMap;

    void removeDFileFromMap(DownloadFile* dfile);
    void updateWindowTitle();
    QList<QListWidgetItem*> itemsMarkedForRemoval;

protected:
    void resizeEvent(QResizeEvent *r);

signals:
    void playFile(QString path);

public slots:
    void oneDownloadFinished(bool error);
    void oneDownloadProgressStatus(QString status);
    void oneDownloadError(int);
    void oneDownloadProgress(int percent, qint64 total );
    void cancelDownload(QListWidgetItem* item);
    void retryDownload(QListWidgetItem* item);
    void playDownload(QListWidgetItem* item);
    void urlToDownload(QMap<int,QString>,QString);
    void fetchUrlError(QString title, int error);
    void clearList();
    void openFolder();
    void updateClearListButton();
    void videoDoubleClicked(QListWidgetItem* item);
    void saveList();
    void loadList();
    void recordVideo(QMap<int,QString> qualityMap, QString title, QString id);

protected:
    QString recordings_directory;
    int recording_quality;
    int recording_format;
};

#endif // RECORDINGDIALOG_H
