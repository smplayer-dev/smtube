/*  smtube, a small youtube browser.
    Copyright (C) 2012 Ricardo Villalba <rvm@users.sourceforge.net>
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

#include <QNetworkRequest>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QTextDocument>
#include "retrievevideourl.h"

RetrieveVideoUrl::RetrieveVideoUrl(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(gotVideoPage(QNetworkReply*)));
    reply = 0;
}

void RetrieveVideoUrl::fetchYTVideoPage(QString videoId, QString title)
{
    QString videoPageUrl = "http://www.youtube.com/watch?v=" + videoId;
    QNetworkRequest req(videoPageUrl);
    reply = manager->get(req);
    id = videoId;
    m_title = title;
}

void RetrieveVideoUrl::gotVideoPage(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        emit errorOcurred(m_title, (int)reply->error());
        return;
    }   
    QByteArray arr = reply->readAll();    
    if(arr.isEmpty())
    {
        fetchYTVideoPage(id, m_title);
        return;
    }
    QString replyString = QString::fromUtf8(arr.constData(), arr.size());        
    QRegExp regex("\\\"url_encoded_fmt_stream_map\\\"\\s*:\\s*\\\"([^\\\"]*)");
    regex.indexIn(replyString);
    QString fmtArray = regex.cap(1);    
    fmtArray = sanitizeForUnicodePoint(fmtArray);
    fmtArray.replace(QRegExp("\\\\(.)"), "\\1");
    htmlDecode(fmtArray);
    QStringList codeList = fmtArray.split(',');
    foreach(QString code, codeList)
    {
		// (2012-12-20) Youtube Fix by RVM for SMPlayer (http://smplayer.sourceforge.net)

		/* qDebug("RetrieveYoutubeUrl::parse: code: '%s'", code.toLatin1().constData()); */

		int itag = 0;
		QString n_url;
		QString url;
		QString s_itag;

		QStringList par_list = code.split(QRegExp("&|\\?"));
		foreach(QString par, par_list) {
			/* qDebug("RetrieveYoutubeUrl::parse: par: %s", par.toLatin1().constData()); */

			if (par.startsWith("url=")) url = par.mid(4);
			else
			if (par.startsWith("itag=")) {
				if (s_itag.isEmpty()) {
					s_itag = par;
					QRegExp rx("itag=(\\d+)");
					if (rx.indexIn(s_itag) != -1) itag = rx.cap(1).toInt();
					/* qDebug("RetrieveYoutubeUrl::parse: itag: %d", itag); */
				}
			}
			else {
				if (!n_url.isEmpty()) n_url += "&";
				n_url += par;
			}
		}
		n_url = url + "?" + s_itag + "&" + n_url;
		n_url.replace("&sig=", "&signature=");

		/* qDebug("RetrieveYoutubeUrl::parse: n_url: '%s'", n_url.toLatin1().constData()); */

		urlMap[itag] = n_url;
    }

    emit gotUrls(urlMap, m_title, id);    
}

QString RetrieveVideoUrl::sanitizeForUnicodePoint(QString string)
{
    QRegExp rx("\\\\u(\\d{4})");
    while (rx.indexIn(string) != -1) {
        string.replace(rx.cap(0), QString(QChar(rx.cap(1).toInt(0,16))));
    }
    return string;
}

void RetrieveVideoUrl::htmlDecode(QString& string)
{
    string.replace("%3A", ":", Qt::CaseInsensitive);
    string.replace("%2F", "/", Qt::CaseInsensitive);
    string.replace("%3F", "?", Qt::CaseInsensitive);
    string.replace("%3D", "=", Qt::CaseInsensitive);
    string.replace("%25", "%", Qt::CaseInsensitive);
    string.replace("%26", "&", Qt::CaseInsensitive);
    string.replace("%3D", "=", Qt::CaseInsensitive);
}

void RetrieveVideoUrl::cancel()
{
    reply->abort();
}

#include "moc_retrievevideourl.cpp"
