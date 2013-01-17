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

#include <QPushButton>
#include <QListWidget>
#include <QBoxLayout>
#include <QUrl>
#include <QPainter>
#include <QApplication>
#include <QStyleOptionProgressBarV2>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QEvent>
#include <QRegExp>
#include <QResizeEvent>
#include <QPersistentModelIndex>
#include <QAbstractItemModel>
#include <QDesktopServices>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include "recordingdialog.h"
#include "downloadfile.h"
#include "retrievevideourl.h"
/* #include "helper.h" */
#include "myborder.h"

RecordingDelegate::RecordingDelegate(QObject *parent)
    :QStyledItemDelegate(parent), selectedBrush(QPixmap(":/Control/bg-download-active.png"))
{
    QPixmap playPix(":/Control/bt-download-play.png");
    QPixmap cancelPix(":/Control/bt-download.png");
    QPixmap retryPix(":/Control/bt-download-retry.png");
    playIcon = getIcon(playPix);
    cancelIcon = getIcon(cancelPix);
    retryIcon = getIcon(retryPix);    
}

void RecordingDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    QRect rect = option.rect;    
    if(index.row() % 2 == 0)
    {
        painter->fillRect(rect, QColor("#f0f6ff"));
    }
    else
    {
        painter->fillRect(rect, Qt::white);
    }
    painter->translate(rect.topLeft());
    QFont font = painter->font();
    font.setPixelSize(14);
    painter->setFont(font);
    painter->setPen(Qt::black);
    DownloadData* dd = index.data(RecordingDialog::DownloadDataRole).value<DownloadData*>();
    if(option.state & QStyle::State_Selected)
    {        
        painter->fillRect(QRect(QPoint(), rect.size()), selectedBrush);
        painter->setPen(Qt::white);
    }
    QPixmap currentPix;    
    if(dd->downloadState == DownloadData::Progressing)
    {
        QRect headerTextRect(13, 7, rect.width() - 26, rect.height());
        painter->drawText(headerTextRect, Qt::AlignLeft | Qt::AlignTop, dd->title, &headerTextRect);
        currentPix = cancelIcon.pixmap(dd->buttonState, MyIcon::Off );
        QStyleOptionProgressBarV2 optionProgress;
        optionProgress.rect = QRect( 11, headerTextRect.bottom() + 6, rect.width() - 11 - 13 - 9 - currentPix.width(), 16 );
        optionProgress.maximum = 100;
        optionProgress.minimum = 0;
        optionProgress.progress = 48;
        optionProgress.textVisible = false;
        optionProgress.orientation = Qt::Horizontal;
        optionProgress.progress = dd->downloadProgressPercent;
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &optionProgress, painter, 0);                
        QRect buttonRect(optionProgress.rect.right() + 9, optionProgress.rect.top(), currentPix.width(), currentPix.height());
        if(dd->buttonRect != buttonRect) dd->buttonRect = buttonRect;
        painter->drawPixmap(buttonRect, currentPix );
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(QRect(13, optionProgress.rect.bottom() + 3, rect.width(), rect.height()), Qt::AlignLeft | Qt::AlignTop, dd->downloadProgress);
    }
    else if(dd->downloadState == DownloadData::Canceled || dd->downloadState == DownloadData::Error)
    {        
        QDateTime time = dd->completionTime;
        QString currentDateString;
        if(time.date() == QDate::currentDate())
            currentDateString = time.toString("hh:mm AP");
        else
            currentDateString = time.toString("MMM dd");
        QRect headerTextRect(13, 7, rect.width() - 26, rect.height());
        int wid = painter->fontMetrics().width(currentDateString);
        headerTextRect.adjust(0,0,-wid, 0);
        painter->drawText(headerTextRect, Qt::AlignLeft | Qt::AlignTop, dd->title, &headerTextRect);
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(QRect(rect.width() - 13 - wid , headerTextRect.top(), wid,
                                headerTextRect.height()), Qt::AlignVCenter | Qt::AlignRight, currentDateString );
        painter->drawText(QRect(headerTextRect.left(), headerTextRect.bottom() + 5, rect.width(), rect.height()),
                          Qt::AlignLeft | Qt::AlignTop, dd->downloadState == DownloadData::Canceled ? tr("Canceled") : tr("Error"));
        currentPix = retryIcon.pixmap(dd->buttonState, MyIcon::Off );
        QRect buttonRect(rect.width() - 13 - currentPix.width(), headerTextRect.bottom() + 5, currentPix.width(), currentPix.height());
        if(dd->buttonRect != buttonRect) dd->buttonRect = buttonRect;
        painter->drawPixmap(buttonRect, currentPix);
    }
    else if(dd->downloadState == DownloadData::Completed)
    {
        QDateTime time = dd->completionTime;
        QString currentDateString;
        if(time.date() == QDate::currentDate())
            currentDateString = time.toString("hh:mm AP");
        else
            currentDateString = time.toString("MMM dd");
        QRect headerTextRect(13, 7, rect.width() - 26, rect.height());
        int wid = painter->fontMetrics().width(currentDateString);
        headerTextRect.adjust(0,0,-wid, 0);
        painter->drawText(headerTextRect, Qt::AlignLeft | Qt::AlignTop, dd->title, &headerTextRect);
        font.setPixelSize(12);
        painter->setFont(font);
        painter->drawText(QRect(rect.width() - 13 - wid, headerTextRect.top(), wid,
                                headerTextRect.height()), Qt::AlignVCenter | Qt::AlignRight, currentDateString );
        int size = dd->fileSize;
        /*
        QString sizeString = Helper::formatTime(dd->videoDuration) + " -- ";
        if(sizeString.startsWith("00:")) sizeString.remove(0, 3);
        */
        QString sizeString;
        if(size > 1048576 )
        {
            sizeString += tr("%1 MB").arg(QString::number(size / (qreal)1048576, 'f', 1));
        }
        else if(size > 1024 )
        {
            sizeString += tr("%1 KB").arg(QString::number(size / (qreal)1024, 'f', 1));
        }
        else
        {
            sizeString += tr("%1 bytes").arg(size);
        }
        sizeString += " ";
        painter->drawText(QRect(headerTextRect.left(), headerTextRect.bottom() + 5, rect.width(), rect.height()),
                          Qt::AlignLeft | Qt::AlignTop, sizeString);        
        currentPix = playIcon.pixmap(dd->buttonState, MyIcon::Off );
        QRect buttonRect(rect.width() - 13 - currentPix.width(), headerTextRect.bottom() + 5, currentPix.width(), currentPix.height());
        if(dd->buttonRect != buttonRect) dd->buttonRect = buttonRect;
        painter->drawPixmap(buttonRect, currentPix);
    }
    painter->setPen(QColor("#e3e3e3"));
    painter->drawLine(0, rect.height()-1, rect.width(), rect.height() - 1 );
    painter->restore();
}

MyIcon RecordingDelegate::getIcon(QPixmap pix)
{
    MyIcon icon;
    int w = pix.width();
    int h = pix.height();
    icon.setPixmap(pix.copy(0, 0, w, h/4 ), MyIcon::Normal, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/4, w, h/4 ), MyIcon::MouseOver, MyIcon::Off);
    icon.setPixmap(pix.copy(0, h/2, w, h/4 ), MyIcon::MouseDown, MyIcon::Off);
    icon.setPixmap(pix.copy(0, 3*h/4, w, h/4 ), MyIcon::Disabled, MyIcon::Off);
    return icon;
}

QSize RecordingDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    DownloadData* dd = index.data(RecordingDialog::DownloadDataRole).value<DownloadData*>();
    if(dd->downloadState == DownloadData::Progressing)
        return QSize(width, 70);
    else
        return QSize(width, 50);
}

RecordingDialog::RecordingDialog(QWidget *parent) :
    QWidget(parent), pressedItemLeftButton(0), pressedItemRightButton(0)
{
    recordings_directory = "/tmp/";
    recording_quality = 37;
    recording_format = 0;

    setWindowIcon( QPixmap(":/icons/logo.png") );
    setWindowTitle(tr("Recordings"));
    clearListButton = new QPushButton(tr("&Clear list"), this);
    openFolderButton = new QPushButton(tr("&Open folder"), this);
    downloadList = new QListWidget(this);

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(clearListButton);
    hbox->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Preferred));
    hbox->addWidget(openFolderButton);
    hbox->setContentsMargins(7, 4, 7, 3);
    MyBorder* border = new MyBorder(this);
    border ->setBGColor(palette().color(backgroundRole()));

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(downloadList);    
    vbox->addWidget(border);
    vbox->addLayout(hbox);
    vbox->setContentsMargins(0,0,0,0);
    vbox->setSpacing(0);    

    RecordingDelegate* delegate = new RecordingDelegate(downloadList);
    downloadList->setItemDelegate(delegate);
    downloadList->setSelectionMode(QAbstractItemView::SingleSelection);
    downloadList->setAlternatingRowColors(true);
    downloadList->viewport()->installEventFilter(this);
    downloadList->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
    downloadList->viewport()->setMouseTracking(true);
    resize(500, 300);
    QAbstractItemModel* downloadModel = downloadList->model();

    connect(clearListButton, SIGNAL(clicked()), this, SLOT(clearList()));    
    connect(downloadModel, SIGNAL(rowsRemoved(const QModelIndex&, int, int)), this, SLOT(updateClearListButton()));
    connect(downloadModel, SIGNAL(rowsInserted(const QModelIndex&, int, int)), this, SLOT(updateClearListButton()));
    connect(downloadList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(videoDoubleClicked(QListWidgetItem*)));
    connect(downloadModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateClearListButton()));
    connect(openFolderButton, SIGNAL(clicked()), this, SLOT(openFolder()));
    loadList();
}

RecordingDialog::~RecordingDialog()
{
    foreach(DownloadFile* dfile, itemDownloadMap.keys())
    {
        dfile->cancel();
    }
}

void RecordingDialog::downloadVideoId(QString videoId, QString title, double)
{
    RetrieveVideoUrl* rvu = new RetrieveVideoUrl(this);
    connect(rvu, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), this, SLOT(recordVideo(QMap<int,QString>,QString,QString)));
    connect(rvu, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), rvu, SLOT(deleteLater()));
    rvu->fetchYTVideoPage(videoId, title);

    if(!isVisible()) show();
}

void RecordingDialog::recordVideo(QMap<int, QString> qualityMap, QString title, QString id)
{
    QString url;
    switch(recording_quality)
    {
    case RetrieveVideoUrl::FullHD:
        url = qualityMap.value(RetrieveVideoUrl::FullHD, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::HD:
        url = qualityMap.value(RetrieveVideoUrl::HD, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::Normal:
        url = qualityMap.value(RetrieveVideoUrl::Normal, QString());
        break;
    case RetrieveVideoUrl::NormalFlv:
        url = qualityMap.value(RetrieveVideoUrl::NormalFlv, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::BelowNormalFlv:
        url = qualityMap.value(RetrieveVideoUrl::BelowNormalFlv, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::LowFlv:
        url = qualityMap.value(RetrieveVideoUrl::LowFlv, QString());
        break;
    }
    if(url.isNull())
    {
        QMessageBox::warning(0, tr("Recording failed"), tr("There was an error in retrieving the download URL."));
        return;
    }
    download(url, title, id, 0);
}

void RecordingDialog::download(QString url, QString title, QString id, double duration)
{
    QUrl qurl(url);    
    QListWidgetItem* item = new QListWidgetItem(0, QListWidgetItem::UserType + 1);
    downloadList->insertItem(0, item);
    DownloadData* dd = new DownloadData;
    dd->downloadState = DownloadData::Progressing;
    dd->videoId = id;
    QFile* file = new QFile(getUniqueFileName(title + (recording_format == 0 ? ".mp4" : ".flv")));
    dd->title = QFileInfo(*file).fileName();
    dd->filePath = QFileInfo(*file).absoluteFilePath();
    dd->videoDuration = (int)duration;
    dd->downloadProgress = tr("Starting");
    dd->completionTime = QDateTime::currentDateTime();
    item->setData(DownloadDataRole, QVariant::fromValue(dd));
    item->setData(emitDataChangedRole, true);
    DownloadFile* dfile = new DownloadFile(url, file, this);
    file->setParent(dfile);    
    itemDownloadMap[dfile] = item;    
    connect(dfile, SIGNAL(downloadFinished(bool)), this, SLOT(oneDownloadFinished(bool)));
    connect(dfile, SIGNAL(downloadStatus(QString)), this, SLOT(oneDownloadProgressStatus(QString)));
    connect(dfile, SIGNAL(progress(int, qint64)), this, SLOT(oneDownloadProgress(int, qint64)));
    connect(dfile, SIGNAL(errorOcurred(int)), this, SLOT(oneDownloadError(int)));
}

QString RecordingDialog::getUniqueFileName(QString name)
{
    QDir dir(recordings_directory);
    if(!dir.exists()) dir.mkpath(dir.absolutePath());
    name.replace(QRegExp("[/\\\\\\?\\%\\*\\:\\|\\\"\\<\\>]"), "_");
    QString nameCopy = name;
    for(int i=1; dir.exists(nameCopy); ++i)
    {
        nameCopy = name;
        nameCopy.insert(name.lastIndexOf('.'), QString("_%1").arg(i));
    }
    return dir.filePath(nameCopy);
}

void RecordingDialog::oneDownloadFinished(bool error)
{
    DownloadFile* dfile = static_cast<DownloadFile*>(sender());
    DownloadData* dd = itemDownloadMap.value(dfile)->data(DownloadDataRole).value<DownloadData*>();
    if(error){
        dd->downloadState = DownloadData::Error;
        dd->downloadProgress = tr("Error");
    }
    else {
        dd->downloadState = DownloadData::Completed;
        dd->downloadProgress = tr("Completed");
        /* dd->videoDuration = InfoProvider::getInfo( dd->filePath ).duration; */
        dd->videoDuration = 0;
    }
    dd->completionTime = QDateTime::currentDateTime();    
    dd->fileSize = dfile->fileSize();    
    QListWidgetItem* item = itemDownloadMap.value(dfile);
    item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
    removeDFileFromMap(dfile);
    saveList();
}

void RecordingDialog::oneDownloadProgressStatus(QString status)
{
    DownloadFile* dfile = static_cast<DownloadFile*>(sender());
    DownloadData* dd = itemDownloadMap.value(dfile)->data(DownloadDataRole).value<DownloadData*>();
    dd->downloadProgress = status;
    QListWidgetItem* item = itemDownloadMap.value(dfile);
    item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
}

void RecordingDialog::oneDownloadError(int error)
{    
    DownloadFile* dfile = static_cast<DownloadFile*>(sender());
    DownloadData* dd = itemDownloadMap.value(dfile)->data(DownloadDataRole).value<DownloadData*>();
    if(error == 5) //OperationCanceledError
    {
        dd->downloadState = DownloadData::Canceled;
    }
    else
    {
        dd->downloadState = DownloadData::Error;
    }    
    QListWidgetItem* item = itemDownloadMap.value(dfile);    
    item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());    
    if(itemsMarkedForRemoval.contains(item))
    {
        delete item->data(DownloadDataRole).value<DownloadData*>();
        delete downloadList->takeItem(downloadList->row(item));
        itemsMarkedForRemoval.removeOne(item);
    }
    removeDFileFromMap(dfile);
    saveList();
}

void RecordingDialog::oneDownloadProgress(int percent, qint64 total)
{
    DownloadFile* dfile = static_cast<DownloadFile*>(sender());
    DownloadData* dd = itemDownloadMap.value(dfile)->data(DownloadDataRole).value<DownloadData*>();
    dd->downloadProgressPercent = percent;
    dd->fileSize = total;
    updateWindowTitle();
    QListWidgetItem* item = itemDownloadMap.value(dfile);
    item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
}

void RecordingDialog::updateWindowTitle()
{
    // update window title
    qint64 sum = 0;
    qint64 downloadedSum = 0;
    int count = itemDownloadMap.count();    
    foreach(QListWidgetItem* item, itemDownloadMap.values())
    {        
        DownloadData* downloadData = item->data(DownloadDataRole).value<DownloadData*>() ;
        if(downloadData->fileSize <= 0)
        {
            --count;
            continue;
        }
        downloadedSum += downloadData->fileSize*downloadData->downloadProgressPercent/100 ;
        sum += downloadData->fileSize;
    }
    if(count == 0)
    {
        setWindowTitle(tr("Recordings"));
        return;
    }
    int percent = downloadedSum*100/(double)sum;
    setWindowTitle(tr("%1% of %n video - Recordings", "", count).arg(percent));
}

void RecordingDialog::removeDFileFromMap(DownloadFile *dfile)
{
    dfile->disconnect();
    itemDownloadMap.remove(dfile);
    dfile->deleteLater();
    updateWindowTitle();
}

void RecordingDialog::resizeEvent(QResizeEvent *r)
{
    static_cast<RecordingDelegate*>(downloadList->itemDelegate())->setWidth( downloadList->viewport()->width());
}

bool RecordingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == downloadList->viewport())
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* m = static_cast<QMouseEvent*>(event);
            QPoint vpPos = downloadList->viewport()->mapFromGlobal(m->globalPos());
            if(m->button() == Qt::LeftButton)
            {
                pressedItemLeftButton = downloadList->itemAt(vpPos);
                if(pressedItemLeftButton)
                {
                    DownloadData* dd = pressedItemLeftButton->data(DownloadDataRole).value<DownloadData*>();
                    if(dd->buttonRect.contains(vpPos - downloadList->visualItemRect(pressedItemLeftButton).topLeft() ))
                    {
                        dd->buttonState = MyIcon::MouseDown;
                        pressedItemLeftButton->setData(emitDataChangedRole, !pressedItemLeftButton->data(emitDataChangedRole).toBool());
                    }
                    else
                    {
                        pressedItemLeftButton = 0;
                    }
                }
            }
            else if(m->button() == Qt::RightButton)
            {
                pressedItemRightButton = downloadList->itemAt(vpPos);
            }
        }
        if(event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* m = static_cast<QMouseEvent*>(event);
            QPoint vpPos = downloadList->viewport()->mapFromGlobal(m->globalPos());
            if(m->button() == Qt::LeftButton)
            {
                QListWidgetItem* releasedItem = downloadList->itemAt(vpPos);
                if(pressedItemLeftButton && pressedItemLeftButton == releasedItem && releasedItem->data(DownloadDataRole).value<DownloadData*>()->
                   buttonRect.contains(vpPos- downloadList->visualItemRect(releasedItem).topLeft()))
                {
                    DownloadData* dd = releasedItem->data(DownloadDataRole).value<DownloadData*>();
                    dd->buttonState = MyIcon::Normal;
                    releasedItem->setData(emitDataChangedRole, !releasedItem->data(emitDataChangedRole).toBool());
                    switch(dd->downloadState)
                    {
                    case DownloadData::Progressing:
                        cancelDownload(releasedItem);
                        break;
                    case DownloadData::Canceled:
                    case DownloadData::Error:
                        retryDownload(releasedItem);
                        break;
                    case DownloadData::Completed:
                        playDownload(releasedItem);
                        break;
                    }
                }
                pressedItemLeftButton = 0;
            }
            else if(m->button() == Qt::RightButton)
            {
                QListWidgetItem* releasedItem = downloadList->itemAt(vpPos);
                if(releasedItem && releasedItem == pressedItemRightButton)
                {
                    pressedItemRightButton = 0;
                    DownloadData* dd = releasedItem->data(DownloadDataRole).value<DownloadData*>();
                    QMenu contextMenu;
                    if(dd->downloadState == DownloadData::Progressing)
                    {
                        contextMenu.addAction(tr("&Cancel"))->setData("cancel");
                        contextMenu.addSeparator();
                        contextMenu.addAction(tr("&Remove from list"))->setData("remove");
                        QAction* action = contextMenu.exec(m->globalPos());
                        if(action)
                        {
                            if(action->data().toString() == "cancel")
                            {
                                cancelDownload(releasedItem);
                            }
                            else if(action->data().toString() == "remove")
                            {
                                itemsMarkedForRemoval.append(releasedItem);
                                cancelDownload(releasedItem);
                            }
                        }
                    }
                    else if(dd->downloadState == DownloadData::Canceled || dd->downloadState == DownloadData::Error )
                    {
                        contextMenu.addAction(tr("&Retry"))->setData("retry");
                        contextMenu.addSeparator();
                        contextMenu.addAction(tr("Remove from &list"))->setData("remove");
                        QAction* action = contextMenu.exec(m->globalPos());
                        if(action)
                        {
                            if(action->data().toString() == "retry")
                            {
                                retryDownload(releasedItem);
                            }
                            else if(action->data().toString() == ("remove"))
                            {
                                delete releasedItem->data(DownloadDataRole).value<DownloadData*>();
                                delete downloadList->takeItem(downloadList->row(releasedItem));
                            }
                        }

                    }
                    else if(dd->downloadState == DownloadData::Completed)
                    {
                        contextMenu.addAction(tr("&Play"))->setData("play");
                        contextMenu.addAction(tr("&Open folder"))->setData("folder");
                        contextMenu.addSeparator();
                        contextMenu.addAction(tr("&Delete"))->setData("delete");
                        contextMenu.addAction(tr("Remove from &list"))->setData("remove");
                        QAction* action = contextMenu.exec(m->globalPos());
                        if(action)
                        {
                            if(action->data().toString() == "play")
                            {
                                playDownload(releasedItem);
                            }
                            else if(action->data().toString() == "folder")
                            {
                                openFolder();
                            }
                            else if(action->data().toString() == "delete")
                            {
                                QFile::remove(releasedItem->data(DownloadDataRole).value<DownloadData*>()->filePath);
                                delete releasedItem->data(DownloadDataRole).value<DownloadData*>();
                                delete downloadList->takeItem(downloadList->row(releasedItem));
                            }
                            else if(action->data().toString() == "remove")
                            {
                                delete releasedItem->data(DownloadDataRole).value<DownloadData*>();
                                delete downloadList->takeItem(downloadList->row(releasedItem));
                            }
                        }

                    }


                }
            }
        }
        if(event->type() == QEvent::MouseMove)
        {
            QMouseEvent* m = static_cast<QMouseEvent*>(event);
            QPoint vpPos = downloadList->viewport()->mapFromGlobal(m->globalPos());
            QListWidgetItem* movedItem = downloadList->itemAt(vpPos);
            if( pressedItemLeftButton &&  pressedItemLeftButton == movedItem && ! movedItem->data(DownloadDataRole).value<DownloadData*>()->
                buttonRect.contains(vpPos- downloadList->visualItemRect(movedItem).topLeft()))
            {
                movedItem->data(DownloadDataRole).value<DownloadData*>()->buttonState = MyIcon::Normal;
                movedItem->setData(emitDataChangedRole, !movedItem->data(emitDataChangedRole).toBool());
            }
            if(!pressedItemLeftButton && movedItem && movedItem->data(DownloadDataRole).value<DownloadData*>()->
               buttonRect.contains(vpPos- downloadList->visualItemRect(movedItem).topLeft()))
            {
                movedItem->data(DownloadDataRole).value<DownloadData*>()->buttonState = MyIcon::MouseOver;
                movedItem->setData(emitDataChangedRole, !movedItem->data(emitDataChangedRole).toBool());
            }
            if( !pressedItemLeftButton && movedItem && !movedItem->data(DownloadDataRole).value<DownloadData*>()->
                buttonRect.contains(vpPos- downloadList->visualItemRect(movedItem).topLeft()))
            {
                movedItem->data(DownloadDataRole).value<DownloadData*>()->buttonState = MyIcon::Normal;
                movedItem->setData(emitDataChangedRole, !movedItem->data(emitDataChangedRole).toBool());
            }

        }
    }
    return false;
}

void RecordingDialog::cancelDownload(QListWidgetItem *item)
{
    DownloadFile* dfile = itemDownloadMap.key(item, 0);
    if(dfile)
    {
        dfile->cancel();
    }
    RetrieveVideoUrl* rvu = itemRVUMap.key(item, 0);
    if(rvu)
    {
        rvu->cancel();
    }
}

void RecordingDialog::retryDownload(QListWidgetItem *item)
{
    DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
    dd->downloadState = DownloadData::Progressing;
    dd->downloadProgress = tr("Fetching URL...");
    dd->downloadProgressPercent = 0;
    item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
    RetrieveVideoUrl* rvu = new RetrieveVideoUrl(this);
    connect(rvu, SIGNAL(gotUrls(QMap<int,QString>,QString, QString)), this, SLOT(urlToDownload(QMap<int,QString>,QString)));
    connect(rvu, SIGNAL(errorOcurred(QString,int)), this, SLOT(fetchUrlError(QString,int)));
    rvu->fetchYTVideoPage(dd->videoId, dd->title);
    itemRVUMap[rvu] = item;
}

void RecordingDialog::playDownload(QListWidgetItem *item)
{
    DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
    if(!QFile::exists(dd->filePath))
    {
        int button = QMessageBox::warning(this, tr("File error"),
                                          tr("The file does not exists, would you like to remove it from the list?"),
                                          QMessageBox::Yes, QMessageBox::No);
        if(button == QMessageBox::Yes)
        {
            delete item->data(DownloadDataRole).value<DownloadData*>();
            delete downloadList->takeItem(downloadList->row(item));
            saveList();
        }
        return;
    }
    emit playFile(dd->filePath);
}

void RecordingDialog::urlToDownload(QMap<int, QString> qualityMap, QString title)
{
    QString url;
    switch(recording_quality)
    {
    case RetrieveVideoUrl::FullHD:
        url = qualityMap.value(RetrieveVideoUrl::FullHD, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::HD:
        url = qualityMap.value(RetrieveVideoUrl::HD, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::Normal:
        url = qualityMap.value(RetrieveVideoUrl::Normal, QString());
        break;
    case RetrieveVideoUrl::NormalFlv:
        url = qualityMap.value(RetrieveVideoUrl::NormalFlv, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::BelowNormalFlv:
        url = qualityMap.value(RetrieveVideoUrl::BelowNormalFlv, QString());
        if(!url.isNull()) break;
    case RetrieveVideoUrl::LowFlv:
        url = qualityMap.value(RetrieveVideoUrl::LowFlv, QString());
        break;
    }
    if(url.isNull())
    {
        QMessageBox::warning(0, tr("Recording failed"), tr("There was an error in retrieving the download URL."));
        return;
    }
    RetrieveVideoUrl* rvu = static_cast<RetrieveVideoUrl*>(sender());
    QListWidgetItem* item = itemRVUMap.value(rvu, 0);
    if(!item) return;
    DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
    rvu->disconnect();
    itemRVUMap.remove(rvu);
    rvu->deleteLater();

    if(dd && dd->title == title)
    {
        QUrl qurl(url);                                
        dd->downloadState = DownloadData::Progressing;        
        QFile* file = new QFile(getUniqueFileName(title));
        dd->title = QFileInfo(*file).fileName();
        dd->filePath = QFileInfo(*file).absoluteFilePath();
        dd->downloadProgress = tr("Starting");
        dd->completionTime = QDateTime::currentDateTime();        
        item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
        DownloadFile* dfile = new DownloadFile(url, file, this);
        file->setParent(dfile);
        itemDownloadMap[dfile] = item;        
        connect(dfile, SIGNAL(downloadFinished(bool)), this, SLOT(oneDownloadFinished(bool)));
        connect(dfile, SIGNAL(downloadStatus(QString)), this, SLOT(oneDownloadProgressStatus(QString)));
        connect(dfile, SIGNAL(progress(int, qint64)), this, SLOT(oneDownloadProgress(int, qint64)));
        connect(dfile, SIGNAL(errorOcurred(int)), this, SLOT(oneDownloadError(int)));
    }
}

void RecordingDialog::fetchUrlError(QString title, int error)
{
    RetrieveVideoUrl* rvu = static_cast<RetrieveVideoUrl*>(sender());    
    QListWidgetItem* item = itemRVUMap.value(rvu, 0);
    if(item)
    {
        DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
        if(dd && dd->title == title)
        {
            if(error == 5) dd->downloadState = DownloadData::Canceled;
            else dd->downloadState = DownloadData::Error;
            item->setData(emitDataChangedRole, !item->data(emitDataChangedRole).toBool());
        }
    }
    rvu->disconnect();
    itemRVUMap.remove(rvu);
    rvu->deleteLater();
}

void RecordingDialog::clearList()
{
    QAbstractItemModel* model = downloadList->model();
    for(int i =0; i < model->rowCount(); )
    {
        if(model->index(i, 0).data(DownloadDataRole).value<DownloadData*>()->downloadState == DownloadData::Progressing)
        {
            ++i ;
        }
        else
        {            
            delete model->index(i, 0).data(DownloadDataRole).value<DownloadData*>();
            model->removeRow(i);
        }
    }
    saveList();
}

void RecordingDialog::updateClearListButton()
{
    QAbstractItemModel* model = downloadList->model();
    int nonProgressCount = 0;
    for(int i =0; i < model->rowCount(); ++i)
    {
        DownloadData* dd = model->index(i, 0).data(DownloadDataRole).value<DownloadData*>();
        if(!dd) continue;
        if(dd->downloadState != DownloadData::Progressing)
        {
            ++nonProgressCount;
        }
    }
    if(nonProgressCount > 0)
        clearListButton->setEnabled(true);
    else
        clearListButton->setEnabled(false);

}

void RecordingDialog::videoDoubleClicked(QListWidgetItem *item)
{
    DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
    if(dd->downloadState == DownloadData::Completed)
    {
        playDownload(item);
    }
}

void RecordingDialog::openFolder()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(recordings_directory));
}

void RecordingDialog::saveList()
{
    QList<QVariant> itemList;
    for(int i=0; i < downloadList->count(); ++i)
    {
        QVariantMap ddMap;
        QListWidgetItem* item = downloadList->item(i);
        if(item)
        {
            DownloadData* dd = item->data(DownloadDataRole).value<DownloadData*>();
            if(dd->downloadState == DownloadData::Progressing)
                continue;
            ddMap["state"] = dd->downloadState;
            ddMap["time"] = dd->completionTime;
            ddMap["size"] = dd->fileSize;
            ddMap["path"] = dd->filePath;
            ddMap["videoid"] = dd->videoId;
            ddMap["video_duration"] = dd->videoDuration;
            ddMap["title"] = dd->title;
        }
        itemList.append(ddMap);
    }
    /* pref->downloadHistory = itemList; */
}

void RecordingDialog::loadList()
{
    /*
    for(int i=0; i < pref->downloadHistory.count(); ++i)
    {
        QVariantMap ddMap = pref->downloadHistory.value(i).toMap();
        QListWidgetItem* item = new QListWidgetItem(0, QListWidgetItem::UserType + 1);
        downloadList->insertItem(0, item);
        DownloadData* dd = new DownloadData;
        dd->downloadState = (DownloadData::State)ddMap.value("state").toInt();
        dd->videoId = ddMap.value("videoid").toString();
        dd->title = ddMap.value("title").toString();
        dd->filePath = ddMap.value("path").toString();
        dd->videoDuration = ddMap.value("video_duration").toInt();
        dd->completionTime = ddMap.value("time").toDateTime();
        dd->fileSize = ddMap.value("size").toLongLong();
        item->setData(DownloadDataRole, QVariant::fromValue(dd));
        item->setData(emitDataChangedRole, true);
    }
    */
}

#include "moc_recordingdialog.cpp"
