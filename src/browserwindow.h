/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015 Ricardo Villalba <rvm@users.sourceforge.net>

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
	BrowserWindow(const QString & config_dir, QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~BrowserWindow();

#ifdef YT_USE_YTSIG
	void setScriptFile(const QString & file) { script_file = file; };
#endif

public slots:
	void loadUrl(const QUrl& url);
	void loadHomePage() { loadUrl(QUrl(home_page)); };
	void search(const QString & term);

protected slots:
	void adjustLocation();
	void changeLocation();
	void adjustTitle();
	void setProgress(int p);
	void finishLoading(bool);

	void viewToolbar(bool);
	void viewStatusbar(bool);

	void processLink(const QUrl & url );

	void openWith(int player_id, const QUrl & url);
	void openWith(const QString & player, const QUrl & url);

	void openYTUrl(QString title, QString extension, const QString & url);
	void openYTUrl(const QString & url, int itag);

	void openAudioWith(const QString & player, const QUrl & url);
	void openYTAudioUrl(const QMap<int, QString>&);

	void showAboutRelease();
	void showAbout();
	void showConfigDialog();

	void showErrorSignatureNotFound(const QString &);
	void showErrorNoSslSupport();
	void showErrorEmptyList();

#ifdef YT_USE_YTSIG
	void updateYTCode();
#endif

protected:
	void saveConfig();
	void loadConfig();

private:
	MyWebView *view;

	QToolBar *toolbar;
	QLineEdit *location;
	RetrieveYoutubeUrl * ryu;
	RetrieveYoutubeUrl * ryua;

	QAction *toggleToolbarAct;
	QAction *toggleStatusbarAct;

	QSettings * settings;

	int preferred_quality;

#ifdef USE_PLAYERS
	Players players;
	int current_player;
#endif

#ifdef YT_USE_YTSIG
	QString script_file;
	CodeDownloader * codedownloader;
#endif

	QString home_page;
};

#endif
