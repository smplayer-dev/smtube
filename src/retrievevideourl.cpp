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
    if (reply->error() != QNetworkReply::NoError)
    {
        emit errorOcurred(m_title, (int)reply->error());
        return;
    }
    QByteArray text = reply->readAll();
    if(text.isEmpty())
    {
        fetchYTVideoPage(id, m_title);
        return;
    }

	urlMap.clear();

	QRegExp regex("\\\"url_encoded_fmt_stream_map\\\"\\s*:\\s*\\\"([^\\\"]*)");
	regex.indexIn(text);
	QString fmtArray = regex.cap(1);
	fmtArray = sanitizeForUnicodePoint(fmtArray);
	fmtArray.replace(QRegExp("\\\\(.)"), "\\1");

	QList<QByteArray> codeList = fmtArray.toLatin1().split(',');
	foreach(QByteArray code, codeList) {
		code = QUrl::fromPercentEncoding(code).toLatin1();
		//qDebug("code: %s", code.constData());

		QUrl line;
		line.setEncodedQuery(code);

		if (line.hasQueryItem("url")) {
			QUrl url( line.queryItemValue("url") );
			line.setScheme(url.scheme());
			line.setHost(url.host());
			line.setPath(url.path());
			line.setEncodedQuery( line.encodedQuery() + "&" + url.encodedQuery() );
			line.removeQueryItem("url");

			if (line.hasQueryItem("sig")) {
				line.addQueryItem("signature", line.queryItemValue("sig"));
				line.removeQueryItem("sig");
			}
			else
			if (line.hasQueryItem("s")) {
				QByteArray signature = aclara(line.queryItemValue("s").toLatin1());
				if (!signature.isEmpty()) {
					line.addQueryItem("signature", signature);
				}
				line.removeQueryItem("s");
			}
			line.removeAllQueryItems("fallback_host");
			line.removeAllQueryItems("type");
			if (line.hasQueryItem("itag")) {
				QString itag = line.queryItemValue("itag");
				line.removeAllQueryItems("itag"); // Remove duplicated itag
				line.addQueryItem("itag", itag);
				urlMap[itag.toInt()] = line.toString();
				//qDebug("line: %s", line.toString().toLatin1().constData());
			}
		}
	}

	emit gotUrls(urlMap, m_title, id);    
}

QString RetrieveVideoUrl::sanitizeForUnicodePoint(QString string) {
	QRegExp rx("\\\\u(\\d{4})");
	while (rx.indexIn(string) != -1) {
		string.replace(rx.cap(0), QString(QChar(rx.cap(1).toInt(0,16))));
	}
	return string;
}

void RetrieveVideoUrl::htmlDecode(QString& string) {
	string.replace("%3A", ":", Qt::CaseInsensitive);
	string.replace("%2F", "/", Qt::CaseInsensitive);
	string.replace("%3F", "?", Qt::CaseInsensitive);
	string.replace("%3D", "=", Qt::CaseInsensitive);
	string.replace("%25", "%", Qt::CaseInsensitive);
	string.replace("%26", "&", Qt::CaseInsensitive);
	string.replace("%3D", "=", Qt::CaseInsensitive);
}

void RetrieveVideoUrl::cancel() {
	reply->abort();
}

QByteArray RetrieveVideoUrl::aclara(QByteArray text) {
	QByteArray res;

	if (text.size() != 87) return res;

	QByteArray r1, r2;

	QByteArray s = text.mid(44,40);
	for (int n = s.size(); n > 0; n--) {
		r1.append(s.at(n-1));
	}

	s = text.mid(3,40);
	for (int n = s.size(); n > 0; n--) {
		r2.append(s.at(n-1));
	}

	res = r1.mid(21,1) + r1.mid(1,20) + r1.mid(0,1) + r1.mid(22,9) + text.mid(0,1) + r1.mid(32,8) + text.mid(43,1) + r2;

	/*
	qDebug("r1: %s", r1.constData());
	qDebug("r2: %s", r2.constData());
	qDebug("res: %s", res.constData());
	*/

	return res;
}

#include "moc_retrievevideourl.cpp"
