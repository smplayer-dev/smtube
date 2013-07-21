/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef _RETRIEVEYOUTUBEURL_
#define _RETRIEVEYOUTUBEURL_

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>

class RetrieveYoutubeUrl : public QObject
{
	Q_OBJECT

public:
	enum Quality { FLV_240p = 5, MP4_360p = 18, MP4_720p = 22, FLV_360p = 34,
                   FLV_480p = 35, MP4_1080p = 37, WEBM_360p = 43, 
                   WEBM_480p = 44, WEBM_720p = 45, WEBM_1080p = 46 };

	RetrieveYoutubeUrl( QObject* parent = 0 );
	~RetrieveYoutubeUrl();

	void fetchPage(const QString & url);
	void close();

	void setUserAgent(const QString & s) { user_agent = s; };
	QString userAgent() { return user_agent; };

	void setPreferredQuality(Quality q) { preferred_quality = q; }
	Quality preferredQuality() { return preferred_quality; }

	static QString findPreferredUrl(const QMap<int, QString>& urlMap, Quality q);
	QString findPreferredUrl();

	QString urlTitle() { return url_title; }
	QString latestPreferredUrl() { return latest_preferred_url; }
	QString origUrl() { return orig_url; }

#ifdef YT_USE_SCRIPT
	static void setScriptFile(const QString & file);
#endif

signals:
	void gotUrls(const QMap<int, QString>&);
	void gotPreferredUrl(const QString &);
	void gotEmptyList();

	void connecting(QString host);
	void errorOcurred(int error_number, QString error_str);

	void signatureNotFound(const QString & title);

protected slots:
	void gotResponse(QNetworkReply* reply);
	void parse(QByteArray text);

protected:
	static QString sanitizeForUnicodePoint(QString string);
	static void htmlDecode(QString& string);

	QMap<int, QString> urlMap;
	QString url_title;
	QString orig_url;
	QString latest_preferred_url;

	Quality preferred_quality;
	QString user_agent;

private:
	QNetworkAccessManager* manager;
	QNetworkReply* reply;
};

#endif
