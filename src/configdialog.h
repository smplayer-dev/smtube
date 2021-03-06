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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "ui_configdialog.h"

#include <QDialog>

#ifdef FONT_CHANGE
#include <QFont>
#endif

#ifdef USE_PLAYERS
#include "players.h"
#endif

#ifdef USE_SITES
#include "sites.h"
#endif

class ConfigDialog : public QDialog, public Ui::ConfigDialog
{
	Q_OBJECT

public:
	ConfigDialog( QWidget * parent = 0, Qt::WindowFlags f = QFlag(0) );
	~ConfigDialog();

#ifdef SELECT_RESOLUTION
	void setPlaybackResolution(int resolution);
	int playbackResolution();
#endif

#ifdef D_BUTTON
	void setAddDownloadButton(bool b);
	bool addDownloadButton();

	void setExternalDownloadUrl(const QString &);
	QString externalDownloadUrl();
#endif

#ifdef FONT_CHANGE
	void setDefaultFont(const QFont & f);
	QFont defaultFont();
#endif

#ifdef STYLE_SWITCHING
	void setStyle(const QString & style);
	QString style();
#endif

#ifdef USE_SITES
	void setSites(QList<Site> list);
	QList<Site> sites() { return site_list; }

	void setCurrentSite(const QString & name);
	QString currentSite();
#endif

#ifdef USE_YT_DL
	void setYtdlBin(const QString & path);
	QString ytdlBin();
#endif

#ifdef USE_PLAYERS
	void setPlayers(QList<Player> list);
	QList<Player> players();

	void setDefaultPlayers(QList<Player> players);

protected slots:
	void on_up_button_clicked();
	void on_down_button_clicked();
	void on_delete_button_clicked();
	void on_add_button_clicked();
	void editCurrentItem();
	void on_addplayers_button_clicked();

	void updateAddPlayersButton();

protected:
	QList<Player> default_players;
#endif

#ifdef USE_SITES
protected slots:
	void on_edit_site_button_clicked();
	void on_add_site_button_clicked();
	void on_delete_site_button_clicked();

protected:
	int find_site(const QString & name);
	void refreshSitesCombo();
	QList<Site> site_list;
#endif

#ifdef FONT_CHANGE
protected slots:
	void on_change_font_button_clicked();
#endif

protected:
	int find_tab(const QString & object_name);
};

#endif
