/*  smtube, a small youtube browser.
    Copyright (C) 2012-2015 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2014 zzd10h

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

#ifndef ERRORAMIGA_H
#define ERRORAMIGA_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ErrorAmiga : public QWidget
{
	Q_OBJECT

public:
	ErrorAmiga(QWidget * parent = 0, Qt::WindowFlags f = 0);
	void setText(const QString & text);
	static void showError(const QString & title, const QString & error);

private:
	QLabel *labelTitle;
	static ErrorAmiga * window;
};

#endif // ERRORAMIGA_H
