/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2020 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "sitedialog.h"

SiteDialog::SiteDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);
}

SiteDialog::~SiteDialog() {
}

void SiteDialog::setSite(Site s) {
	name_edit->setText(s.name());
	home_edit->setText(s.homeUrl());
	search_edit->setText(s.searchUrl());
}

Site SiteDialog::site() {
	return Site(name_edit->text(), home_edit->text(), search_edit->text());
}

#include "moc_sitedialog.cpp"
