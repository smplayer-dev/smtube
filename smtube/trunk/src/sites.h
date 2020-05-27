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

#ifndef SITES_H
#define SITES_H

#include <QString>
#include <QStringList>
#include <QList>

class QSettings;

class Site
{
public:
	Site(const QString & name, const QString & home, const QString & search)
	{
		site_name = name;
		home_url = home;
		search_url = search;
	}

	void setName(const QString & name) { site_name = name; }
	void setHomeUrl(const QString & url) { home_url = url; }
	void setSearchUrl(const QString & url) { search_url = url; }

	QString name() { return site_name; }
	QString homeUrl() { return home_url; }
	QString searchUrl() { return search_url; }

protected:
	QString site_name, home_url, search_url;
};


class Sites
{
public:
	Sites();

	int count() { return list.count(); }

	void setCurrent( int c ) { curr = c; }
	int current() { return curr; }

	Site item(int i) { return list[i]; }
	Site currentSite() { return list[curr]; }

	QList<Site> availableSites();
	int findName(QString name);

	void save(QSettings * set);
	void load(QSettings * set);

	void setAllSites(QList<Player> sites);
	QList<Site> allSites() { return list; }

	QList<Site> defaultSites() { return default_list; }

protected:
	QList<Site> list;
	QList<Site> default_list;

	int curr;
};

#endif
