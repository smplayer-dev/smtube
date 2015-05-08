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
#define COL_DIRECTPLAY 3

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);

	// Setup player's table
	table->setColumnCount(4);
	table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Binary") << tr("Parameters") );

	table->setAlternatingRowColors(true);

#if QT_VERSION >= 0x050000
	table->horizontalHeader()->setSectionResizeMode(COL_BINARY, QHeaderView::Stretch);
#else
	table->horizontalHeader()->setResizeMode(COL_BINARY, QHeaderView::Stretch);
#endif

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

		QTableWidgetItem * dplay_item = new QTableWidgetItem;
		dplay_item->setCheckState( list[n].directPlay() ? Qt::Checked : Qt::Unchecked );

		table->setItem(n, COL_NAME, name_item);
		table->setItem(n, COL_BINARY, binary_item);
		table->setItem(n, COL_PARMS, params_item);
		table->setItem(n, COL_DIRECTPLAY, dplay_item);
	}
}

QList<Player> ConfigDialog::players() {
	QList<Player> list;
	return list;
}

void ConfigDialog::on_delete_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_delete_button_clicked: current_row:" << row;

	if (row > -1) table->removeRow(row);

	if (row >= table->rowCount()) row--;
	table->setCurrentCell(row, table->currentColumn());
}

void ConfigDialog::on_add_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_add_button_clicked: current_row:" << row;
	row++;
	table->insertRow(row);

	QTableWidgetItem * dplay_item = new QTableWidgetItem;
	dplay_item->setCheckState(Qt::Unchecked);

	table->setItem(row, COL_NAME, new QTableWidgetItem);
	table->setItem(row, COL_BINARY, new QTableWidgetItem);
	table->setItem(row, COL_PARMS, new QTableWidgetItem);
	table->setItem(row, COL_DIRECTPLAY, dplay_item);

	table->setCurrentCell(row, table->currentColumn());
}

void ConfigDialog::on_up_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_up_button_clicked: current_row:" << row;

	if (row == 0) return;

	// take whole rows
	QList<QTableWidgetItem*> source_items = takeRow(row);
	QList<QTableWidgetItem*> dest_items = takeRow(row-1);
 
	// set back in reverse order
	setRow(row, dest_items);
	setRow(row-1, source_items);

	table->setCurrentCell(row-1, table->currentColumn());
}

void ConfigDialog::on_down_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_down_button_clicked: current_row:" << row;

	if ((row+1) >= table->rowCount()) return;

	// take whole rows
	QList<QTableWidgetItem*> source_items = takeRow(row);
	QList<QTableWidgetItem*> dest_items = takeRow(row+1);
 
	// set back in reverse order
	setRow(row, dest_items);
	setRow(row+1, source_items);

	table->setCurrentCell(row+1, table->currentColumn());
}

// takes and returns the whole row
QList<QTableWidgetItem*> ConfigDialog::takeRow(int row) {
	QList<QTableWidgetItem*> rowItems;
	for (int col = 0; col < table->columnCount(); ++col)
	{
		rowItems << table->takeItem(row, col);
	}
	return rowItems;
}

// sets the whole row
void ConfigDialog::setRow(int row, const QList<QTableWidgetItem*>& rowItems) {
	for (int col = 0; col < table->columnCount(); ++col)
	{
		table->setItem(row, col, rowItems.at(col));
	}
}

#include "moc_configdialog.cpp"
