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

#include "sites.h"
#include <QSettings>
#include <QDebug>

Sites::Sites() {
	list.clear();
	list << Site("Tonvid.com", "http://www.tonvid.com", "http://www.tonvid.com/search.php?q=SEARCHTERM");
	list << Site("Youtube.com", "http://m.youtube.com", "http://m.youtube.com/results?search_query=SEARCHTERM");

	QStringList sites_inv;
	sites_inv << "yewtu.be" << "invidious.snopyta.org";
	foreach (QString site, sites_inv) {
		list << Site(site, "https://" + site, "https://" + site + "/search?q==SEARCHTERM");
	}

	default_list = list;
}

void Sites::setSelectedSite(const QString & name) {
	int index = findName(name);
	if (index == -1) index = 0;
	selected_site = list[index].name();
}

QString Sites::selectedSite() {
	int index = findName(selected_site);
	if (index == -1) index = 0;
	return list[index].name();
}

Site Sites::currentSite() {
	int index = findName(selected_site);
	if (index == -1) index = 0;
	return list[index];
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
	set->setValue("selected_site_name", selected_site);

	for (int n = 0; n < list.count(); n++) {
		QString section = QString("site_%1").arg(n);
		set->beginGroup(section);
		set->setValue("name", list[n].name());
		set->setValue("home_url", list[n].homeUrl());
		set->setValue("search_url", list[n].searchUrl());
		set->setValue("visible", list[n].isVisible());
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
			bool visible = set->value("visible", true).toBool();

			list << Site(name, home_url, search_url, visible);
			set->endGroup();
		}

		selected_site = set->value("selected_site_name", "").toString();
	}

	set->endGroup();

	// Merge with default sites
	foreach(Site site, defaultSites()) {
		if (findName(site.name()) == -1) {
			list << site;
		}
	}
}
