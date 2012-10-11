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

#include <QVBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QTimeLine>
#include <QProcess>
#include <QClipboard>
#include <QSettings>
#include "ytdialog.h"
#include "yttabbar.h"
#include "ytdelegate.h"
#include "ytdataapi.h"
#include "retrievevideourl.h"
#include "myborder.h"
#include "searchbox.h"
#include "recordingdialog.h"
#include "configdialog.h"
#include "about.h"

#define PAGE_RESULT_COUNT 25

OverlayWidget::OverlayWidget(QWidget* parent) : QWidget(parent)
{
    loadingOverlay = QPixmap(":/Control/bg-youtube-loading-overlay.png");
    exclaimPix = QPixmap(":/Control/bg-alert.png");    
    timeLine = new QTimeLine(1000, this);
    timeLine->setLoopCount(0);
    timeLine->setFrameRange(1, 13);
    for(int i=0; i < 12; ++i)
    {
        stages[i] = QPixmap((QString("stage%1").arg(i+1, 2, 10, QChar('0'))));
    }
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(frameHasChanged(int)));
}

void OverlayWidget::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.drawPixmap(rect(), loadingOverlay  );
    if(m_text.isEmpty())
    {        
        const QPixmap pix = stages[qMax(1,qMin(12,timeLine->currentFrame()))-1];
        QPoint trans = rect().center() - pix.rect().center();
        p.drawPixmap(trans, pix);
        return;
    }
    p.setPen(Qt::white);
    QFont font = p.font();
    font.setPointSize(12);
    p.setFont(font);
    QRect textRect(0, 0, 280, 40);
    textRect.translate(rect().center() - textRect.center());
    QRect alertRect = exclaimPix.rect();
    alertRect.setHeight(textRect.height());
    alertRect.moveTopLeft(textRect.topLeft());
    p.drawPixmap( alertRect, exclaimPix);
    textRect.setX(alertRect.right() + 10);
    p.drawText(textRect, Qt::AlignTop|Qt::AlignLeft|Qt::TextWordWrap| Qt::TextDontClip ,m_text);
}

void OverlayWidget::showEvent(QShowEvent* s)
{
    if(!s->spontaneous() && m_text.isEmpty())
    {
        timeLine->start();
    }
}

void OverlayWidget::hideEvent(QHideEvent *h)
{
    if(!h->spontaneous() && m_text.isEmpty())
    {
        timeLine->stop();
    }
}

void OverlayWidget::setText(QString text)
{
    m_text = text;
    update();
}

void OverlayWidget::frameHasChanged(int)
{
    update();
}


/******************************************************************************/


YTButton::YTButton(QWidget *parent)
    : QAbstractButton(parent), hovered(false)
{

}

void YTButton::setPix(QPixmap pix)
{
   normalPix = pix.copy(0, 0, pix.width(), pix.height()/2);
   hoverPix = pix.copy(0, pix.height()/2, pix.width(), pix.height()/2);
   setFixedSize(normalPix.size());
}

void YTButton::enterEvent(QEvent *e)
{
    hovered = true;
    update();
}

void YTButton::leaveEvent(QEvent *e)
{
    hovered = false;
    update();
}

void YTButton::paintEvent(QPaintEvent *e)
{    
    QPainter p(this);
    if(hovered)
    {
        p.drawPixmap(0, 0, hoverPix);
    }
    else
    {
        p.drawPixmap(0, 0, normalPix);
    }
}
/*******************************************************************************************************
***********************************************************************************************************/

PixmapLoader::~PixmapLoader()
{
    manager->deleteLater();
}

int PixmapLoader::getPixmap(QString url)
{
    QNetworkRequest req(url);
    QNetworkReply* rep = manager->get(req);
    rep->setObjectName(url);
    return (qint64)rep;

}

PixmapLoader::PixmapLoader(QObject *parent)
    :QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotPixmap(QNetworkReply*)));
}

void PixmapLoader::gotPixmap(QNetworkReply *reply)
{
    QByteArray arr = reply->readAll();
    QPixmap pix;
    pix.loadFromData(arr);
    reply->deleteLater();
    emit pixmapResult(pix, (qint64)reply);
}

void PixmapLoader::reset()
{
    manager->deleteLater();
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotPixmap(QNetworkReply*)));
}

/*******************************************************************************************************
***********************************************************************************************************/

YTDialog::YTDialog(QWidget *parent, QSettings * settings) :
    QWidget(parent), overlayVisible(false)
{
    set = settings;

    setWindowIcon( QPixmap(":/icons/logo.png") );
    setAutoFillBackground(true);
    setWindowTitle(tr("YouTube Browser - SMPlayer"));
    tabBar = new YTTabBar(this);
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(gotCurrentTab(int)));
    overlay = new OverlayWidget(this);
    videoList = new QListWidget(this);
    pixmap_loader = new PixmapLoader;

    recording_dialog = new RecordingDialog;
    recording_dialog->setRecordingsDirectory("");
    recording_dialog->setRecordingQuality(HD);
    recording_dialog->setRecordingFormat(0);

    MyBorder* border = new MyBorder(this);
    border ->setBGColor(palette().color(backgroundRole()));

    videoList->setFrameShape(QFrame::NoFrame);
    delegate = new YTDelegate(videoList);
    videoList->setItemDelegate(delegate);    
    //videoList->setMouseTracking(true);
    //videoList->viewport()->installEventFilter(this);
    videoList->setContextMenuPolicy(Qt::CustomContextMenu);
    videoList->setAutoScroll(false);
    nextButton = new QPushButton(this);
    nextButton->setToolTip(tr("Next"));
    nextButton->setIcon(QPixmap(":/icons/next.png"));
    nextButton->setShortcut(QKeySequence("Alt+N"));
    nextButton->adjustSize();
    nextButton->setFixedWidth(nextButton->width());
    prevButton = new QPushButton(this);
    prevButton->setToolTip(tr("Previous"));
    prevButton->setIcon(QPixmap(":/icons/previous.png"));
    prevButton->setShortcut(QKeySequence("Alt+P"));
    prevButton->adjustSize();
    prevButton->setFixedWidth(prevButton->width());
    searchBox = new SearchBox(this);
    connect(searchBox, SIGNAL(search(QString)), this, SLOT(setSearchTerm(QString)));    
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextClicked()));
    connect(prevButton, SIGNAL(clicked()), this, SLOT(prevClicked()));

    configButton = new QToolButton(this);
    //configButton->setText("Config");
    configButton->setIcon(QPixmap(":/icons/prefs.png"));
    configButton->setToolTip(tr("Settings"));
    configButton->setShortcut(QKeySequence("Alt+S"));
    connect(configButton, SIGNAL(clicked()), this, SLOT(showConfigDialog()));

    infoButton = new QToolButton(this);
    infoButton->setIcon(QPixmap(":/icons/info.png"));
    infoButton->setToolTip(tr("Information"));
    infoButton->setShortcut(QKeySequence("Alt+I"));
    connect(infoButton, SIGNAL(clicked()), this, SLOT(showAboutDialog()));

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(searchBox);
    hbox->addWidget(configButton);
    hbox->addWidget(infoButton);

    hbox->addSpacerItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Preferred));
    hbox->addWidget(prevButton);        
    hbox->addWidget(nextButton);    
    hbox->setContentsMargins(6, 5, 8, 3);
    hbox->setSpacing(9);
    QVBoxLayout* vbox = new QVBoxLayout;    
    vbox->addWidget(tabBar);
    vbox->addWidget(videoList);
    vbox->addWidget(border );
    vbox->addLayout(hbox);
    setLayout(vbox);
    vbox->setSpacing(0);
    vbox->setContentsMargins( 0, 0, 0, 0);
    overlay->raise();
    setLoadingOverlay(false);    
    api = new YTDataAPI(this);
    connect(api, SIGNAL(finalResults(YTReply)), this, SLOT(gotAPIReply(YTReply)));
    connect(pixmap_loader, SIGNAL(pixmapResult(QPixmap,int)), this, SLOT(gotPixmap(QPixmap,int)));
    connect(videoList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(videoClicked(QListWidgetItem*)));
    connect(videoList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(videoDblClicked(QListWidgetItem*)));
    connect(videoList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(recording_dialog, SIGNAL(playFile(QString)), this, SLOT(playVideo(QString)));
    /*
    connect(this, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), 
            this, SLOT(playYTUrl(QMap<int,QString>,QString, QString)));
    */

    setTabOrder(searchBox, configButton);
    setTabOrder(configButton, infoButton);
    setTabOrder(infoButton, prevButton);
    setTabOrder(prevButton, nextButton);

    loadConfig();

    searchBox->setFocus();
}

YTDialog::~YTDialog() 
{
    delete pixmap_loader;
    delete recording_dialog; 
}

void YTDialog::setLoadingOverlay(bool enable)
{
    overlayVisible = enable;
    if(enable)
    {
        overlay->resize(size());
        overlay->setText(QString());
        overlay->show();
    }
    else
    {
        overlay->hide();
    }
}

void YTDialog::resizeEvent(QResizeEvent* r)
{
    if(overlayVisible)
    {
        overlay->resize(r->size());
    }
}

bool YTDialog::eventFilter(QObject* w, QEvent* e)
{
    if(w == videoList->viewport())
    {        
        if(e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* m = static_cast<QMouseEvent*>(e);
            QListWidgetItem* hoveredItem =  videoList->itemAt(videoList->viewport()->mapFromGlobal(m->globalPos()));
            for(int i=0; i < videoList->count(); ++i)
            {
                QListWidgetItem* wi = videoList->item(i);                
                if( wi == hoveredItem )
                {                    
                    wi->setData(Hovered, true);
                }
                else
                {                    
                    wi->setData(Hovered, false);
                }
            }
            if(hoveredItem)
                setCursor(QCursor(Qt::PointingHandCursor));
            else
                setCursor(QCursor(Qt::ArrowCursor));
        }
        if(e->type() == QEvent::Leave)
        {
            for(int i=0; i < videoList->count(); ++i)
            {
                QListWidgetItem* wi = videoList->item(i) ;                
                wi->setData(Hovered, false);
            }
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
    return false;
}

void YTDialog::setMode(Mode mode)
{
    disconnect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(gotCurrentTab(int)));
    reset();    
    if(mode == Button)
    {
        searchTerm = QString();
        searchBox->setText(searchTerm);
        addTab(Popular);
        addTab(ViewedButton);
        addTab(Rated);        
    }
    else if(mode == Search)
    {
        searchBox->setText(searchTerm);
        addTab(Relevant);
        addTab(Recent);
        addTab(Viewed);        
    }
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(gotCurrentTab(int)));
    tabBar->setCurrentIndex(0);
    gotCurrentTab(0);
}

void YTDialog::addTab(Tabs tab)
{
    int index = -1;    
    if(tab == Relevant)
    {
        index = tabBar->addTab(tr("Most Relevant"));
    }
    else if(tab == Recent)
    {
        index = tabBar->addTab(tr("Most Recent"));
    }
    else if(tab == Viewed)
    {
        index = tabBar->addTab(tr("Most Viewed"));
    }
    else if(tab == Popular)
    {
        index = tabBar->addTab(tr("Most Popular"));
    }
    else if(tab == ViewedButton)
    {
        index = tabBar->addTab(tr("Most Viewed"));
    }
    else if(tab == Rated)
    {
        index = tabBar->addTab(tr("Most Rated"));
    }    
    tabBar->setTabData(index, tab);
}

void YTDialog::gotCurrentTab(int index)
{
    Tabs tab = static_cast<Tabs>(tabBar->tabData(index).toInt());
    //Title text
    switch((int)tab)
    {
    case Popular: setWindowTitle(tr("Most popular videos on YouTube%1").arg(QChar(0x2122)));break;
    case Rated: setWindowTitle(tr("Most rated videos on YouTube%1").arg(QChar(0x2122)));break;
    case ViewedButton: setWindowTitle(tr("Most viewed videos on YouTube%1").arg(QChar(0x2122)));break;
    case Relevant :
    case Recent :
    case Viewed : setWindowTitle(tr("YouTube%2 results for \"%1\"").arg(searchTerm).arg(QChar(0x2122)));break;
    }

    if(entries.value(tab).populated)
    {
        updateNextPrevWidget();
    }
    else
    {
        setLoadingOverlay(true);
        switch((int)tab)
        {
        case Popular: resultForTab[api->getMost(Popular)] = Popular; break;
        case Rated: resultForTab[api->getMost(Rated)]= Rated; break;
        case ViewedButton: resultForTab[api->getMost(ViewedButton)]= ViewedButton; break;
        case Relevant : resultForTab[api->getMost(Relevant, searchTerm)]= Relevant; break;
        case Recent : resultForTab[api->getMost(Recent, searchTerm)]= Recent; break;
        case Viewed : resultForTab[api->getMost(Viewed, searchTerm)]= Viewed; break;
        }
    }
}

void YTDialog::nextClicked()
{    
    Tabs tab = static_cast<Tabs>(tabBar->tabData(tabBar->currentIndex()).toInt());
    TabData td = entries.value(tab);
    int lastPageNumber = lastPageNo(tab);
    if( lastPageNumber == td.pageNo)
    {
        if( !entries[tab].nextUrl.isEmpty() )
        {
            entries[tab].pageNo += 1;
            setLoadingOverlay(true);
            resultForTab[api->getNextResults(entries[tab].nextUrl)] = tab;
        }
    }
    else
    {
        entries[tab].pageNo += 1;
        updateNextPrevWidget();
    }

}

void YTDialog::prevClicked()
{
    Tabs tab = static_cast<Tabs>(tabBar->tabData(tabBar->currentIndex()).toInt());    
    entries[tab].pageNo = qMax(entries[tab].pageNo - 1, 1);
    updateNextPrevWidget();
}

void YTDialog::updateNextPrevWidget()
{
    videoList->clear();
    Tabs tab = static_cast<Tabs>(tabBar->tabData(tabBar->currentIndex()).toInt());
    QList<SingleVideoItem*> vList = entries.value(tab).data;
    //if(vList.empty())
        //return;
    QList<SingleVideoItem*>::iterator it = vList.begin();
    int thisPageNo = entries.value(tab).pageNo;
    it += (thisPageNo -1 ) * PAGE_RESULT_COUNT;
    for(int i = 0; i< PAGE_RESULT_COUNT; ++i)
    {
        if(it >= vList.end() )
            break;
        QListWidgetItem* wItem = new QListWidgetItem(videoList, QListWidgetItem::UserType + 1);
        SingleVideoItem* svi = *it;
        if(!svi->pixUrl.isEmpty())
        {
            int id = pixmap_loader->getPixmap(svi->pixUrl);
            pendingPixmapQueue[id] = svi;
        }
        wItem->setData(0, qVariantFromValue(*it));
        wItem->setToolTip(svi->header);
        ++it;
    }        
    if(thisPageNo < lastPageNo(tab))
        nextButton->setEnabled(true);
    else if(!entries.value(tab).nextUrl.isEmpty())
        nextButton->setEnabled(true);
    else
        nextButton->setEnabled(false);
    if(thisPageNo == 1 )
        prevButton->setEnabled(false);
    else
        prevButton->setEnabled(true);

    videoList->scrollToItem(videoList->item(0), QAbstractItemView::EnsureVisible);
}

void YTDialog::gotAPIReply(const YTReply& formattedReply)
{    
    bool error = false;

    if(resultForTab.contains(formattedReply.replyPointer))
    {        
        Tabs tab = resultForTab.value(formattedReply.replyPointer);            
        for(int i=0; i < formattedReply.results.count(); ++i)
        {
            SingleVideoItem* svi = new SingleVideoItem;
            SingleResult sing = formattedReply.results.value(i);
            svi->date = sing.publishedDate;
            svi->desc = sing.desc.simplified();
            svi->header = sing.title;
            svi->totalTime = sing.duration;
            svi->views = sing.viewCount;
            svi->pixUrl = sing.thumbUrl;
            svi->videoid = sing.videoId;
            entries[tab].data.append(svi);
        }
        entries[tab].populated = true;
        entries[tab].nextUrl = formattedReply.nextUrl;
        resultForTab.remove(formattedReply.replyPointer);
        updateNextPrevWidget();
        if(!formattedReply.errorString.isEmpty())
        {
            overlay->setText(tr("Error: Could not connect to Youtube server."));
            return;
        }
        if(formattedReply.results.count() == 0)
        {
            error = true;

            if(searchTerm.isEmpty())
                showErrorDialog(tr("No videos found"));
            else
                showErrorDialog(tr("No videos found for \"%1\"").arg(searchTerm));
        }

    }
    setLoadingOverlay(false);

    if (error) setMode(YTDialog::Button);
}

void YTDialog::gotPixmap(QPixmap pix, int id)
{
    SingleVideoItem* item = pendingPixmapQueue.value(id);
    if(item)
    {
        item->pix = pix;
        item->pixUrl = QString();
        videoList->viewport()->update();
        pendingPixmapQueue.remove(id);
    }
}

void YTDialog::setSearchTerm(QString term)
{
    searchTerm = term;    
    setMode(Search);
}

void YTDialog::reset()
{
    pixmap_loader->reset();
    api->reset();
    pendingPixmapQueue.clear();
    resultForTab.clear();
    foreach(TabData d, entries.values())
    {
        qDeleteAll(d.data);
    }
    entries.clear();
    videoList->clear();
    while(tabBar->count() > 0)
    {
        tabBar->removeTab(0);
    }
}

int YTDialog::lastPageNo(Tabs tab)
{
    TabData td = entries[tab];
    return td.data.count()/PAGE_RESULT_COUNT + ((td.data.count() % PAGE_RESULT_COUNT)>0?1:0);
}

void YTDialog::videoClicked(QListWidgetItem *item)
{
    for(int i=0; i < videoList->count(); ++i)
    {
        QListWidgetItem* wi = videoList->item(i) ;
        wi->setData(Clicked, false);
    }
    item->setData(Clicked, true);
}

void YTDialog::videoDblClicked(QListWidgetItem *item)
{
    SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
    if (!players.currentPlayer().directPlay()) {
        RetrieveVideoUrl* rvu = new RetrieveVideoUrl(this);
        connect(rvu, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), this, SLOT(playYTUrl(QMap<int,QString>, QString, QString)) );
        connect(rvu, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), rvu, SLOT(deleteLater()));
        rvu->fetchYTVideoPage( svi->videoid, svi->header );
    } else {
        QString video = "http://www.youtube.com/watch?v=" + svi->videoid;
        playVideo(video);
    }
}

void YTDialog::showContextMenu(QPoint point)
{ 
    QMenu menu;
    menu.addAction(tr("&Play video"))->setData("play");
    menu.addAction(tr("&Record video"))->setData("record");
    menu.addAction(tr("&Watch on YouTube"))->setData("watch");
    menu.addAction(tr("&Copy link"))->setData("link");

    QAction* action = menu.exec(videoList->viewport()->mapToGlobal(point));
    if(!action) return;
    QListWidgetItem* item = videoList->itemAt(point);
    if(action->data().toString() == "play")
    {
       videoDblClicked(item);
    }
    else if(action->data().toString() == "record")
    {
       recordItem(item);
    }
    else if(action->data().toString() == "watch")
    {
        SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
        QDesktopServices::openUrl(QString("http://www.youtube.com/watch?v=%1").arg(svi->videoid));
    }
    else if(action->data().toString() == "link")
    {
        SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
        qApp->clipboard()->setText(QString("http://www.youtube.com/watch?v=%1").arg(svi->videoid));
    }
}

void YTDialog::recordItem(QListWidgetItem *item)
{
    SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
    recording_dialog->downloadVideoId(svi->videoid, svi->header, 0);
}

void YTDialog::playVideo(QString file) 
{
    QString exec = players.currentPlayer().executable();
    qDebug("YTDialog::playVideo: command: '%s'", exec.toUtf8().constData());
    QProcess::startDetached(exec, QStringList() << file);
}

void YTDialog::playYTUrl(const QMap<int, QString> &qualityMap, QString title, QString id)
{
    QString url;
    switch( recording_dialog->recordingQuality() )
    {
    case YTDialog::FullHD:
        url = qualityMap.value(YTDialog::FullHD, QString());
        if(!url.isNull()) break;
    case YTDialog::HD:
        url = qualityMap.value(YTDialog::HD, QString());
        if(!url.isNull()) break;
    case YTDialog::Normal:
        url = qualityMap.value(YTDialog::Normal, QString());
    }

    qDebug("YTDialog::playYTUrl: title: '%s', url: '%s'", title.toUtf8().constData(), url.toUtf8().constData());

    QString exec = players.currentPlayer().executable();
    qDebug("YTDialog::playYTUrl: command: '%s'", exec.toUtf8().constData());
    QStringList args;
    args << url;
    QString title_opt = players.currentPlayer().titleOption();
    if (!title_opt.isEmpty()) {
        if (title_opt.endsWith(" ")) {
            args << title_opt.left(title_opt.length()-1) << title;
        } else {
            args << title_opt + title;
        }
    }
    QProcess::startDetached(exec, args);
}

void YTDialog::handleMessage(const QString& message)
{
    qDebug("YTDialog::handleMessage: '%s'", message.toUtf8().constData());
    if (!isVisible()) show();
    if (message.startsWith("search "))
    {
        QString search_term = message.mid(7);
        qDebug("YTDialog::handleMessage: search_term: '%s'", search_term.toUtf8().constData());
        setSearchTerm(search_term);
    }
}

void YTDialog::showErrorDialog(const QString & error) 
{
    QMessageBox::warning(this, tr("Error"), error);
}

void YTDialog::showAboutDialog() 
{
    About d(this);
    d.exec();
}

void YTDialog::showConfigDialog() 
{
    qDebug("YTDialog::showConfigDialog");

    ConfigDialog d(this);
    d.setRecordingDirectory(recording_dialog->recordingsDirectory());
    d.setRecordingFormat(recording_dialog->recordingFormat());
    d.setRecordingQuality(recording_dialog->recordingQuality());
    d.setPlayerNames( players.availablePlayers() );
    d.setPlayer( players.currentPlayer().name() );

    if (d.exec() == QDialog::Accepted) {
        recording_dialog->setRecordingsDirectory(d.recordingDirectory());
        recording_dialog->setRecordingFormat(d.recordingFormat());
        recording_dialog->setRecordingQuality(d.recordingQuality());
        players.setCurrent( players.findName( d.player() ) );
        saveConfig();
    }
}

void YTDialog::loadConfig() 
{
    QString recording_directory;
    if (set) {
        set->beginGroup("general");
        recording_directory = set->value("recording_directory", recording_dialog->recordingsDirectory()).toString();
        recording_dialog->setRecordingFormat(set->value("recording_format", recording_dialog->recordingFormat()).toInt());
        recording_dialog->setRecordingQuality(set->value("recording_quality", recording_dialog->recordingQuality()).toInt());
        players.setCurrent(set->value("player", players.current()).toInt());
        set->endGroup();
    }

    if (!recording_directory.isEmpty()) {
        recording_dialog->setRecordingsDirectory(recording_directory);
    } else {
#ifdef PORTABLE_APP
        recording_dialog->setRecordingsDirectory(qApp->applicationDirPath());
#else
        QString mdir = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
        if (mdir.isEmpty()) mdir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
        if (mdir.isEmpty()) mdir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
        if (mdir.isEmpty()) mdir = "/tmp";
        if (!QFile::exists(mdir)) {
            qWarning("YTDialog::loadConfig: folder '%s' does not exist. Using /tmp as fallback", mdir.toUtf8().constData());
            mdir = "/tmp";
        }
        QString default_recording_folder = mdir + "/Youtube";
        if (!QFile::exists(default_recording_folder)) {
            qDebug("YTDialog::loadConfig: creating '%s'", default_recording_folder.toUtf8().constData());
            if (!QDir().mkdir(default_recording_folder)) {
                qWarning("YTDialog::YTDialog: failed to create '%s'", default_recording_folder.toUtf8().constData());
            }
        }
        recording_dialog->setRecordingsDirectory(default_recording_folder);
#endif
    }
}

void YTDialog::saveConfig() 
{
    if (set) {
        set->beginGroup("general");
        set->setValue("recording_directory", recording_dialog->recordingsDirectory());
        set->setValue("recording_format", recording_dialog->recordingFormat());
        set->setValue("recording_quality", recording_dialog->recordingQuality());
        set->setValue("player", players.current());
        set->endGroup();
        set->sync();
    }
}

#include "moc_ytdialog.cpp"
