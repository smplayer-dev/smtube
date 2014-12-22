/*  smtube, a small youtube browser.
    Copyright (C) 2012-2014 Ricardo Villalba <rvm@users.sourceforge.net>
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
#include <QInputDialog>
#include "ytdialog.h"
#include "yttabbar.h"
#include "ytdelegate.h"
#include "ytdataapi.h"
#include "rvu.h"
#include "myborder.h"
#include "searchbox.h"
#include "recordingdialog.h"
#include "proxy.h"
#include "configdialog.h"
#include "regions.h"
#include "about.h"

#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#endif

#ifdef YT_USE_SCRIPT
#include "ytsig.h"
#include "codedownloader.h"
#endif

#include "fontpref.h"

#ifdef Q_WS_AMIGA // zzd10h
#include "erroramiga.h"
#endif

int FontPref::base_size = 11;


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
        stages[i] = QPixmap((QString(":/icons/stage%1").arg(i+1, 2, 10, QChar('0'))));
    }
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(frameHasChanged(int)));
}

void OverlayWidget::paintEvent(QPaintEvent * /*e*/)
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
    font.setPixelSize(FontPref::base_size + 1);
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

void YTButton::enterEvent(QEvent * /*e*/)
{
    hovered = true;
    update();
}

void YTButton::leaveEvent(QEvent * /*e*/)
{
    hovered = false;
    update();
}

void YTButton::paintEvent(QPaintEvent * /*e*/)
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
    playback_quality = RetrieveYoutubeUrl::MP4_720p;

    setWindowIcon( QPixmap(":/icons/logo.png") );
    setAutoFillBackground(true);
    setWindowTitle(tr("YouTube Browser - SMPlayer"));
    tabBar = new YTTabBar(this);
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(gotCurrentTab(int)));
    overlay = new OverlayWidget(this);
    videoList = new SmoothListWidget(this);
    pixmap_loader = new PixmapLoader;

#ifdef YT_DL
    recording_dialog = new RecordingDialog(0, settings);
    recording_dialog->setRecordingsDirectory("");
    recording_dialog->setRecordingQuality(RetrieveYoutubeUrl::MP4_720p);
#endif

    MyBorder* border = new MyBorder(this);
    border->setBGColor(palette().color(backgroundRole()));

    videoList->setFrameShape(QFrame::NoFrame);
    delegate = new YTDelegate(videoList);
    videoList->setItemDelegate(delegate);    
    //videoList->setMouseTracking(true);
    //videoList->viewport()->installEventFilter(this);
    videoList->setContextMenuPolicy(Qt::CustomContextMenu);
    //videoList->setAutoScroll(false);

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

    if (!qApp->isLeftToRight()) {
        nextButton->setIcon(QPixmap::fromImage(QPixmap(":/icons/next.png").toImage().mirrored(true, false)));
        prevButton->setIcon(QPixmap::fromImage(QPixmap(":/icons/previous.png").toImage().mirrored(true, false)));
    }

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

#ifdef YT_DL
    recordingButton = new QToolButton(this);
    recordingButton->setIcon(QPixmap(":/icons/recordings.png"));
    recordingButton->setToolTip(tr("Show recordings"));
    recordingButton->setShortcut(QKeySequence("Alt+R"));
    connect(recordingButton, SIGNAL(clicked()), recording_dialog, SLOT(show()));

    enterUrlButton = new QToolButton(this);
    enterUrlButton->setIcon(QPixmap(":/icons/download.png"));
    enterUrlButton->setToolTip(tr("Enter URL"));
    enterUrlButton->setShortcut(QKeySequence("Alt+U"));
    connect(enterUrlButton, SIGNAL(clicked()), this, SLOT(enterUrl()));
#endif

    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(searchBox);
#ifdef YT_DL
    hbox->addWidget(recordingButton);
    hbox->addWidget(enterUrlButton);
#endif
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
    connect(videoList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(videoDblClicked(QListWidgetItem*)));
    connect(videoList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(videoItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(videoList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

#ifdef YT_DL
    connect(recording_dialog, SIGNAL(playFile(QString)), this, SLOT(playVideo(QString)));
    connect(recording_dialog, SIGNAL(signatureNotFound(const QString &)),
            this, SLOT(showErrorSignatureNotFound(const QString &)));
    connect(recording_dialog, SIGNAL(noSslSupport()),
            this, SLOT(showErrorNoSslSupport()));
#endif

    /*
    connect(this, SIGNAL(gotUrls(QMap<int,QString>, QString, QString)), 
            this, SLOT(playYTUrl(QMap<int,QString>,QString, QString)));
    */

#ifdef YT_DL
    setTabOrder(searchBox, recordingButton);
    setTabOrder(recordingButton, enterUrlButton);
    setTabOrder(enterUrlButton, configButton);
#else
    setTabOrder(searchBox, configButton);
#endif
    setTabOrder(configButton, infoButton);
    setTabOrder(infoButton, prevButton);
    setTabOrder(prevButton, nextButton);
    setTabOrder(nextButton, tabBar);
    setTabOrder(tabBar, videoList);

    // Key shortcuts to change the font size
    QAction * incFontSizeAct = new QAction(this);
    incFontSizeAct->setShortcut(QKeySequence::ZoomIn);
    connect(incFontSizeAct, SIGNAL(triggered()), this, SLOT(incFontSize()));
    addAction(incFontSizeAct);

    QAction * decFontSizeAct = new QAction(this);
    decFontSizeAct->setShortcut(QKeySequence::ZoomOut);
    connect(decFontSizeAct, SIGNAL(triggered()), this, SLOT(decFontSize()));
    addAction(decFontSizeAct);

    proxy = new Proxy;
    loadConfig();

	move(0, 0);
	resize(400, 500);
	// Load position and size
	if (set) {
		set->beginGroup("main_window");
		QPoint p = set->value("pos", pos()).toPoint();
		QSize s = set->value("size", size()).toSize();
		set->endGroup();
		move(p);
		resize(s);
	}

    searchBox->setFocus();
}

YTDialog::~YTDialog() 
{
	// Save position and size
	if (set) {
		set->beginGroup("main_window");
		set->setValue("pos", pos());
		set->setValue("size", size());
		set->endGroup();
		set->sync();
	}
	saveConfig();
	delete proxy;

    delete pixmap_loader;
#ifdef YT_DL
    delete recording_dialog;
#endif
}

void YTDialog::setLoadingOverlay(bool enable)
{
    overlayVisible = enable;
    if(enable)
    {
        overlay->move(videoList->pos());
        overlay->resize(videoList->size());
        overlay->setText(QString());
        overlay->show();
    }
    else
    {
        overlay->hide();
    }
}

void YTDialog::resizeEvent(QResizeEvent * /*r*/)
{
    if(overlayVisible)
    {
        overlay->move(videoList->pos());
        overlay->resize(videoList->size());
    }
}

/*
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
*/

void YTDialog::setMode(Mode mode)
{
    disconnect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(gotCurrentTab(int)));
    reset();    
    if(mode == Button)
    {
        searchTerm = QString();
        searchBox->setText(searchTerm);
        addTab(Popular);
        addTab(Music);
        addTab(Sport);
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
    else if(tab == Music)
    {
        index = tabBar->addTab(tr("Music"));
    }
    else if(tab == Sport)
    {
        index = tabBar->addTab(tr("Sports"));
    }    

#ifdef Q_WS_AMIGA  // zzd10h  
    QStringList args = qApp->arguments();
    if(tab != Relevant && args.count() )
        tabBar->setTabEnabled(index,false);
#endif

    tabBar->setTabData(index, tab);
}

void YTDialog::gotCurrentTab(int index)
{
    Tabs tab = static_cast<Tabs>(tabBar->tabData(index).toInt());
    //Title text
    switch((int)tab)
    {
    case Popular: setWindowTitle(tr("Most popular videos on YouTube%1").arg(QChar(0x2122)));break;
    case Sport: setWindowTitle(tr("Most popular sport videos on YouTube%1").arg(QChar(0x2122)));break;
    case Music: setWindowTitle(tr("Most popular music videos on YouTube%1").arg(QChar(0x2122)));break;
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
        case Sport: resultForTab[api->getMost(Sport)]= Sport; break;
        case Music: resultForTab[api->getMost(Music)]= Music; break;
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

    if ((error) && (!searchTerm.isEmpty())) setMode(YTDialog::Button);
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

void YTDialog::videoItemChanged(QListWidgetItem * current, QListWidgetItem * previous) {
    //qDebug("YTDialog::videoItemChanged");
    if (previous) previous->setData(Clicked, false);
    if (current) current->setData(Clicked, true);
}

void YTDialog::videoDblClicked(QListWidgetItem *item)
{
    SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
#ifdef USE_PLAYERS
    bool direct_play = players.currentPlayer().directPlay();
#else
    bool direct_play = HCPLAYER_DIRECTPLAY;
#endif
    if (!direct_play) {
        RetrieveVideoUrl* rvu = new RetrieveVideoUrl(this);
        rvu->setPreferredQuality((RetrieveYoutubeUrl::Quality) playback_quality);
        #ifdef YT_USE_SCRIPT
        YTSig::setScriptFile(script_file);
        #endif
        connect(rvu, SIGNAL(gotPreferredUrl(const QString &, QString, QString)),
                this, SLOT(playYTUrl(const QString &, QString, QString)));
        connect(rvu, SIGNAL(signatureNotFound(const QString &)), 
                this, SLOT(showErrorSignatureNotFound(const QString &)));
        connect(rvu, SIGNAL(noSslSupport()), this, SLOT(showErrorNoSslSupport()));
        connect(rvu, SIGNAL(gotPreferredUrl(const QString &, QString, QString)),
                rvu, SLOT(deleteLater()));
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
#ifdef YT_DL
    menu.addAction(tr("&Record video"))->setData("record");
    menu.addAction(tr("R&ecord audio"))->setData("record_audio");
#endif
    menu.addAction(tr("&Watch on YouTube"))->setData("watch");
    menu.addAction(tr("&Copy link"))->setData("link");
 #ifndef Q_WS_AMIGA // zzd10h
    menu.addAction(tr("&Add URL to the SMPlayer playlist"))->setData("playlist");
 #endif

    QAction* action = menu.exec(videoList->viewport()->mapToGlobal(point));
    if(!action) return;

    QListWidgetItem* item = videoList->itemAt(point);
    if(action->data().toString() == "play")
    {
       videoDblClicked(item);
    }
#ifdef YT_DL
    else if(action->data().toString() == "record")
    {
       recordItem(item);
    }
    else if(action->data().toString() == "record_audio")
    {
       recordAudioItem(item);
    }
#endif
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
    else if(action->data().toString() == "playlist")
    {
        SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
        addToPlaylist(QString("http://www.youtube.com/watch?v=%1").arg(svi->videoid));
    }
}

#ifdef YT_DL
void YTDialog::recordItem(QListWidgetItem *item)
{
    #ifdef YT_USE_SCRIPT
    YTSig::setScriptFile(script_file);
    #endif

    SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
    recording_dialog->downloadVideoId(svi->videoid, svi->header, 0);
}

void YTDialog::recordAudioItem(QListWidgetItem *item)
{
    #ifdef YT_USE_SCRIPT
    YTSig::setScriptFile(script_file);
    #endif

    SingleVideoItem* svi = item->data(0).value<SingleVideoItem*>();
    recording_dialog->downloadAudioId(svi->videoid, svi->header, 0);
}

void YTDialog::downloadUrl(const QString & url) {
	#ifdef YT_USE_SCRIPT
	YTSig::setScriptFile(script_file);
	#endif
	recording_dialog->downloadUrl(url);
}

void YTDialog::enterUrl() {
	qDebug("YTDialog::enterUrl");
	QString url = QInputDialog::getText(this, tr("Enter a URL"),
                                        tr("Enter a Youtube URL to record"));
	if (!url.isEmpty()) {
		RetrieveYoutubeUrl r(this);
		QString full_url = r.fullUrl(url);
		qDebug("RecordingDialog::enterUrl: full_url: %s", full_url.toUtf8().constData());
		if (!full_url.isEmpty()) {
			downloadUrl(full_url);
		}
	}
}
#endif // YT_DL

void YTDialog::playVideo(QString file) 
{
#ifdef USE_PLAYERS
	QString exec = players.currentPlayer().executable();
#else
	QString exec = HCPLAYER_EXECUTABLE;
#endif
	qDebug("YTDialog::playVideo: command: '%s'", exec.toUtf8().constData());

	QStringList args;
 #ifdef Q_WS_AMIGA // zzd10h
	args << "file";
 #endif
	args << file;
	QProcess::startDetached(exec, args);
}

void YTDialog::playYTUrl(const QString & url, QString title, QString /*id*/)
{
    qDebug("YTDialog::playYTUrl: title: '%s', url: '%s'", title.toUtf8().constData(), url.toUtf8().constData());

#ifdef USE_PLAYERS
    QString exec = players.currentPlayer().executable();
    QString title_opt = players.currentPlayer().titleOption();
#else
    QString exec = HCPLAYER_EXECUTABLE;
    QString title_opt = HCPLAYER_TITLEOPTION;
#endif
    qDebug("YTDialog::playYTUrl: command: '%s'", exec.toUtf8().constData());
    QStringList args;
 #ifdef Q_WS_AMIGA // zzd10h
    args << "stream";
 #endif
    args << url;
    if (!title_opt.isEmpty()) {
        if (title_opt.endsWith(" ")) {
            args << title_opt.left(title_opt.length()-1) << title;
        } else {
            args << title_opt + title;
        }
    }
    QProcess::startDetached(exec, args);
}

void YTDialog::addToPlaylist(const QString &url) {
	qDebug("YTDialog::addToPlaylist: %s", url.toUtf8().constData());

#ifdef USE_PLAYERS
	int p = players.findName("SMPlayer");
	if (p > -1) {
		QString exec = players.item(p).executable();
		qDebug("YTDialog::playVideo: command: '%s'", exec.toUtf8().constData());

		QStringList args;
		args << "-add-to-playlist" << url;
		QProcess::startDetached(exec, args);
	} else {
		qDebug("YTDialog::addToPlaylist: player not found");
	}
#else
	QString exec = HCPLAYER_EXECUTABLE;
	if (exec.contains("smplayer")) {
		QStringList args;
		args << "-add-to-playlist" << url;
		QProcess::startDetached(exec, args);
	} else {
		qDebug("YTDialog::addToPlaylist: smplayer not found");
	}
#endif
}

#ifdef USE_SINGLE_APPLICATION
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
	#ifdef YT_DL
    else
    if (message.startsWith("download "))
    {
        QString download_url = message.mid(9);
        qDebug("YTDialog::handleMessage: download_url: '%s'", download_url.toUtf8().constData());
        downloadUrl(download_url);
    }
	#endif
}
#endif

void YTDialog::showErrorDialog(const QString & error) 
{
 #ifndef Q_WS_AMIGA  // zzd10h
    QMessageBox::warning(this, tr("Error"), error);
 #else
    ErrorAmiga::showError(tr("Error"), error);
 #endif
}

void YTDialog::showErrorNoSslSupport() {
	qDebug("YTDialog::showErrorNoSslSupport");
	QMessageBox::warning(this, tr("Connection failed"),
		tr("The video you requested needs to open a HTTPS connection.") +"<br>"+
		tr("Unfortunately the openssl component, required for it, it's not available in your system.") +"<br>"+
		tr("Please, visit %1 to know how to fix this problem.")
			.arg("<a href=\"http://smplayer.sourceforge.net/openssl.php\">" + tr("this link") + "</a>") );
}

void YTDialog::showErrorSignatureNotFound(const QString & title) {
	qDebug("YTDialog::showErrorSignatureNotFound: %s", title.toUtf8().constData());

	QString t = title;
	t.replace(" - YouTube", "");

	#ifdef YT_USE_SCRIPT
	static CodeDownloader * downloader = 0;
	int ret = QMessageBox::question(this, tr("Problems with Youtube"),
				tr("Unfortunately due to changes in Youtube, the video '%1' can't be played or recorded.").arg(t) + "<br><br>" +
				tr("Do you want to update the Youtube code? This may fix the problem."),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (ret == QMessageBox::Yes) {
		if (!downloader) downloader = new CodeDownloader(this);
		downloader->saveAs(script_file);
		downloader->show();
		downloader->download(QUrl("http://updates.smplayer.info/yt.js"));
	}
	#else
	QMessageBox::warning(this, tr("Problems with Youtube"),
		tr("Unfortunately due to changes in Youtube, the video '%1' can't be played or recorded.").arg(t) + "<br><br>" +
		tr("Maybe updating this application could fix the problem."));
	#endif
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
#ifdef YT_DL
    d.setRecordingDirectory(recording_dialog->recordingsDirectory());
    d.setRecordingQuality(recording_dialog->recordingQuality());
#endif
#ifdef USE_PLAYERS
    d.setPlayerNames( players.availablePlayers() );
    d.setPlayer( players.currentPlayer().name() );
#endif
    d.setPlaybackQuality( playback_quality );

    QString period = api->period();
    QString region = api->region();

    d.setPeriod( period );
    d.setRegion( region );

	d.setUseProxy(proxy->useProxy());
	d.setProxyHostname(proxy->host());
	d.setProxyPort(proxy->port());
	d.setProxyUsername(proxy->username());
	d.setProxyPassword(proxy->password());
	d.setProxyType(proxy->type());

    if (d.exec() == QDialog::Accepted) {
        #ifdef YT_DL
        recording_dialog->setRecordingsDirectory(d.recordingDirectory());
        recording_dialog->setRecordingQuality(d.recordingQuality());
        #endif
        #ifdef USE_PLAYERS
        int p = players.findName(d.player());
        if (p == -1) p = 0;
        players.setCurrent(p);
        #endif
        playback_quality = d.playbackQuality();
        api->setPeriod( d.period() );
        api->setRegion( d.region() );

		proxy->setUseProxy(d.useProxy());
		proxy->setHost(d.proxyHostname());
		proxy->setPort(d.proxyPort());
		proxy->setUsername(d.proxyUsername());
		proxy->setPassword(d.proxyPassword());
		proxy->setType(d.proxyType());
		proxy->applyProxy();

        saveConfig();

        if ((d.period() != period) || (d.region() != region)) {
            setMode(YTDialog::Button); // Refresh list
        }
    }
}

void YTDialog::loadConfig() 
{
#ifdef YT_DL
    QString recording_directory;
#endif
    if (set) {
        set->beginGroup("General");
        #ifdef YT_DL
        recording_directory = set->value("recording_directory", recording_dialog->recordingsDirectory()).toString();
        recording_dialog->setRecordingQuality(set->value("record_quality", recording_dialog->recordingQuality()).toInt());
        #endif
#ifdef USE_PLAYERS
        players.setCurrent(set->value("player", players.current()).toInt());
#endif
        // Find user's region to be used as default
        //QLocale locale(QLocale::German, QLocale::Austria); // TEST
        QLocale locale = QLocale::system();
        QString local_region = Regions::findRegionForLocale(locale);
        qDebug("YTDialog::loadConfig: default region: %s", local_region.toUtf8().constData());
        api->setRegion(set->value("region", local_region).toString());
        api->setPeriod(set->value("period", "today").toString());
        playback_quality = set->value("playback_quality", playback_quality).toInt();
        /* RetrieveYoutubeUrl::setUserAgent(set->value("user_agent", "Mozilla/5.0 (X11; Linux x86_64; rv:5.0.1) Gecko/20100101 Firefox/5.0.1").toString()); */
        RetrieveYoutubeUrl::setUserAgent(set->value("user_agent", "").toString());
        RetrieveYoutubeUrl::setUseHttpsMain(set->value("use_https_main", false).toBool());
        RetrieveYoutubeUrl::setUseHttpsVi(set->value("use_https_vi", false).toBool());
        FontPref::base_size = set->value("font_base_size", FontPref::base_size).toInt();

        int config_version = set->value("config_version", 0).toInt();
        if (config_version < 1) {
            qDebug("YTDialog::loadConfig: reseting some options");
            config_version = 1;
            set->setValue("config_version", config_version);
            RetrieveYoutubeUrl::setUserAgent("");
        }
        set->endGroup();

        proxy->load(set);
        proxy->applyProxy();
    }

#ifdef YT_DL
    if (!recording_directory.isEmpty()) {
        recording_dialog->setRecordingsDirectory(recording_directory);
    } else {
#ifdef PORTABLE_APP
        recording_dialog->setRecordingsDirectory(qApp->applicationDirPath());
#else
 #ifdef Q_WS_AMIGA // zzd10h
        QDir currentDir ;
        QString mdir = currentDir.absolutePath() + "/Amiga/Videos";
        if (!QFile::exists(mdir)) {
            qWarning("YTDialog::loadConfig: folder '%s' does not exist. Using \"RAM Disk:\" as fallback", mdir.toUtf8().constData());
            mdir = "/RAM Disk/";
        }
        QString default_recording_folder = mdir ;
 #else
        QString mdir;
        #if QT_VERSION >= 0x050000
        mdir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        if (mdir.isEmpty()) mdir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        if (mdir.isEmpty()) mdir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        #else
        mdir = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
        if (mdir.isEmpty()) mdir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
        if (mdir.isEmpty()) mdir = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
        #endif
        if (mdir.isEmpty()) mdir = "/tmp";
        if (!QFile::exists(mdir)) {
            qWarning("YTDialog::loadConfig: folder '%s' does not exist. Using /tmp as fallback", mdir.toUtf8().constData());
            mdir = "/tmp";
        }
        QString default_recording_folder = mdir + QDir::separator() + "Youtube";
 #endif
        if (!QFile::exists(default_recording_folder)) {
            qDebug("YTDialog::loadConfig: creating '%s'", default_recording_folder.toUtf8().constData());
            if (!QDir().mkdir(default_recording_folder)) {
                qWarning("YTDialog::YTDialog: failed to create '%s'", default_recording_folder.toUtf8().constData());
            }
        }
        recording_dialog->setRecordingsDirectory(default_recording_folder);
#endif
    }
#endif // YT_DL
}

void YTDialog::saveConfig() 
{
    if (set) {
        set->beginGroup("General");
        #ifdef YT_DL
        set->setValue("recording_directory", recording_dialog->recordingsDirectory());
        set->setValue("record_quality", recording_dialog->recordingQuality());
        #endif
        #ifdef USE_PLAYERS
        set->setValue("player", players.current());
        #endif
        set->setValue("region", api->region());
        set->setValue("period", api->period());
        set->setValue("playback_quality", playback_quality);
        set->setValue("user_agent", RetrieveYoutubeUrl::userAgent());
        set->setValue("use_https_main", RetrieveYoutubeUrl::useHttpsMain());
        set->setValue("use_https_vi", RetrieveYoutubeUrl::useHttpsVi());

        set->setValue("font_base_size", FontPref::base_size);
        set->endGroup();

        proxy->save(set);

        set->sync();
    }
}

void YTDialog::incFontSize() {
    qDebug("YTDialog::incFontSize");
    FontPref::base_size++;
    updateNextPrevWidget();
}

void YTDialog::decFontSize() {
    qDebug("YTDialog::decFontSize");
    FontPref::base_size--;
    updateNextPrevWidget();
}

#include "moc_ytdialog.cpp"
