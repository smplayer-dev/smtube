/*  smplayer, GUI front-end for mplayer.
    Copyright (C) 2006-2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "about.h"
#include "version.h"

About::About(QWidget * parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	setWindowIcon( QPixmap(":/icons/logo.png") );

	logo->setPixmap( QPixmap(":/icons/logo.png").scaledToHeight(64, Qt::SmoothTransformation) );
	translators_icon->setPixmap( QPixmap(":/icons/translators.png" ) );
	license_icon->setPixmap( QPixmap(":/icons/license.png" ) );

    info->setHtml(
        "<b>"+ tr("YouTube%1 Browser for SMPlayer").arg(QChar(0x2122)) +"</b>"
        "<p>&copy; 2012 Ricardo Villalba &lt;rvm@users.sourceforge.net&gt;" + 
        "<br>" + tr("Based on UMPlayer") + " &copy; 2010 Ori Rejwan" 
        "<p><b>" +
        tr("Version: %1").arg(appVersion()) + "</b>"
#if PORTABLE_APP
        " (" + tr("Portable Edition") + ")"
#endif
        "<br>" +
        tr("Compiled with Qt %1 (using %2)").arg(QT_VERSION_STR).arg(qVersion()) + 
        "<p>" + 
        tr("Visit our web for updates:") +"<br>"+ link("http://smplayer.sf.net")
    );

    license->setHtml(
        "<i>"
        "This program is free software; you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License as published by "
        "the Free Software Foundation; either version 2 of the License, or "
        "(at your option) any later version."  "</i><br><br>"
    );
    /*
	license->setOpenLinks(false);
	license->setOpenExternalLinks(false);
	connect(license, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(openLink(const QUrl&)));
    */

	translators->setHtml( getTranslators() );

	// Copy the background color ("window") of the tab widget to the "base" color of the qtextbrowsers
	// Problem, it doesn't work with some styles, so first we change the "window" color of the tab widgets.
	info_tab->setAutoFillBackground(true);
	translations_tab->setAutoFillBackground(true);
	license_tab->setAutoFillBackground(true);

	QPalette pal = info_tab->palette();
	pal.setColor(QPalette::Window, palette().color(QPalette::Window) );

	info_tab->setPalette(pal);
	translations_tab->setPalette(pal);
	license_tab->setPalette(pal);

	QPalette p = info->palette();
	//p.setBrush(QPalette::Base, info_tab->palette().window());
	p.setColor(QPalette::Base, info_tab->palette().color(QPalette::Window));

	info->setPalette(p);
	translators->setPalette(p);
	license->setPalette(p);

	adjustSize();
}

About::~About() {
}

QString About::getTranslators() {
	return QString(
		 tr("The following people have contributed with translations:") +
         "<p>" + 
         trad(tr("Spanish"), "Ricardo Villalba <rvm@users.sourceforge.net>") +
         trad(tr("Russian"), QString::fromUtf8("Белый Владимир <wiselord1983@gmail.com>")) +
         trad(tr("Lithuanian"), QString::fromUtf8("Algimantas Margevičius <margevicius.algimantas@gmail.com>")) +
         trad(tr("Japanese"), "Nardog <alphisation@gmail.com>") +
         trad(tr("Simplified-Chinese"), "OpenBDH <opensource@bendihua.org>") +
         trad(tr("Basque"), "Xabier Aramendi <azpidatziak@gmail.com>") +
         trad(tr("Portuguese"), QString::fromUtf8("Sérgio Marques <smarquespt@gmail.com>")) +
         trad(tr("Hungarian"), "Gojko <m.gojko.71@gmail.com>") +
         trad(tr("Polish"), "Adam <ad56@poczta.fm>") +
         "");
}

QString About::trad(const QString & lang, const QString & author) {
	return trad(lang, QStringList() << author);
}

QString About::trad(const QString & lang, const QStringList & authors) {
	QString s;
	for (int n = 0; n < authors.count(); n++) {
		QString author = authors[n];
		s += author.replace("<", "&lt;").replace(">", "&gt;");
		if (n < (authors.count()-1)) s += "<br>";
	}
	return QString("<h3>%1:</h3><h4>%2</h4><hr>").arg(lang).arg(s);
}

QString About::link(const QString & url, QString name) {
	if (name.isEmpty()) name = url;
	return QString("<a href=\"" + url + "\">" + name +"</a>");
}

QString About::contr(const QString & author, const QString & thing) {
	return "<li>"+ tr("<b>%1</b> (%2)").arg(author).arg(thing) +"</li>";
}

QSize About::sizeHint () const {
	return QSize(518, 326);
}

/*
void About::openLink(const QUrl & link) {
	qDebug("About::openLink: '%s'", link.toString().toUtf8().constData());
	QDesktopServices::openUrl(link);
}
*/

#include "moc_about.cpp"
