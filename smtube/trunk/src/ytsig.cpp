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

#include "ytsig.h"

#ifdef YT_USE_SCRIPT
#include <QtScript>
#endif

#ifdef YT_USE_SCRIPT

QString YTSig::parsed_ts;

QString YTSig::aclara(const QString & text, const QString & player, const QString & function_name) {
	int dot = text.indexOf('.');
	qDebug("YTSig::aclara: length: %d (%d.%d) p: %d", text.size(), dot, text.size()-dot-1, !player.isEmpty());

	if (script.isEmpty()) script = default_script;

	QScriptEngine engine;

	//QScriptSyntaxCheckResult r = engine.checkSyntax(script);
	//qDebug() << (int) r.state();

	engine.evaluate(script);

	QScriptValueList args;
	QString fname;

	if (!function_name.isEmpty()) {
		fname = function_name;
		args << text;
	}
	else
	if (player.isEmpty()) {
		fname = "aclara";
		args << text;
	}
	else {
		fname = "aclara_p";
		args << text << player;
	}

	//qDebug("YTSig::aclara: function_name: %s", function_name.toLatin1().constData());

	QScriptValue aclarar = engine.globalObject().property(fname);
	QString res = aclarar.call(QScriptValue(), args).toString();

	//qDebug() << res;

	if (res.isEmpty()) {
		qDebug("YTSig::aclara: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
	}

	return res;
}

void YTSig::reloadScriptFile() {
	qDebug("YTSig::reloadScriptFile: %s", script_file.toUtf8().constData());

	if (!QFile::exists(script_file)) {
		qDebug("YTSig::reloadScriptFile: file doesn't exist.");
		return;
	}

	QFile f(script_file);
	f.open(QIODevice::ReadOnly);
	QByteArray bytes = f.readAll();
	f.close();

	parsed_ts = "";

	if (!bytes.isEmpty()) {
		script = bytes;

		QRegExp rx("TS: ([\\d,a-z,A-Z-]+)");
		if (rx.indexIn(bytes)) {
			parsed_ts = rx.cap(1);
			qDebug("YTSig::reloadScriptFile: parsed_ts: %s", parsed_ts.toLatin1().constData());
		}

	}
}

QString YTSig::script;
QString YTSig::script_file;

QString YTSig::default_script;

#else

QString YTSig::parsed_ts = "0";

QString YTSig::rev(const QString & orig) {
	QString r;
	for (int n = orig.size()-1; n >= 0; n--) {
		r.append(orig.at(n));
	}
	return r;
}

QString YTSig::aclara(const QString & text, const QString & player, const QString & function_name) {
	QString res;

	int dot = text.indexOf('.');
	qDebug("YTSig::aclara (2): length: %d (%d.%d)", text.size(), dot, text.size()-dot-1);

#if 0
	if (!function_name.isEmpty()) {
		res = <your code>;
		return res;
	}

	if (text.size() == xx) {
		res = <your code>;
	}
	else {
		qDebug("YTSig::aclara: signature length not supported: %d: %s", text.size(), text.toLatin1().constData());
		return res;
	}
#endif

	/*
	qDebug("%d: orig: %s", text.size(), text.toLatin1().constData());
	qDebug("%d: conv: %s", text.size(), res.toLatin1().constData());
	*/

	return res;
}

#endif

void YTSig::check(QString & u) {
	if (!parsed_ts.isEmpty()) {
		u.append(QString("%1%2%3%4%5%6").arg(u['3']).arg(u['5']).arg(u['0'-23]).arg(u['a'-44]).arg(u['I'-33]).arg(parsed_ts));
	}
}
