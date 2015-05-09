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
#include <QItemDelegate>
#include <QComboBox>
#include <QDebug>
#include "filechooser.h"
#include "retrieveyoutubeurl.h"
#include "playerdialog.h"

#define COL_NAME 0
#define COL_BINARY 1
#define COL_PARMS 2
#define COL_DIRECTPLAY 3

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);

	playback_quality_combo->addItem( "240p (flv)", RetrieveYoutubeUrl::FLV_240p );
	playback_quality_combo->addItem( "360p (flv)", RetrieveYoutubeUrl::FLV_360p );
	playback_quality_combo->addItem( "360p (mp4)", RetrieveYoutubeUrl::MP4_360p );
	playback_quality_combo->addItem( "360p (webm)", RetrieveYoutubeUrl::WEBM_360p );
	playback_quality_combo->addItem( "480p (flv)", RetrieveYoutubeUrl::FLV_480p );
	playback_quality_combo->addItem( "480p (webm)", RetrieveYoutubeUrl::WEBM_480p );
	playback_quality_combo->addItem( "720p (mp4)", RetrieveYoutubeUrl::MP4_720p );
	playback_quality_combo->addItem( "720p (webm)", RetrieveYoutubeUrl::WEBM_720p );
	playback_quality_combo->addItem( "1080p (mp4)", RetrieveYoutubeUrl::MP4_1080p );
	playback_quality_combo->addItem( "1080p (webm)", RetrieveYoutubeUrl::WEBM_1080p );

#ifdef USE_PLAYERS
	// Setup player's table
	table->setColumnCount(1);
	//table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Binary") << tr("Parameters") << tr("Support for") );
	table->horizontalHeader()->setVisible(false);
	table->verticalHeader()->setVisible(false);
	table->setAlternatingRowColors(true);

	#if QT_VERSION >= 0x050000
	table->horizontalHeader()->setSectionResizeMode(COL_NAME, QHeaderView::Stretch);
	#else
	table->horizontalHeader()->setResizeMode(COL_NAME, QHeaderView::Stretch);
	#endif

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::ExtendedSelection);
#endif
}

ConfigDialog::~ConfigDialog() {
}

void ConfigDialog::setPlaybackQuality(int quality) {
	playback_quality_combo->setCurrentIndex(playback_quality_combo->findData(quality));
}

int ConfigDialog::playbackQuality() {
	int index = playback_quality_combo->currentIndex();
	return playback_quality_combo->itemData(index).toInt();
}

#ifdef USE_PLAYERS
void ConfigDialog::setPlayers(QList<Player> list) {
	qDebug() << "ConfigDialog::setPlayers:" << list.count();

	table->setRowCount(list.count());
	for (int n = 0; n < list.count(); n++) {
		QTableWidgetItem * name_item = new QTableWidgetItem;
		name_item->setText( list[n].name() );
		name_item->setData(Qt::UserRole + COL_NAME, list[n].name());
		name_item->setData(Qt::UserRole + COL_BINARY, list[n].binary());
		name_item->setData(Qt::UserRole + COL_PARMS, list[n].arguments());
		name_item->setData(Qt::UserRole + COL_DIRECTPLAY, list[n].directPlay());
		name_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		table->setItem(n, COL_NAME, name_item);
	}

	table->setCurrentCell(0, 0);
}

QList<Player> ConfigDialog::players() {
	QList<Player> list;

	for (int n = 0; n < table->rowCount(); n++) {
		QTableWidgetItem * i = table->item(n, COL_NAME);
		QString name = i->data(Qt::UserRole + COL_NAME).toString();
		QString binary = i->data(Qt::UserRole + COL_BINARY).toString();
		QString params = i->data(Qt::UserRole + COL_PARMS).toString();
		bool direct_play = i->data(Qt::UserRole + COL_DIRECTPLAY).toBool();

		if (!name.isEmpty() && !binary.isEmpty()) {
			list << Player(name, binary, params, direct_play);
		}
	}

	return list;
}

void ConfigDialog::on_edit_button_clicked() {
	qDebug() << "ConfigDialog::on_edit_button_clicked";

	int row = table->currentRow();
	QTableWidgetItem * i = table->item(row, COL_NAME);

	QString name = i->data(Qt::UserRole + COL_NAME).toString();
	QString binary = i->data(Qt::UserRole + COL_BINARY).toString();
	QString parms = i->data(Qt::UserRole + COL_PARMS).toString();
	bool direct_play = i->data(Qt::UserRole + COL_DIRECTPLAY).toBool();

	PlayerDialog d(this);
	d.setName(name);
	d.setBinary(binary);
	d.setParameters(parms);
	d.setDirectPlay(direct_play);

	if (d.exec() == QDialog::Accepted) {
		i->setText(d.name());
		i->setData(Qt::UserRole + COL_NAME, d.name());
		i->setData(Qt::UserRole + COL_BINARY, d.binary());
		i->setData(Qt::UserRole + COL_PARMS, d.parameters());
		i->setData(Qt::UserRole + COL_DIRECTPLAY, d.directPlay());
	}
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

	QTableWidgetItem * name_item = new QTableWidgetItem;
	name_item->setText("");
	name_item->setData(Qt::UserRole + COL_NAME, "");
	name_item->setData(Qt::UserRole + COL_BINARY, "");
	name_item->setData(Qt::UserRole + COL_PARMS, "%u");
	name_item->setData(Qt::UserRole + COL_DIRECTPLAY, false);
	name_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

	table->setItem(row, COL_NAME, name_item);

	table->setCurrentCell(row, table->currentColumn());

	on_edit_button_clicked();
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
#endif

#include "moc_configdialog.cpp"
