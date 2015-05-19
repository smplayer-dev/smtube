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

#ifdef USE_PLAYERS
	connect(edit_button, SIGNAL(clicked()), this, SLOT(editCurrentItem()));
	connect(table, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(editCurrentItem()));
#else
	players_group->hide();
#endif

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

	for (int n = 0; n < list.count(); n++) {
		QListWidgetItem * i = new QListWidgetItem;
		i->setText( list[n].name() );
		i->setData(Qt::UserRole + COL_NAME, list[n].name());
		i->setData(Qt::UserRole + COL_BINARY, list[n].binary());
		i->setData(Qt::UserRole + COL_PARMS, list[n].arguments());
		i->setData(Qt::UserRole + COL_DIRECTPLAY, list[n].directPlay());

		table->addItem(i);
	}

	table->setCurrentRow(0);
}

QList<Player> ConfigDialog::players() {
	QList<Player> list;

	for (int n = 0; n < table->count(); n++) {
		QListWidgetItem * i = table->item(n);
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

void ConfigDialog::setDefaultPlayers(QList<Player> players) {
	default_players = players;
	updateAddPlayersButton();
}

void ConfigDialog::editCurrentItem() {
	qDebug() << "ConfigDialog::editCurrentItem";

	QListWidgetItem * i = table->currentItem();

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
	qDebug() << "ConfigDialog::on_delete_button_clicked: row:" << row;

	QListWidgetItem * i = table->takeItem(row);
	delete i;

	updateAddPlayersButton();
}

void ConfigDialog::on_add_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_add_button_clicked: row:" << row;
	row++;

	QListWidgetItem * i = new QListWidgetItem;
	i->setText("");
	i->setData(Qt::UserRole + COL_NAME, "");
	i->setData(Qt::UserRole + COL_BINARY, "");
	i->setData(Qt::UserRole + COL_PARMS, "%u");
	i->setData(Qt::UserRole + COL_DIRECTPLAY, false);

	table->insertItem(row, i);
	table->setCurrentRow(row);

	editCurrentItem();

	updateAddPlayersButton();
}

void ConfigDialog::on_up_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_up_button_clicked: row:" << row;

	QListWidgetItem * i = table->takeItem(row);
	table->insertItem(row-1, i);
	table->setCurrentRow(row-1);
}

void ConfigDialog::on_down_button_clicked() {
	int row = table->currentRow();
	qDebug() << "ConfigDialog::on_down_button_clicked: row:" << row;

	QListWidgetItem * i = table->takeItem(row);
	table->insertItem(row+1, i);
	table->setCurrentRow(row+1);
}

void ConfigDialog::on_addplayers_button_clicked() {
	qDebug() << "ConfigDialog::on_addplayers_button_clicked";

	for (int n = 0; n < default_players.count(); n++) {
		QString name = default_players[n].name();
		QList<QListWidgetItem *> items = table->findItems(name, Qt::MatchExactly);
		if (items.isEmpty()) {
			qDebug() << "ConfigDialog::on_addplayers_button_clicked: player" << name << "not found";
			// Add player
			QListWidgetItem * i = new QListWidgetItem;
			i->setText( name );
			i->setData(Qt::UserRole + COL_NAME, name);
			i->setData(Qt::UserRole + COL_BINARY, default_players[n].binary());
			i->setData(Qt::UserRole + COL_PARMS, default_players[n].arguments());
			i->setData(Qt::UserRole + COL_DIRECTPLAY, default_players[n].directPlay());
			table->addItem(i);
		}
	}
	addplayers_button->setEnabled(false);
}

void ConfigDialog::updateAddPlayersButton() {
	bool available_player = false;

	for (int n = 0; n < default_players.count(); n++) {
		QString name = default_players[n].name();
		QList<QListWidgetItem *> items = table->findItems(name, Qt::MatchExactly);
		if (items.isEmpty()) {
			available_player = true;
		}
	}

	addplayers_button->setEnabled(available_player);
}
#endif

#include "moc_configdialog.cpp"
