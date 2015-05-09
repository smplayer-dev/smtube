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

#ifdef USE_PLAYERS
class TDelegate : public QItemDelegate
{
public:
	TDelegate(QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;
	virtual void setModelData(QWidget * editor, QAbstractItemModel * model,
                              const QModelIndex & index ) const;
};

TDelegate::TDelegate(QObject *parent) : QItemDelegate(parent) {
}

QWidget * TDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index) const {
	//qDebug("TDelegate::createEditor");

	if (index.column() == COL_BINARY) {
		FileChooser * fch = new FileChooser(parent);
		fch->setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::DontResolveSymlinks); // Crashes if use the KDE dialog
		fch->setText( index.model()->data(index, Qt::DisplayRole).toString() );
		return fch;
	}
	if (index.column() == COL_DIRECTPLAY) {
		QComboBox * combo = new QComboBox(parent);
		combo->addItem(Player::directPlayToString(false), false);
		combo->addItem(Player::directPlayToString(true), true);
		combo->setCurrentIndex( index.model()->data(index, Qt::UserRole).toBool() ? 1 : 0 );
		return combo;
	}
	else {
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void TDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
	if (index.column() == COL_BINARY) {
		FileChooser * fch = static_cast<FileChooser*>(editor);
		model->setData(index, fch->text() );
	}

	if (index.column() == COL_DIRECTPLAY) {
		QComboBox * combo = static_cast<QComboBox*>(editor);
		int i = combo->currentIndex();
		if (i > -1) {
			bool b = combo->itemData(i).toBool();
			model->setData(index, b, Qt::UserRole);
			model->setData(index, Player::directPlayToString(b), Qt::DisplayRole);
		}
	}
}
#endif

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
	table->setColumnCount(4);
	table->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Binary") << tr("Parameters") << tr("Support for") );
	table->verticalHeader()->setVisible(false);
	table->setAlternatingRowColors(true);

	#if QT_VERSION >= 0x050000
	table->horizontalHeader()->setSectionResizeMode(COL_PARMS, QHeaderView::Stretch);
	#else
	table->horizontalHeader()->setResizeMode(COL_PARMS, QHeaderView::Stretch);
	#endif

	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->setSelectionMode(QAbstractItemView::ExtendedSelection);

	table->setItemDelegateForColumn( COL_BINARY, new TDelegate(table) );
	table->setItemDelegateForColumn( COL_DIRECTPLAY, new TDelegate(table) );
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

		QTableWidgetItem * binary_item = new QTableWidgetItem;
		binary_item->setText( list[n].binary() );

		QTableWidgetItem * params_item = new QTableWidgetItem;
		params_item->setText( list[n].arguments() );

		QTableWidgetItem * dplay_item = new QTableWidgetItem;
		bool value = list[n].directPlay();
		dplay_item->setData(Qt::UserRole, value);
		dplay_item->setData(Qt::DisplayRole, Player::directPlayToString(value));

		table->setItem(n, COL_NAME, name_item);
		table->setItem(n, COL_BINARY, binary_item);
		table->setItem(n, COL_PARMS, params_item);
		table->setItem(n, COL_DIRECTPLAY, dplay_item);
	}

	table->setCurrentCell(0, 0);
}

QList<Player> ConfigDialog::players() {
	QList<Player> list;

	for (int n = 0; n < table->rowCount(); n++) {
		QString name = table->item(n, COL_NAME)->text();
		QString binary = table->item(n, COL_BINARY)->text();
		QString params = table->item(n, COL_PARMS)->text();
		bool direct_play = table->item(n, COL_DIRECTPLAY)->data(Qt::UserRole).toBool();

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

	QTableWidgetItem * dplay_item = new QTableWidgetItem;
	dplay_item->setData(Qt::UserRole, false);
	dplay_item->setData(Qt::DisplayRole, Player::directPlayToString(false));

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
#endif

#include "moc_configdialog.cpp"
