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

#include "configdialog.h"
#include <QDebug>

#define COL_NAME 0
#define COL_BINARY 1
#define COL_PARMS 2

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);

	// Setup player's table
	table->setColumnCount(4);
	table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Binary") << tr("Parameters") );

	table->setAlternatingRowColors(true);
/*
#if QT_VERSION >= 0x050000
	table->horizontalHeader()->setSectionResizeMode(COL_FILE, QHeaderView::Stretch);
#else
	table->horizontalHeader()->setResizeMode(COL_FILE, QHeaderView::Stretch);
#endif
*/

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

ConfigDialog::~ConfigDialog() {
}

void ConfigDialog::setPlayers(QList<Player> list) {
	qDebug() << "ConfigDialog::setPlayers:" << list.count();

	table->setRowCount(list.count());
	for (int n = 0; n < list.count(); n++) {
		QTableWidgetItem * name_item = new QTableWidgetItem;
		name_item->setText( list[n].name() );

		QTableWidgetItem * binary_item = new QTableWidgetItem;
		binary_item->setText( list[n].binary() );

		QTableWidgetItem * params_item = new QTableWidgetItem;
		params_item->setText( list[n].arguments() );

		table->setItem(n, COL_NAME, name_item);
		table->setItem(n, COL_BINARY, binary_item);
		table->setItem(n, COL_PARMS, params_item);
	}
}

QList<Player> ConfigDialog::players() {
	QList<Player> list;
	return list;
}

#include "moc_configdialog.cpp"
