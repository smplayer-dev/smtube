/*  smtube, a small youtube browser.
    Copyright (C) 2012-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

class QSettings;

class Player
{
public:
	Player(QString name, QString binary, QString arguments, bool directPlay) {
		player_name = name;
		player_bin = binary;
		args = arguments;
		direct_play = directPlay;
	}

	void setName(QString name) { player_name = name; }
	void setBinary(QString binary) { player_bin = binary; }
	void setArguments(QString arguments) { args = arguments; }
	void setDirectPlay(bool b) { direct_play = b; }

	QString name() { return player_name; }
	QString binary() { return player_bin; }
	QString arguments() { return args; };
	bool directPlay() { return direct_play; }

	QString executable(bool * found = 0);

	static QString directPlayToString(bool b);

protected:
#ifdef Q_OS_LINUX
	QString findExecutable(const QString & name);
#endif

	QString player_name, player_bin, args;
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

	QList<Player> availablePlayers();
	int findName(QString name);

	void save(QSettings * set);
	void load(QSettings * set);

	void setAllPlayers(QList<Player> players);
	QList<Player> allPlayers() { return list; }

	QList<Player> defaultPlayers() { return default_list; }

protected:
	QList<Player> list;
	QList<Player> default_list;

	int curr;

	QList <Player> available_players_cache;
};

#endif
