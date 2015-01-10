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

#include "proxy.h"
#include <QSettings>
#include <QNetworkProxy>

Proxy::Proxy() {
	use_proxy = false;
	proxy_type = QNetworkProxy::HttpProxy;
	proxy_host = "";
	proxy_port = 0;
	proxy_username = "";
	proxy_password = "";
}

Proxy::~Proxy() {
}

void Proxy::save(QSettings * set) {
	set->beginGroup("proxy");
	set->setValue("use_proxy", use_proxy);
	set->setValue("type", proxy_type);
	set->setValue("host", proxy_host);
	set->setValue("port", proxy_port);
	set->setValue("username", proxy_username);
	set->setValue("password", proxy_password);
	set->endGroup();
}

void Proxy::load(QSettings * set) {
	set->beginGroup("proxy");
	use_proxy = set->value("use_proxy", use_proxy).toBool();
	proxy_type = set->value("type", proxy_type).toInt();
	proxy_host = set->value("host", proxy_host).toString();
	proxy_port = set->value("port", proxy_port).toInt();
	proxy_username = set->value("username", proxy_username).toString();
	proxy_password = set->value("password", proxy_password).toString();
	set->endGroup();
}

void Proxy::applyProxy() {
	qDebug("Proxy::applyProxy");

	QNetworkProxy proxy;

	if ( (use_proxy) && (!proxy_host.isEmpty()) ) {
		proxy.setType((QNetworkProxy::ProxyType) proxy_type);
		proxy.setHostName(proxy_host);
		proxy.setPort(proxy_port);
		if ( (!proxy_username.isEmpty()) && (!proxy_password.isEmpty()) ) {
			proxy.setUser(proxy_username);
			proxy.setPassword(proxy_password);
		}
		qDebug("Proxy::applyProxy: using proxy: host: %s, port: %d, type: %d",
               proxy_host.toUtf8().constData(), proxy_port, proxy_type);
	} else {
		// No proxy
		proxy.setType(QNetworkProxy::NoProxy);
		qDebug("Proxy::applyProxy: no proxy");
	}

	QNetworkProxy::setApplicationProxy(proxy);
}
