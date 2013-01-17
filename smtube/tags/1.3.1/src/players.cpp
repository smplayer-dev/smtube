/*  smtube, a small youtube browser.
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

#include "players.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>

QString Player::executable(bool * found) {
	if (found) *found = true;

	QString bin = qApp->applicationDirPath() + "/" + player_bin;
	#ifdef Q_OS_WIN
	bin += ".exe";
	#endif

#ifndef Q_OS_WIN
	QFileInfo fi(bin);
	if (!fi.exists() || !fi.isExecutable() || fi.isDir()) {
		qDebug("Player::exec: command: '%s' is not a valid executable", bin.toUtf8().constData());
		bin = "/usr/bin/" + player_bin;

		fi.setFile(bin);
		if (!fi.exists() || !fi.isExecutable() || fi.isDir()) {
			qDebug("Player::exec: command: '%s' is not a valid executable", bin.toUtf8().constData());
			bin = player_bin;
			if (found) *found = false;
		}
	}
#endif

	return bin;
}

Players::Players() {
	list.push_back( Player("SMPlayer", "smplayer", true) );
	list.push_back( Player("MPlayer", "mplayer", false, "-title ") );
	list.push_back( Player("VLC", "vlc", false, "--meta-title=") );
	list.push_back( Player("Dragon Player", "dragon", false) );
	list.push_back( Player("Totem", "totem", false) );
	list.push_back( Player("GNOME-MPlayer", "gnome-mplayer", false) );

	curr = 0;
}

QStringList Players::availablePlayers() {
	QStringList l;
	bool found;
	for (int n = 0; n < list.count(); n++) {
		QString exec = list[n].executable(&found);
		if (found) {
			l << list[n].name();
		}
	}
	return l;
}

int Players::findName(QString name) {
	for (int n = 0; n < list.count(); n++) {
		if (list[n].name() == name) return n;
	}
	return 0;
}
