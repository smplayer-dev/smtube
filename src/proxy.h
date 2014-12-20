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

#ifndef PROXY_H
#define PROXY_H

#include <QString>

class QSettings;

class Proxy
{
public:
	Proxy();
	~Proxy();

	void applyProxy();

	void setUseProxy(bool b) { use_proxy = b; };
	bool useProxy() { return use_proxy; };

	void setType(int type) { proxy_type = type; };
	int type() { return proxy_type; };

	void setHost(const QString & host) { proxy_host = host; };
	QString host() { return proxy_host; };

	void setPort(int port) { proxy_port = port; };
	int port() { return proxy_port; };

	void setUsername(const QString & username) { proxy_username = username; };
	QString username() { return proxy_username; };

	void setPassword(const QString & pass) { proxy_password = pass; };
	QString password() { return proxy_password; };

	void save(QSettings * set);
	void load(QSettings * set);

protected:
	bool use_proxy;
	int proxy_type;
	QString proxy_host;
	int proxy_port;
	QString proxy_username;
	QString proxy_password;
};

#endif
