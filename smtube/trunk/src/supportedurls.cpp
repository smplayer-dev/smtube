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

#include "supportedurls.h"

QList<QRegExp> SupportedUrls::list;
QList<QRegExp> SupportedUrls::ds_list;
QRegExp SupportedUrls::ytrx;

int SupportedUrls::site(const QString & url) {
	if (ytrx.indexIn(url) > -1) {
		return Youtube;
	}

	for (int n = 0; n < list.count(); n++) {
		if (list[n].indexIn(url) > -1) {
			return StreamingSite;
		}
	}

	for (int n = 0; n < ds_list.count(); n++) {
		if (ds_list[n].indexIn(url) > -1) {
			return DirectStream;
		}
	}

	return SupportedUrls::Unsupported;
}

void SupportedUrls::load() {
	ytrx = QRegExp("^(https?\\:\\/\\/)?(www\\.|m\\.)?youtube\\.com\\/watch\\?v=.+$");

	list << QRegExp("^(https?\\:\\/\\/)?(www\\.)?dailymotion\\.com\\/video\\/.+$");
	list << QRegExp("^(https?\\:\\/\\/)?(www\\.)?vimeo\\.com\\/(channels\\/staffpicks\\/)?\\d+$");

	ds_list << QRegExp("^(https?\\:\\/\\/).*m3u8?\\??");
}
