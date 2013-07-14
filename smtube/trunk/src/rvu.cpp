/*  smtube, a small youtube browser.
    Copyright (C) 2012-2013 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2010 Ori Rejwan

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

#include "rvu.h"

RetrieveVideoUrl::RetrieveVideoUrl(QObject *parent) :
    RetrieveYoutubeUrl(parent)
{
	connect(this, SIGNAL(gotUrls(const QMap<int, QString>&)),
            this, SLOT(receiveUrls(const QMap<int, QString>&)));
	connect(this, SIGNAL(gotPreferredUrl(const QString &)),
            this, SLOT(receivePreferredUrl(const QString &)));
	connect(this, SIGNAL(errorOcurred(int,QString)),
            this, SLOT(receiveErrorOcurred(int,QString)));
}

void RetrieveVideoUrl::fetchYTVideoPage(QString videoId, QString title) {
	QString videoPageUrl = "http://www.youtube.com/watch?v=" + videoId;
	id = videoId;
	m_title = title;

	fetchPage(videoPageUrl);
}

void RetrieveVideoUrl::receiveUrls(const QMap<int, QString>& map) {
	qDebug("RetrieveVideoUrl::receiveUrls");
	emit gotUrls(map, m_title, id);
}

void RetrieveVideoUrl::receivePreferredUrl(const QString & url) {
	qDebug("RetrieveVideoUrl::receivePreferredUrl");
	emit gotPreferredUrl(url, m_title, id);
}

void RetrieveVideoUrl::receiveErrorOcurred(int error, QString) {
	qDebug("RetrieveVideoUrl::receiveErrorOcurred: %d", error);
	emit errorOcurred(m_title, error);
}

void RetrieveVideoUrl::cancel() {
	qDebug("RetrieveVideoUrl::cancel");
	close();
}

#include "moc_rvu.cpp"
