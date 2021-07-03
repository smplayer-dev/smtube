/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

#ifdef USE_PLAYERS
#include "players.h"
#else
#include "hcplayer.h"
#endif

#ifdef USE_SITES
#include "sites.h"
#endif

class QToolBar;
class MyWebView;
class QLineEdit;
class RetrieveYoutubeUrl;
class QAction;
class QSettings;
class CodeDownloader;

class BrowserWindow : public QMainWindow
{
	Q_OBJECT

public:
	BrowserWindow(const QString & config_dir, QWidget * parent = 0, Qt::WindowFlags flags = QFlag(0));
	~BrowserWindow();

public slots:
	void loadUrl(const QUrl& url);
	void loadHomePage() { loadUrl(QUrl(homePageUrl())); };
	void search(const QString & term);

#ifndef USE_QPROCESS
signals:
	void requestOpenUrl(const QString & url);

protected slots:
	void testOpenUrl(const QString & url);
#endif

protected slots:
	void adjustLocation();
	void changeLocation();
	void adjustTitle();
	void setProgress(int p);
	void finishLoading(bool);

	void viewToolbar(bool);
	void viewStatusbar(bool);

	void incZoom();
	void decZoom();

	void processLink(const QUrl & url );

	void openWith(int player_id, const QUrl & url);
	void openWith(const QString & player, const QUrl & url);

#ifdef USE_YT_DL
	void openWithBrowser(const QUrl & url);

	void openYTUrl(QString title, QString extension, const QString & url);
	void openYTUrl(const QString & url, int itag);

	void openAudioWith(const QString & player, const QUrl & url);
	void openYTAudioUrl(const QString & url, int itag);
#endif

	void showAbout();

#ifdef USE_CONFIG_DIALOG
	void showConfigDialog();
#endif

	void showErrorEmptyList();

#ifdef CODEDOWNLOADER
	void updateYTCode();
#endif
	void YTFailedToStart();

protected:
	QString homePageUrl();

	void saveConfig();
	void loadConfig();

#ifdef STYLE_SWITCHING
	void setStyle(QString style);
#endif

#ifdef USE_YT_DL
	void fetchVideoUrl(RetrieveYoutubeUrl * ry, const QUrl & url, int player_id = Undefined);
#endif

private:
	enum PlayerId { Undefined = -1, WebBrowser = -2 };

	MyWebView *view;

	QToolBar *toolbar;
	QLineEdit *location;

#ifdef USE_YT_DL
	RetrieveYoutubeUrl * ryu;
	RetrieveYoutubeUrl * ryua;
	QString ytdl_bin;
#endif

#ifdef SELECT_RESOLUTION
	int preferred_resolution;
#endif

	QAction *toggleToolbarAct;
	QAction *toggleStatusbarAct;

	QSettings * settings;

	bool use_cookies;

#ifdef USE_PLAYERS
	Players players;
#endif
	int current_player;

#ifdef USE_SITES
	Sites sites;
#endif

#ifdef D_BUTTON
	bool add_download_button;
	QString external_download_url;
#endif

#ifdef STYLE_SWITCHING
	QString default_style;
#endif
};

#endif
