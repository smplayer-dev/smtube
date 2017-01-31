/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2017 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "mycookiejar.h"
#include <QNetworkCookie>
#include <QSettings>
#include <QDebug>

MyCookieJar::MyCookieJar(const QString & cookies_file, QObject * parent)
	: QNetworkCookieJar(parent)
{
	set = new QSettings(cookies_file, QSettings::IniFormat, this);
	load();
}

MyCookieJar::~MyCookieJar() {
	save();
}

void MyCookieJar::save() {
	qDebug() << "MyCookieJar::save";

	set->beginGroup("cookies");

	QList<QNetworkCookie> cookies = allCookies();
	int saved_cookies = 0;
	for (int n = 0; n < cookies.count(); n++) {
		if (!cookies[n].isSessionCookie()) {
			qDebug() << "MyCookieJar::save:" << cookies[n].name();
			set->setValue(QString("cookie_%1").arg(n), cookies[n].toRawForm());
			saved_cookies++;
		}
	}

	set->setValue("count", saved_cookies);
	set->endGroup();
}

void MyCookieJar::load() {
	qDebug() << "MyCookieJar::load";

	QList<QNetworkCookie> cookies;

	set->beginGroup("cookies");
	int count = set->value("count", 0).toInt();

	for (int n = 0; n < count; n++) {
		QByteArray cookie = set->value(QString("cookie_%1").arg(n)).toByteArray();
		cookies << QNetworkCookie::parseCookies(cookie);
		//qDebug() << "MyCookieJar::load:" << cookies.count();
	}

	set->endGroup();

	qDebug() << "MyCookieJar::load:" << cookies.count() << "cookies loaded";

	setAllCookies(cookies);
}

#include "moc_mycookiejar.cpp"
