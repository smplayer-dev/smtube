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

#include "browserwindow.h"
#include "version.h"
#include <QApplication>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#ifdef HDPI_SUPPORT
#include "hdpisupport.h"
#endif

#ifdef PORTABLE_APP
#ifdef Q_OS_WIN
#include <windows.h>
QString applicationPath() {
	wchar_t my_path[_MAX_PATH+1];
	GetModuleFileName(NULL, my_path,_MAX_PATH);
	QString app_path = QString::fromWCharArray(my_path);
	if (app_path.isEmpty()) return "";
	QFileInfo fi(app_path);
	return fi.absolutePath();
}
#else
QString applicationPath() {
	QString exe_file = QFile::symLinkTarget(QString("/proc/%1/exe").arg(QCoreApplication::applicationPid()));
	return QFileInfo(exe_file).absolutePath();
}
#endif
#endif // PORTABLE_APP


QString configPath() {
#ifdef PORTABLE_APP
	return qApp->applicationDirPath();
#else
#if !defined(Q_OS_WIN) && !defined(Q_OS_OS2)
	const char * XDG_CONFIG_HOME = getenv("XDG_CONFIG_HOME");
	if (XDG_CONFIG_HOME!=NULL) {
		/* qDebug("configPath: XDG_CONFIG_HOME: %s", XDG_CONFIG_HOME); */
		return QString(XDG_CONFIG_HOME) + "/smtube";
	}
	else
	return QDir::homePath() + "/.config/smtube";
#else
	return QDir::homePath() + "/.smtube";
#endif
#endif // PORTABLE_APP
}

#ifdef HDPI_SUPPORT
QString smplayerConfigPath() {
#ifdef PORTABLE_APP
	return applicationPath();
#else
#if !defined(Q_OS_WIN) && !defined(Q_OS_OS2)
	const char * XDG_CONFIG_HOME = getenv("XDG_CONFIG_HOME");
	if (XDG_CONFIG_HOME!=NULL) {
		/* qDebug("configPath: XDG_CONFIG_HOME: %s", XDG_CONFIG_HOME); */
		return QString(XDG_CONFIG_HOME) + "/smplayer";
	}
	else
	return QDir::homePath() + "/.config/smplayer";
#else
	return QDir::homePath() + "/.smplayer";
#endif
#endif // PORTABLE_APP
}
#endif // HDPI_SUPPORT


QString translationsPath() {
#ifdef Q_WS_AMIGA // zzd10h
	QDir::setCurrent(qApp->applicationDirPath());
#endif

	QString path = "translations";
#ifdef Q_OS_MACX
	path = qApp->applicationDirPath() + "/../Resources/translations";
#else
#ifdef Q_OS_WIN
	if (!QDir().exists(path)) {
		path = qApp->applicationDirPath() + "/translations";
	}
#else
	#ifdef TRANSLATION_PATH
	QString s = QString(TRANSLATION_PATH);
	if (!s.isEmpty()) path = s;
	#endif
#endif
#endif
	qDebug() << "Translations path:" << path;
	return path;
}

QString qtTranslationsPath() {
#if defined(Q_OS_WIN)
	return "translations";
#else
	return QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
}

int main(int argc, char * argv[]) {
#ifdef HDPI_SUPPORT
	QString hdpi_config_path = smplayerConfigPath();
	HDPISupport * hdpi = 0;
	if (!hdpi_config_path.isEmpty()) {
		hdpi = new HDPISupport(hdpi_config_path);
	}
#endif

	QApplication a(argc, argv);
	a.setApplicationName("smtube");

#if QT_VERSION >= 0x040400
	a.setApplicationVersion(smtubeVersion());

	// Enable icons in menus
	a.setAttribute(Qt::AA_DontShowIconsInMenus, false);
#endif

	QUrl url;
	QString search_term;
	QString language;

	QStringList args = qApp->arguments();
	for (int n = 1; n < args.count(); n++) {
		QString argument = args[n];
		if (argument == "-lang") {
			if (n+1 < args.count()) {
				n++;
				language = args[n];
			}
		}
		else
		if (argument == "-url") {
			if (n+1 < args.count()) {
				n++;
				url = QUrl::fromUserInput(argv[n]);
			}
		}
		else
		search_term = args[n];
	}

	QString locale = QLocale::system().name();
	if (!language.isEmpty()) locale = language;
	QTranslator app_trans;
	app_trans.load("smtube_" + locale, translationsPath());

	QTranslator qt_trans;
	qt_trans.load("qt_" + locale, qtTranslationsPath());

	a.installTranslator(&app_trans);
	a.installTranslator(&qt_trans);
	
	#if QT_VERSION >= 0x050000
	QTranslator qtbase_trans;
	qtbase_trans.load("qtbase_" + locale, qtTranslationsPath());
	a.installTranslator(&qtbase_trans);
	#endif

	if (!QFile::exists(configPath())) {
		qDebug() << "Creating" << configPath();
		QDir().mkpath( configPath() );
	}

	BrowserWindow * w = new BrowserWindow(configPath());

	if (!search_term.isEmpty()) {
		w->search(search_term);
	}
	else
	if (url.isValid()) {
		w->loadUrl(url);
	}
	else {
		w->loadHomePage();
	}

	w->show();
	int r = a.exec();

	delete w;

#ifdef HDPI_SUPPORT
	if (hdpi != 0) delete hdpi;
#endif

	return r;
}
