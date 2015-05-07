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

#include "players.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>

//#define DEBUG_FIND_EX

#ifdef Q_OS_LINUX
QString Player::findExecutable(const QString & name) {
	QByteArray env = qgetenv("PATH");
	QStringList search_paths = QString::fromLocal8Bit(env.constData()).split(':', QString::SkipEmptyParts);
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
	return QString::null;
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

#ifdef Q_OS_LINUX
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
	list.push_back( Player("SMPlayer", "smplayer.exe", "%u", true) );
	/*
	list.push_back( Player("VLC", "C:\\Program Files\\VideoLAN\\VLC\\vlc.exe", "%u --meta-title=%t", false) );
	*/
#else
	list.push_back( Player("SMPlayer", "smplayer", "%u", true) );
	list.push_back( Player("MPlayer", "mplayer", "%u -title %t", false) );
	list.push_back( Player("VLC", "vlc", "%u --meta-title=%t", false) );
	list.push_back( Player("Dragon Player", "dragon", "%u", false) );
	list.push_back( Player("Totem", "totem", "%u", false) );
	list.push_back( Player("GNOME-MPlayer", "gnome-mplayer", "%u", false) );
	list.push_back( Player("mpv", "mpv", "%u --title=%t", false) );
	/*
	list.push_back( Player("mpv (0.8.3)", "mpv-0.8.3", "%u", true) );
	list.push_back( Player("uget", "uget-gtk", "--quiet --folder=/tmp --filename=%f %u", false) );
	*/
#endif
	curr = 0;
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
		set->setValue("directplay", list[n].directPlay());
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
			bool directplay = set->value("directplay", false).toBool();
			list.push_back( Player(name, binary, arguments, directplay) );
			set->endGroup();
		}
	}

	set->endGroup();
}
