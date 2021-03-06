/*  smtube, a small youtube browser.
    Copyright (C) 2012-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#include "players.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include "qtcompat.h"

//#define D_PLAYERS
//#define DEBUG_FIND_EX

#ifdef OS_UNIX_NOT_MAC
QString Player::findExecutable(const QString & name) {
	QByteArray env = qgetenv("PATH");
	QStringList search_paths = QString::fromLocal8Bit(env.constData()).split(':', QTC_SkipEmptyParts);
	for (int n = 0; n < search_paths.count(); n++) {
		QString candidate = search_paths[n] + "/" + name;
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::findExecutable: candidate:" << candidate;
		#endif
		QFileInfo info(candidate);
		if (info.isFile() && info.isExecutable()) {
			#ifdef DEBUG_FIND_EX
			qDebug() << "Player::findExecutable: executable found:" << candidate;
			#endif
			return candidate;
		}
	}
	return QString();
}
#endif

QString Player::executable(bool * found) {
	if (found) *found = false;

	QString bin = player_bin;

	QFileInfo fi(bin);
	if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
		if (found) *found = true;
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: found " << bin;
		#endif
		return bin;
	} else {
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: not found " << bin;
		#endif
	}

	bin = qApp->applicationDirPath() + "/" + bin;
	fi.setFile(bin);
	if (fi.exists() && fi.isExecutable() && !fi.isDir()) {
		if (found) *found = true;
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: found " << bin;
		#endif
		return bin;
	} else {
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: not found " << bin;
		#endif
	}

#ifdef OS_UNIX_NOT_MAC
	bin = findExecutable(player_bin);
	if (!bin.isEmpty()) {
		if (found) *found = true;
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: found " << bin;
		#endif
		return bin;
	} else {
		#ifdef DEBUG_FIND_EX
		qDebug() << "Player::executable: executable for" << player_bin << "not found anywhere!";
		#endif
	}
#endif

	return player_bin;
}


Players::Players() {
#ifdef Q_OS_WIN
	list << Player("SMPlayer", "smplayer.exe", "%u", true, true, Player::Video)
		 << Player("SMPlayer (audio)", "smplayer.exe", "%u -media-title %t", true, false, Player::Audio)
		 << Player("SMPlayer (add to playlist)", "smplayer.exe", "-add-to-playlist %u", true, true, Player::VideoAudio);
#else
	list << Player("SMPlayer", "smplayer", "%u", true, true, Player::Video)
		 << Player("SMPlayer (audio)", "smplayer", "%u -media-title %t", true, false, Player::Audio)
		 << Player("SMPlayer (add to playlist)", "smplayer", "-add-to-playlist %u", true, true, Player::VideoAudio)
		 << Player("MPlayer", "mplayer", "%u -title %t", false, false, Player::Video)
		 << Player("VLC", "vlc", "%u --meta-title=%t", false, true, Player::VideoAudio)
		 << Player("Dragon Player", "dragon", "%u", false, false, Player::VideoAudio)
		 << Player("Totem", "totem", "%u", false, false, Player::VideoAudio)
		 << Player("GNOME-MPlayer", "gnome-mplayer", "%u", false, false, Player::VideoAudio)
		 << Player("mpv", "mpv", "%u --title=%t", false, true, Player::Video)
		 << Player("mpv + youtube-dl", "mpv", "--ytdl --ytdl-format=best %u", true, true, Player::Video);
	#ifdef D_PLAYERS
	list << Player("uget", "uget-gtk", "--quiet --folder=/tmp --filename=%f %u", false, false, Player::VideoAudio);
	#endif
#endif
	default_list = list;
	curr = 0;
}

void Players::setAllPlayers(QList<Player> players) { 
	list = players;
	available_players_cache.clear();
}

QList<Player> Players::availablePlayers() {
	if (!available_players_cache.isEmpty()) {
		return available_players_cache;
	}

	QList<Player>  l;
	bool found;
	for (int n = 0; n < list.count(); n++) {
		QString exec = list[n].executable(&found);
		if (found) {
			qDebug() << "Players::availablePlayers:" << n << ":" << exec;
			l << list[n];
		}
	}

	available_players_cache = l;
	return l;
}

int Players::findName(QString name) {
	for (int n = 0; n < list.count(); n++) {
		if (list[n].name() == name) return n;
	}
	return -1;
}

void Players::save(QSettings * set) {
	qDebug() << "Players::save";

	set->beginGroup("players");
	set->setValue("count", list.count());

	for (int n = 0; n < list.count(); n++) {
		QString section = QString("player_%1").arg(n);
		set->beginGroup(section);
		set->setValue("name", list[n].name());
		set->setValue("binary", list[n].binary());
		set->setValue("arguments", list[n].arguments());
		set->setValue("directplay", list[n].supportStreamingSites());
		set->setValue("online_tv", list[n].supportOnlineTV());
		set->setValue("supported_media", list[n].supportedMedia());
		set->setValue("resolution", list[n].preferredResolution());
		set->endGroup();
	}

	set->endGroup();
}

void Players::load(QSettings * set) {
	qDebug() << "Players::load";

	set->beginGroup("players");
	int count = set->value("count", 0).toInt();

	if (count > 0) {
		list.clear();

		for (int n = 0; n < count; n++) {
			QString section = QString("player_%1").arg(n);
			set->beginGroup(section);
			QString name = set->value("name", "").toString();
			QString binary = set->value("binary", "").toString();
			QString arguments = set->value("arguments", "").toString();
			bool support_streaming_sites = set->value("directplay", false).toBool();
			int supported_media = set->value("supported_media", Player::VideoAudio).toInt();

			QVariant var = set->value("online_tv");
			bool support_online_tv = false;
			if (var.isValid()) {
				support_online_tv = var.toBool();
			}
			else
			if (supported_media != Player::Audio) {
				#ifdef Q_OS_WIN
				if (binary.contains("smplayer.exe")) support_online_tv = true;
				#else
				if (binary == "smplayer" || binary == "vlc" || binary == "mpv") support_online_tv = true;
				#endif
			}

			int resolution = set->value("resolution", -1).toInt();
			list.push_back( Player(name, binary, arguments, support_streaming_sites, support_online_tv, (Player::Media) supported_media, resolution) );
			set->endGroup();
		}
	}

	set->endGroup();
}
