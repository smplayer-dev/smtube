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

	l["AR"] = QObject::tr("Argentina");
	l["AU"] = QObject::tr("Australia");
	// l["AT"] = QObject::tr("Austria"); // Invalid code :?
	l["BE"] = QObject::tr("Belgium");
	l["BR"] = QObject::tr("Brazil");
	l["CA"] = QObject::tr("Canada");
	l["CL"] = QObject::tr("Chile");
	l["CO"] = QObject::tr("Colombia");
	l["CZ"] = QObject::tr("Czech Republic");
	l["EG"] = QObject::tr("Egypt");
	l["FR"] = QObject::tr("France");
	l["DE"] = QObject::tr("Germany");
	l["GB"] = QObject::tr("Great Britain");
	l["HK"] = QObject::tr("Hong Kong");
	l["HU"] = QObject::tr("Hungary");
	l["IN"] = QObject::tr("India");
	l["IE"] = QObject::tr("Ireland");
	l["IL"] = QObject::tr("Israel");
	l["IT"] = QObject::tr("Italy");
	l["JP"] = QObject::tr("Japan");
	l["JO"] = QObject::tr("Jordan");
	l["MY"] = QObject::tr("Malaysia");
	l["MX"] = QObject::tr("Mexico");
	l["MA"] = QObject::tr("Morocco");
	l["NL"] = QObject::tr("Netherlands");
	l["NZ"] = QObject::tr("New Zealand");
	l["PE"] = QObject::tr("Peru");
	l["PH"] = QObject::tr("Philippines");
	l["PL"] = QObject::tr("Poland");
	l["RU"] = QObject::tr("Russia");
	l["SA"] = QObject::tr("Saudi Arabia");
	l["SG"] = QObject::tr("Singapore");
	l["ZA"] = QObject::tr("South Africa");
	l["KR"] = QObject::tr("South Korea");
	l["ES"] = QObject::tr("Spain");
	l["SE"] = QObject::tr("Sweden");
	//l["CH"] = QObject::tr("Switzerland"); // Invalid code :?
	l["TW"] = QObject::tr("Taiwan");
	l["AE"] = QObject::tr("United Arab Emirates");
	l["US"] = QObject::tr("United States");

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

