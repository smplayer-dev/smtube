/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef MYWEBPAGE_H
#define MYWEBPAGE_H

#include <QWebPage>

class MyWebPage : public QWebPage
{
	Q_OBJECT

public:
	MyWebPage(QObject * parent = 0) : QWebPage(parent) {};

protected:
	virtual QString userAgentForUrl(const QUrl& url) const;
	virtual void javaScriptConsoleMessage(const QString & message, int lineNumber, const QString & sourceID);
};

#endif
