/*  smtube, a small youtube browser.
    Copyright (C) 2012-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "regions.h"
#include <QObject>
#include <QLocale>

QMap<QString,QString> Regions::list() {
	QMap<QString,QString> l;

	l["AR"] = tr("Argentina");
	l["AU"] = tr("Australia");
	// l["AT"] = tr("Austria"); // Invalid code :?
	l["BE"] = tr("Belgium");
	l["BR"] = tr("Brazil");
	l["CA"] = tr("Canada");
	l["CL"] = tr("Chile");
	l["CO"] = tr("Colombia");
	l["CZ"] = tr("Czech Republic");
	l["EG"] = tr("Egypt");
	l["FR"] = tr("France");
	l["DE"] = tr("Germany");
	l["GB"] = tr("Great Britain");
	l["HK"] = tr("Hong Kong");
	l["HU"] = tr("Hungary");
	l["IN"] = tr("India");
	l["IE"] = tr("Ireland");
	l["IL"] = tr("Israel");
	l["IT"] = tr("Italy");
	l["JP"] = tr("Japan");
	l["JO"] = tr("Jordan");
	l["MY"] = tr("Malaysia");
	l["MX"] = tr("Mexico");
	l["MA"] = tr("Morocco");
	l["NL"] = tr("Netherlands");
	l["NZ"] = tr("New Zealand");
	l["PE"] = tr("Peru");
	l["PH"] = tr("Philippines");
	l["PL"] = tr("Poland");
	l["RU"] = tr("Russia");
	l["SA"] = tr("Saudi Arabia");
	l["SG"] = tr("Singapore");
	l["ZA"] = tr("South Africa");
	l["KR"] = tr("South Korea");
	l["ES"] = tr("Spain");
	l["SE"] = tr("Sweden");
	//l["CH"] = tr("Switzerland"); // Invalid code :?
	l["TW"] = tr("Taiwan");
	l["AE"] = tr("United Arab Emirates");
	l["US"] = tr("United States");

	return l;
}

QString Regions::findRegionForLocale(const QLocale & locale) {
	QMap <QString,QString> l = list();
	QString r = "US"; // default

	QString locale_name = locale.name();
	qDebug("Regions::findRegionForLocale: locale_name: %s", locale_name.toUtf8().constData());

	int pos = locale_name.indexOf("_");
	if (pos > -1) {
		QString country_code = locale_name.mid(pos+1);
		qDebug("Regions::findRegionForLocale: country: %s", country_code.toUtf8().constData());
		if (l.contains(country_code)) return country_code;
	}

	return r;
}

#include "moc_regions.cpp"
