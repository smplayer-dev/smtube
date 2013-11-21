/*  smtube, a small youtube browser.
    Copyright (C) 2012-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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
        "<p>&copy; 2012-2013 Ricardo Villalba &lt;rvm@users.sourceforge.net&gt;" + 
        "<br>" + tr("Based on UMPlayer") + " &copy; 2010 Ori Rejwan" 
        "<p><b>" +
        tr("Version: %1").arg(smtubeVersion()) + "</b>"
#if PORTABLE_APP
        " (" + tr("Portable Edition") + ")"
#endif
        "<br>" +
        tr("Compiled with Qt %1 (using %2)").arg(QT_VERSION_STR).arg(qVersion()) + 
        "<p>" + 
        tr("Visit our web for updates:") +"<br>"+ link("http://www.smtube.org")
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
		 tr("Many people contributed with translations.") +" "+
		 tr("You can also help to translate SMPlayer into your own language.") +"<p>"+
		 tr("Visit %1 and join a translation team.").arg("<a href=\"http://www.transifex.com/projects/p/smplayer/\">http://www.transifex.com/projects/p/smplayer/</a>") +
		"<p>" +
		 tr("Current translators from the transifex teams:") +
		"<p>" + 
		trad(tr("Spanish"), "Ricardo Villalba") +
		trad(tr("Basque"), "Xabier Aramendi") +
		trad(tr("Croatian"), "Gogo") +
		trad(tr("Czech"), QStringList() << QString::fromUtf8("Petr Šimáček") << QString::fromUtf8("Jakub Kožíšek")) +
		trad(tr("Japanese"), QStringList() << "Ever_green" << "Nardog") +
		trad(tr("Korean"), QStringList() << "ParkJS" << "Potato") +
		trad(tr("Portuguese"), QString::fromUtf8("Sérgio Marques")) +
		trad(tr("Serbian"), QStringList() << QString::fromUtf8("Mladen Pejaković") << "Miroslav" << "Rancher") +
		trad(tr("Ukrainian"), "Zubr139") +
		trad(tr("Galician"), QStringList() << QString::fromUtf8("Adrián Chaves Fernández") << "Miguel Branco") +
		trad(tr("Lithuanian"), QString::fromUtf8("Algimantas Margevičius")) +
		trad(tr("Malay"), "Abuyop") +
		trad(tr("Portuguese - Brazil"), QStringList() << QString::fromUtf8("Maico Sertório") << "Vinicius" << "Ronnie Dilli" << QString::fromUtf8("Lucas Simões")) +
		trad(tr("Hebrew"), "GenghisKhan") +
		trad(tr("Simplified Chinese"), QStringList() << "OpenBDH" << "Zhangzheliuli" << "Zhnagmin" << "wwj402") +
		trad(tr("Vietnamese"), QStringList() << "Anh Phan" << "Biz Over" << "Thu Thao Nguyen Ngoc") +
		trad(tr("Polish"), QStringList() <<"Filux" << QString::fromUtf8("Łukasz Hryniuk") << QString::fromUtf8("Piotr Strębski") << QString::fromUtf8("Michał Trzebiatowski")) +
		trad(tr("Russian"), QStringList() << "WiseLord" << "Viktor" << "DmitryKX" << "Gleb Mekhrenin" << "ElFrio") +
		trad(tr("French"), QStringList() << "Olivier Devineau" << "Ybsar") +
		trad(tr("Indonesian"), QStringList() << "Mohamad Hasan Al Banna" << "Aulia Firdaus Simbolon" << "Muhammad Fikri Hariri") +
		trad(tr("Danish"), "Michael Larsen") +
		trad(tr("Hungarian"), "Gojko") +
		trad(tr("Turkish"), QStringList() << "Emre Firat" << QString::fromUtf8("Hasan Akgöz")) +
		trad(tr("Finnish"), QString::fromUtf8("Jiri Grönroos")) +
		trad(tr("German"), QStringList() << "Shaggy" << QString::fromUtf8("Michał Trzebiatowski") << "Eclipse" << "j5lx" << "Tobias Bannert") +
		trad(tr("Traditional Chinese"), QStringList() << "Taijuin Lee" << "Wpliao" ) +
		trad(tr("Bulgarian"), QStringList() << "Ivailo Monev" << QString::fromUtf8("Радослав")) +
		trad(tr("Norwegian Nynorsk"), "Bjorni") +
		trad(tr("Swedish"), QStringList() << "XC" << "Andreas Gustafsson") +
		trad(tr("Arabic"), QStringList() << "Riyadh" << "Muhammad Fawwaz Orabi") +
		trad(tr("Georgian"), "George Machitidze") +
		trad(tr("Arabic - Saudi Arabia"), "Mohamed") +
		trad(tr("Hungarian"), "Gojko") +
		trad(tr("Sinhala"), "Rathnayake") +
		trad(tr("Greek"), QString::fromUtf8("Γιάννης Ανθυμίδης")) +
		trad(tr("Estonian"), QString::fromUtf8("Olav Mägi")) +
		trad(tr("N'ko"), QStringList() << QString::fromUtf8("Kairaba Cissé") << "Youssouf Diaby" << "Lasnei Kante" << "Kante Soufiane") +
		trad(tr("Italian"), "Damtux") +
		trad(tr("Uzbek"), "Umid Almasov") +
		trad(tr("Catalan"), QStringList() << "Anna Fenoy" << "Jmontane") +
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
		if (n < (authors.count()-1)) s += ", ";
	}
	//return QString("<h3>%1:</h3><h4>%2</h4><hr>").arg(lang).arg(s);
	return QString("<p><b>%1</b>: %2</p>").arg(lang).arg(s);
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
