/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QWebView>

#ifdef USE_PLAYERS
#include "players.h"
#endif

class QMenu;
class QAction;

class MyWebView : public QWebView
{
	Q_OBJECT

public:
	MyWebView(QWidget * parent = 0);

#ifdef USE_PLAYERS
	void setPlayers(QList<Player> l) { player_list = l; };
	QList<Player> players() { return player_list; };
#else
	void setPlayer(const QString & player) { player_name = player; };
#endif

	QUrl lastClickedUrl() { return last_clicked_url; }
	QString lastClickedTarget() { return last_clicked_target; }

signals:
	void requestedOpenWith(const QString &, const QUrl &);
	void requestedOpenAudioWith(const QString &, const QUrl &);
	void requestedOpenWithBrowser(const QUrl &);

protected slots:
	void openLinkInExternalBrowser();
	void openWithTriggered();
	void openAudioWithTriggered();
	void openWithBrowserTriggered();

protected:
	void createContextMenu(int site_id, const QUrl & url);
	void contextMenuEvent(QContextMenuEvent* event);

	QWebView *createWindow(QWebPage::WebWindowType type);
	void mousePressEvent(QMouseEvent * event);

private:
	QMenu * context_menu;
	QAction * openLinkInExternalBrowserAct;

#ifdef USE_PLAYERS
	QList<Player> player_list;
	QMenu * audio_menu;
#else
	QString player_name;
#endif

	QPoint last_click;
	QUrl last_clicked_url;
	QString last_clicked_target;
};

#endif
