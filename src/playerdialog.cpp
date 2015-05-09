/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "playerdialog.h"

PlayerDialog::PlayerDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);
}

PlayerDialog::~PlayerDialog() {
}

void PlayerDialog::setName(const QString & name) {
	name_edit->setText(name);
}

void PlayerDialog::setBinary(const QString & binary) {
	binary_edit->setText(binary);
}

void PlayerDialog::setParameters(const QString & parms) {
	parms_edit->setText(parms);
}

void PlayerDialog::setDirectPlay(bool b) {
	directplay_check->setChecked(b);
}

QString PlayerDialog::name() {
	return name_edit->text();
}

QString PlayerDialog::binary() {
	return binary_edit->text();
}

QString PlayerDialog::parameters() {
	return parms_edit->text();
}

bool PlayerDialog::directPlay() {
	return directplay_check->isChecked();
}

#include "moc_playerdialog.cpp"


