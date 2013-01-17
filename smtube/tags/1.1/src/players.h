/*  smtube - a small youtube browser
    Copyright (C) 2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef PLAYERS_H
#define PLAYERS_H

#include <QString>
#include <QStringList>
#include <QList>

class Player
{
public:
	Player(QString name, QString binary, bool directPlay, QString title_option = QString::null) { 
		player_name = name;
		player_bin = binary;
		direct_play = directPlay;
		title_opt = title_option;
	}

	void setName(QString name) { player_name = name; }
	void setBinary(QString binary) { player_bin = binary; }
	void setDirectPlay(bool b) { direct_play = b; }
	void setTitleOption(QString s) { title_opt = s; }

	QString name() { return player_name; }
	QString binary() { return player_bin; }
	bool directPlay() { return direct_play; }
	QString titleOption() { return title_opt; }

	QString executable(bool * found = 0);

protected:
	QString player_name, player_bin, title_opt;
	bool direct_play;
};


class Players
{
public:
	Players();

	int count() { return list.count(); }

	void setCurrent( int c ) { curr = c; }
	int current() { return curr; }

	Player item(int i) { return list[i]; }
	Player currentPlayer() { return list[curr]; }

	QStringList availablePlayers();
	int findName(QString name);

protected:
	QList <Player> list;
	int curr;
};

#endif
