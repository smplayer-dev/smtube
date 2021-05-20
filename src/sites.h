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

#ifndef SITES_H
#define SITES_H

#include <QString>
#include <QStringList>
#include <QList>

class QSettings;

class Site
{
public:
	Site(const QString & name, const QString & home, const QString & search, bool is_visible = true)
	{
		site_name = name;
		home_url = home;
		search_url = search;
		visible = is_visible;
	}

	void setName(const QString & name) { site_name = name; }
	void setHomeUrl(const QString & url) { home_url = url; }
	void setSearchUrl(const QString & url) { search_url = url; }
	void setVisible(bool b) { visible = b; }

	QString name() { return site_name; }
	QString homeUrl() { return home_url; }
	QString searchUrl() { return search_url; }
	bool isVisible() { return visible; }

protected:
	QString site_name, home_url, search_url;
	bool visible;
};


class Sites
{
public:
	Sites();

	int count() { return list.count(); }

	void setSelectedSite(const QString & name);
	QString selectedSite();

	Site item(int i) { return list[i]; }
	Site currentSite();

	int findName(QString name);

	void save(QSettings * set);
	void load(QSettings * set);

	void setAllSites(QList<Site> sites) { list = sites; }
	QList<Site> allSites() { return list; }

	QList<Site> defaultSites() { return default_list; }

protected:
	QList<Site> list;
	QList<Site> default_list;

	QString selected_site;
};

#endif
