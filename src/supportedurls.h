/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef SUPPORTEDURLS_H
#define SUPPORTEDURLS_H

#include <QList>
#include <QRegExp>

class SupportedUrls {
public:
	enum SiteID { Youtube = 1, StreamingSite = 2, DirectStream = 3, Unsupported = 0 };

	static int site(const QString & url);
	static void load();

private:
	static QList<QRegExp> list;
	static QList<QRegExp> ds_list;
	static QRegExp ytrx;
};

#endif
