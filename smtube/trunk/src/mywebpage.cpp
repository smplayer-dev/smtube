/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "mywebpage.h"
#include <QDebug>

QString MyWebPage::userAgentForUrl(const QUrl& url) const {
#if 0
	if (url.host().contains("youtube.com")) {
		return "Mozilla/5.0 (Linux; Android 4.0.4; Galaxy Nexus Build/IMM76B) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.133 Mobile Safari/535.19";
		//return "Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5";
	} else {
		return QWebPage::userAgentForUrl(url);
	}
#else
	return QWebPage::userAgentForUrl(url);
#endif
}

void MyWebPage::javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID) {
	qDebug() << "source:" << sourceID << "line:" << lineNumber << ":" << message;
}

#include "moc_mywebpage.cpp"
