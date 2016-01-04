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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "ui_configdialog.h"

#include <QDialog>

#ifdef USE_PLAYERS
#include "players.h"
#endif

class ConfigDialog : public QDialog, public Ui::ConfigDialog
{
	Q_OBJECT

public:
	ConfigDialog( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~ConfigDialog();

	void setPlaybackQuality(int quality);
	int playbackQuality();

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
};

#endif
