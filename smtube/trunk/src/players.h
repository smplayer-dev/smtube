/*  smtube, a small youtube browser.
    Copyright (C) 2012-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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
	enum Media { None = 0, Video = 1, Audio = 2, VideoAudio = 3 };

	Player(QString name, QString binary, QString arguments,
			bool support_for_streaming_sites, bool support_for_online_tv,
			Media media, int preferred_quality = -1)
	{
		player_name = name;
		player_bin = binary;
		args = arguments;
		support_streaming_sites = support_for_streaming_sites;
		support_online_tv = support_for_online_tv,
		supported_media = media;
		quality = preferred_quality;
	}

	void setName(QString name) { player_name = name; }
	void setBinary(QString binary) { player_bin = binary; }
	void setArguments(QString arguments) { args = arguments; }
	void setSupportStreamingSites(bool b) { support_streaming_sites = b; }
	void setSupportOnlineTV(bool b) { support_online_tv = b; }
	void setSupportedMedia(Media m) { supported_media = m; }
	void setPreferredQuality(int q) { quality = q; }

	QString name() { return player_name; }
	QString binary() { return player_bin; }
	QString arguments() { return args; };
	bool supportStreamingSites() { return support_streaming_sites; }
	bool supportOnlineTV() { return support_online_tv; }
	Media supportedMedia() { return supported_media; }
	int preferredQuality() { return quality; }

	QString executable(bool * found = 0);

protected:
#ifdef Q_OS_LINUX
	QString findExecutable(const QString & name);
#endif

	QString player_name, player_bin, args;
	bool support_streaming_sites;
	bool support_online_tv;
	Media supported_media;
	int quality;
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
