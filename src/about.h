/*  smtube, a small youtube browser.
    Copyright (C) 2012-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#ifndef _ABOUT_H_
#define _ABOUT_H_

#include "ui_about.h"

#include <QDialog>

class QUrl;

//! Shows the about smplayer dialog

/*!
 Displays copyright info, license, translators...
*/

class About : public QDialog, public Ui::About
{
	Q_OBJECT

public:
	About( QWidget * parent = 0, Qt::WindowFlags f = QFlag(0) );
	~About();

	virtual QSize sizeHint () const;

protected:
	QString getTranslators();

	QString link(const QString & url, QString name = "");
#if 0
	//! Return a formatted string with the translator and language
	QString trad(const QString & lang, const QString & author);

	QString trad(const QString & lang, const QStringList & authors);

	//! Return a formatted string with the contributor and what he did
	QString contr(const QString & author, const QString & thing);
#endif

protected slots:
	/* void openLink(const QUrl & link); */
};

#endif
