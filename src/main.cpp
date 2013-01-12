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

#include "QtSingleApplication"
#include <QTranslator>
#include <QLibraryInfo>
#include <QSettings>
#include <QDir>
#include "ytdialog.h"

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

QString translationsPath() {
	QString path = "translations";
#if !defined(Q_OS_WIN)
#ifdef TRANSLATION_PATH
	 QString s = QString(TRANSLATION_PATH);
	 if (!s.isEmpty()) path = s;
#endif
#endif
    //qDebug("Translations path: '%s'", path.toUtf8().constData());
    return path;
}

QString qtTranslationsPath() {
#if defined(Q_OS_WIN)
    return "translations";
#else
    return QLibraryInfo::location(QLibraryInfo::TranslationsPath);
#endif
}

int main( int argc, char ** argv ) 
{
	QtSingleApplication a("smtube", argc, argv);
	/* a.setWheelScrollLines(1); */

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
		search_term = args[n];
	}

	QString message;
	if (!search_term.isEmpty()) message = "search " + search_term;
	if (a.isRunning()) { 
		a.sendMessage(message);
		qDebug("Another instance is running. Exiting.");
		return 0;
	}
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );

	QString locale = QLocale::system().name();
	if (!language.isEmpty()) locale = language;
	QTranslator app_trans;
	app_trans.load("smtube_" + locale, translationsPath());

	QTranslator qt_trans;
	qt_trans.load("qt_" + locale, qtTranslationsPath());

	a.installTranslator(&app_trans);
	a.installTranslator(&qt_trans);

	if (!QFile::exists(configPath())) {
		qDebug("Creating '%s'", configPath().toUtf8().constData() );
		QDir().mkpath( configPath() );
	}

	QSettings settings(configPath() + "/smtube.ini", QSettings::IniFormat);

	YTDialog * yt = new YTDialog(0, &settings);
	QObject::connect(&a, SIGNAL(messageReceived(const QString&)),
                     yt, SLOT(handleMessage(const QString&)));

	a.setActivationWindow(yt);

	if (!search_term.isEmpty()) {
		yt->setSearchTerm(search_term);
	} else {
		yt->setMode(YTDialog::Button);
	}
	yt->show();

	int r = a.exec();

	delete yt;

	return r;
}
