/*  smtube, a small youtube browser.
    Copyright (C) 2012-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "sites.h"
#include <QSettings>
#include <QDebug>

Sites::Sites() {
	list.clear();
	list << Site("Tonvid.com", "http://www.tonvid.com", "http://www.tonvid.com/search.php?q=SEARCHTERM");
	list << Site("Youtube.com", "http://m.youtube.com", "http://m.youtube.com/results?search_query=SEARCHTERM");

	default_list = list;
	curr = 0;
}

int Sites::findName(QString name) {
	for (int n = 0; n < list.count(); n++) {
		if (list[n].name() == name) return n;
	}
	return -1;
}

void Sites::save(QSettings * set) {
	qDebug() << "Sites::save";

	set->beginGroup("sites");
	set->setValue("count", list.count());
	set->setValue("selected_site", curr);

	for (int n = 0; n < list.count(); n++) {
		QString section = QString("site_%1").arg(n);
		set->beginGroup(section);
		set->setValue("name", list[n].name());
		set->setValue("home_url", list[n].homeUrl());
		set->setValue("search_url", list[n].searchUrl());
		set->endGroup();
	}

	set->endGroup();
}

void Sites::load(QSettings * set) {
	qDebug() << "Sites::load";

	set->beginGroup("sites");
	int count = set->value("count", 0).toInt();

	if (count > 0) {
		list.clear();

		for (int n = 0; n < count; n++) {
			QString section = QString("site_%1").arg(n);
			set->beginGroup(section);
			QString name = set->value("name", "").toString();
			QString home_url = set->value("home_url", "").toString();
			QString search_url = set->value("search_url", "").toString();

			list << Site(name, home_url, search_url);
			set->endGroup();
		}

		curr = set->value("selected_site", 0).toInt();
		if (curr >= list.count()) curr = 0;
	}

	set->endGroup();
}
