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

#include "players.h"
#include <QApplication>
#include <QFile>

QString Player::executable() {
	QString bin = qApp->applicationDirPath() + "/" + player_bin;
	#ifdef Q_OS_WIN
	bin += ".exe";
	#endif

	if (!QFile::exists(bin)) {
		qDebug("Player::exec: command: '%s' doesn't exist", bin.toUtf8().constData());
		bin = player_bin;
	}

	return bin;
}

Players::Players() {
	list.push_back( Player("SMPlayer", "smplayer", true) );
	list.push_back( Player("MPlayer", "mplayer", false, "-title") );

	curr = 0;
}

QStringList Players::names() {
	QStringList l;
	for (int n = 0; n < list.count(); n++) {
		l << list[n].name();
	}
	return l;
}

