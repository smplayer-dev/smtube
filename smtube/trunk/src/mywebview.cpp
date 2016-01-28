/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "mywebview.h"
#include <QContextMenuEvent>
#include <QWebHitTestResult>
#include <QMenu>
#include <QDesktopServices>
#include <QWebElement>
#include <QDebug>

#include "supportedurls.h"

MyWebView::MyWebView(QWidget * parent)
	:QWebView(parent)
	,context_menu(0)
#ifdef USE_PLAYERS
	,audio_menu(0)
#endif
{
	openLinkInExternalBrowserAct = new QAction(tr("Open link in a web browser"), this);
	connect(openLinkInExternalBrowserAct, SIGNAL(triggered()), this, SLOT(openLinkInExternalBrowser()));
}

void MyWebView::contextMenuEvent(QContextMenuEvent* event) {
	qDebug("MyWebView::contextMenuEvent");
	QWebHitTestResult hit_test = page()->mainFrame()->hitTestContent(event->pos());
	QUrl url = hit_test.linkUrl();
	qDebug() << "MyWebView::contextMenuEvent: url:" << url.toString();

	// Translate and adjust some actions
	pageAction(QWebPage::DownloadLinkToDisk)->setEnabled(false);
	pageAction(QWebPage::DownloadLinkToDisk)->setText(tr("Save link..."));
	pageAction(QWebPage::CopyLinkToClipboard)->setText(tr("Copy link to clipboard"));
	pageAction(QWebPage::OpenLink)->setText(tr("Open link in this window"));
	pageAction(QWebPage::Copy)->setText(tr("Copy text to clipboard"));
	pageAction(QWebPage::OpenLinkInNewWindow)->setText(tr("Open link in new window"));

	if (url.isEmpty()) {
		QWebView::contextMenuEvent(event);
	} else {
		int site_type = SupportedUrls::site(url.toString());
		if (site_type != SupportedUrls::Unsupported) {
			createContextMenu(site_type, url);
			context_menu->exec(mapToGlobal(QPoint(event->x(),event->y())));
		} else {
			QWebView::contextMenuEvent(event);
		}
	}
}

void MyWebView::createContextMenu(int site_id, const QUrl & url) {
	if (context_menu == 0) {
		context_menu = new QMenu(this);
	}
	context_menu->clear();

#ifdef USE_PLAYERS
	if (audio_menu == 0) {
		audio_menu = new QMenu(tr("Open audio with"), this);
	}
	audio_menu->clear();

	for (int n = 0; n < player_list.count(); n++) {
		bool add_this_player = false;

		if (site_id == SupportedUrls::Youtube) {
			add_this_player = true;
		}
		else
		if (site_id == SupportedUrls::StreamingSite && player_list[n].supportStreamingSites()) {
			add_this_player = true;
		}
		else
		if (site_id == SupportedUrls::DirectStream && player_list[n].supportOnlineTV()) {
			add_this_player = true;
		}

		if (add_this_player) {
			Player::Media media = player_list[n].supportedMedia();
			if (media == Player::Video || media == Player::VideoAudio) {
				QAction * videoAct = new QAction(this);
				connect(videoAct, SIGNAL(triggered()), this, SLOT(openWithTriggered()));
				videoAct->setText(tr("Open with %1").arg(player_list[n].name()));
				videoAct->setData(QStringList() << player_list[n].name() << url.toString());
				context_menu->addAction(videoAct);
			}

			// Audio
			if (site_id == SupportedUrls::Youtube) {
				if (media == Player::Audio || media == Player::VideoAudio) {
					QAction * audioAct = new QAction(this);
					connect(audioAct, SIGNAL(triggered()), this, SLOT(openAudioWithTriggered()));
					audioAct->setText(player_list[n].name());
					audioAct->setData(QStringList() << player_list[n].name() << url.toString());
					audio_menu->addAction(audioAct);
				}
			}
		}
	}

	if (!audio_menu->isEmpty()) {
		context_menu->addMenu(audio_menu);
	}

#else
	QAction * videoAct = new QAction(this);
	connect(videoAct, SIGNAL(triggered()), this, SLOT(openWithTriggered()));
	videoAct->setText(tr("Open with %1").arg(player_name));
	videoAct->setData(QStringList() << player_name << url.toString());
	context_menu->addAction(videoAct);

	// Audio
	QAction * audioAct = new QAction(this);
	connect(audioAct, SIGNAL(triggered()), this, SLOT(openAudioWithTriggered()));
	audioAct->setText(tr("Open audio with %1").arg(player_name));
	audioAct->setData(QStringList() << player_name << url.toString());
	context_menu->addAction(audioAct);
#endif

	context_menu->addSeparator();

	QAction *copy_link = pageAction(QWebPage::CopyLinkToClipboard);
	QAction *open_link = pageAction(QWebPage::OpenLink);
	QAction *copy_text = pageAction(QWebPage::Copy);

	openLinkInExternalBrowserAct->setData(url.toString());

	context_menu->addAction(copy_link);
	context_menu->addAction(open_link);
	context_menu->addAction(openLinkInExternalBrowserAct);
	context_menu->addAction(copy_text);
}

void MyWebView::openWithTriggered() {
	qDebug() << "MyWebView::openWithTriggered";
	QAction * a = qobject_cast<QAction *>(sender());
	if (a) {
		QStringList data = a->data().toStringList();
		if (data.count() == 2) {
			QString player = data[0];
			QString url = data[1];
			qDebug() << "MyWebView::openWithTriggered: player:" << player;
			emit requestedOpenWith(player, url);
		}
	}
}

void MyWebView::openAudioWithTriggered() {
	qDebug() << "MyWebView::openAudioWithTriggered";
	QAction * a = qobject_cast<QAction *>(sender());
	if (a) {
		QStringList data = a->data().toStringList();
		if (data.count() == 2) {
			QString player = data[0];
			QString url = data[1];
			qDebug() << "MyWebView::openWithTriggered: player:" << player;
			emit requestedOpenAudioWith(player, url);
		}
	}
}

void MyWebView::openLinkInExternalBrowser() {
	QString url = openLinkInExternalBrowserAct->data().toString();
	qDebug() << "MyWebView::openLinkInExternalBrowser: url:" << url;
	QDesktopServices::openUrl(url);
}

QWebView * MyWebView::createWindow(QWebPage::WebWindowType type) {
	qDebug() << "MyWebView::createWindow";
	QWebHitTestResult result = page()->mainFrame()->hitTestContent(last_click);
	QUrl url = result.linkUrl();
	qDebug() << "MyWebView::createWindow: url:" << url.toString();

	if (!url.isEmpty()) {
		QDesktopServices::openUrl(url);
	}

	return 0;
}

void MyWebView::mousePressEvent(QMouseEvent * event) {
	//qDebug() << "MyWebView::mousePressEvent";
	last_click = event->pos();
	if (event->button() == Qt::LeftButton) {
		QWebHitTestResult result = page()->mainFrame()->hitTestContent(event->pos());
		last_clicked_url = result.linkUrl();
		last_clicked_target = result.linkElement().attribute("target", "");
		qDebug() << "MyWebView::mousePressEvent: link:" << last_clicked_url.toString();
		qDebug() << "MyWebView::mousePressEvent: target:" << last_clicked_target;
	}
	QWebView::mousePressEvent(event);
}

#include "moc_mywebview.cpp"
