/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2018 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include <QFontDialog>
#include <QStyleFactory>
#include <QDebug>
#include "filechooser.h"
#include "retrieveyoutubeurl.h"
#include "playerdialog.h"

#if QT_VERSION >= 0x050000
#include "myscroller.h"
#endif

#define COL_NAME 0
#define COL_BINARY 1
#define COL_PARMS 2
#define COL_STREAMINGSITES 3
#define COL_ONLINETV 4
#define COL_MEDIA 5
#define COL_QUALITY 6

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);

#ifdef USE_PLAYERS
	connect(edit_button, SIGNAL(clicked()), this, SLOT(editCurrentItem()));
	connect(table, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(editCurrentItem()));

	#if QT_VERSION >= 0x050000
	MyScroller::setScroller(table);
	#endif
#else
	tabs->setTabEnabled(1, false);
#endif

	playback_resolution_combo->addItem( "240p", RetrieveYoutubeUrl::R240p );
	playback_resolution_combo->addItem( "360p", RetrieveYoutubeUrl::R360p );
	playback_resolution_combo->addItem( "480p", RetrieveYoutubeUrl::R480p );
	playback_resolution_combo->addItem( "720p", RetrieveYoutubeUrl::R720p );
	playback_resolution_combo->addItem( "1080p", RetrieveYoutubeUrl::R1080p );

#ifdef D_BUTTON
	external_download_combo->addItem("http://9xbuddy.com/download?url=%YT_URL%");
	external_download_combo->addItem("http://www.savefrom.net/#url=%YT_URL%");
	//external_download_combo->addItem("http://www.telechargerunevideo.com/en/?v=%YT_ID%");
	external_download_combo->addItem("http://www.dlyoutube.com/watch?v=%YT_ID%");
#else
	download_group->hide();
	//adjustSize();
#endif

#ifdef STYLE_SWITCHING
	style_combo->addItem( tr("Default") );
	style_combo->addItems( QStyleFactory::keys() );
#else
	style_label->hide();
	style_combo->hide();
#endif
}

ConfigDialog::~ConfigDialog() {
}

void ConfigDialog::setPlaybackResolution(int resolution) {
	playback_resolution_combo->setCurrentIndex(playback_resolution_combo->findData(resolution));
}

int ConfigDialog::playbackResolution() {
	int index = playback_resolution_combo->currentIndex();
	return playback_resolution_combo->itemData(index).toInt();
}

#ifdef D_BUTTON
void ConfigDialog::setAddDownloadButton(bool b) {
	add_download_check->setChecked(b);
}

bool ConfigDialog::addDownloadButton() {
	return add_download_check->isChecked();
}

void ConfigDialog::setExternalDownloadUrl(const QString & url) {
	int pos = external_download_combo->findText(url);
	if (pos == -1) {
		external_download_combo->addItem(url);
		external_download_combo->setCurrentIndex(external_download_combo->count()-1);
	} else {
		external_download_combo->setCurrentIndex(pos);
	}
	/* external_download_combo->setEditText(url); */
}

QString ConfigDialog::externalDownloadUrl() {
	return external_download_combo->currentText();
}
#endif

#ifdef USE_PLAYERS
void ConfigDialog::setPlayers(QList<Player> list) {
	qDebug() << "ConfigDialog::setPlayers:" << list.count();

	for (int n = 0; n < list.count(); n++) {
		QListWidgetItem * i = new QListWidgetItem;
		i->setText( list[n].name() );
		i->setData(Qt::UserRole + COL_NAME, list[n].name());
		i->setData(Qt::UserRole + COL_BINARY, list[n].binary());
		i->setData(Qt::UserRole + COL_PARMS, list[n].arguments());
		i->setData(Qt::UserRole + COL_STREAMINGSITES, list[n].supportStreamingSites());
		i->setData(Qt::UserRole + COL_ONLINETV, list[n].supportOnlineTV());
		i->setData(Qt::UserRole + COL_MEDIA, list[n].supportedMedia());
		i->setData(Qt::UserRole + COL_QUALITY, list[n].preferredResolution());

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
		bool support_streaming_sites = i->data(Qt::UserRole + COL_STREAMINGSITES).toBool();
		bool support_online_tv = i->data(Qt::UserRole + COL_ONLINETV).toBool();
		int supported_media = i->data(Qt::UserRole + COL_MEDIA).toInt();
		int quality = i->data(Qt::UserRole + COL_QUALITY).toInt();

		if (!name.isEmpty() && !binary.isEmpty()) {
			list << Player(name, binary, params, support_streaming_sites, support_online_tv, (Player::Media) supported_media, quality);
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
	bool support_streaming_sites = i->data(Qt::UserRole + COL_STREAMINGSITES).toBool();
	bool support_online_tv = i->data(Qt::UserRole + COL_ONLINETV).toBool();
	int supported_media = i->data(Qt::UserRole + COL_MEDIA).toInt();
	int quality = i->data(Qt::UserRole + COL_QUALITY).toInt();

	PlayerDialog d(this);
	d.setName(name);
	d.setBinary(binary);
	d.setParameters(parms);
	d.setSupportStreamingSites(support_streaming_sites);
	d.setSupportOnlineTV(support_online_tv);
	d.setMedia(supported_media);
	d.setResolution(quality);

	if (d.exec() == QDialog::Accepted) {
		i->setText(d.name());
		i->setData(Qt::UserRole + COL_NAME, d.name());
		i->setData(Qt::UserRole + COL_BINARY, d.binary());
		i->setData(Qt::UserRole + COL_PARMS, d.parameters());
		i->setData(Qt::UserRole + COL_STREAMINGSITES, d.supportStreamingSites());
		i->setData(Qt::UserRole + COL_ONLINETV, d.supportOnlineTV());
		i->setData(Qt::UserRole + COL_MEDIA, d.media());
		i->setData(Qt::UserRole + COL_QUALITY, d.resolution());
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
	i->setData(Qt::UserRole + COL_STREAMINGSITES, false);
	i->setData(Qt::UserRole + COL_ONLINETV, false);
	i->setData(Qt::UserRole + COL_MEDIA, Player::VideoAudio);
	i->setData(Qt::UserRole + COL_QUALITY, -1);

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
			i->setData(Qt::UserRole + COL_STREAMINGSITES, default_players[n].supportStreamingSites());
			i->setData(Qt::UserRole + COL_ONLINETV, default_players[n].supportOnlineTV());
			i->setData(Qt::UserRole + COL_MEDIA, default_players[n].supportedMedia());
			i->setData(Qt::UserRole + COL_QUALITY, default_players[n].preferredResolution());
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

void ConfigDialog::setDefaultFont(const QFont & f) {
	default_font_edit->setText(f.toString());
}

QFont ConfigDialog::defaultFont() {
	QFont f;
	f.fromString(default_font_edit->text());
	return f;
}

#ifdef STYLE_SWITCHING
void ConfigDialog::setStyle(const QString & style) {
	qDebug() << "ConfigDialog::setStyle:" << style;

	if (style.isEmpty()) {
		style_combo->setCurrentIndex(0);
	} else {
		int i = style_combo->findText(style, Qt::MatchFixedString);
		if (i < 0) i = 0;
		style_combo->setCurrentIndex(i);
	}
}

QString ConfigDialog::style() {
	if (style_combo->currentIndex() == 0)
		return "";
	else 
		return style_combo->currentText().toLower();
}
#endif

void ConfigDialog::on_change_font_button_clicked() {
	QFont f = qApp->font();

	if (!default_font_edit->text().isEmpty()) {
		f.fromString(default_font_edit->text());
	}

	bool ok;
	f = QFontDialog::getFont( &ok, f, this);

	if (ok) {
		default_font_edit->setText( f.toString() );
	}
}

#include "moc_configdialog.cpp"
