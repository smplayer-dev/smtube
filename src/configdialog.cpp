/*  SMTUBE2, a small YouTube browser for SMPlayer
    Copyright (C) 2015-2021 Ricardo Villalba <ricardo@smplayer.info>

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

#ifdef USE_SITES
#include "sitedialog.h"
#endif

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
	tabs->removeTab(find_tab("players_tab"));
#endif

#ifdef SELECT_RESOLUTION
	playback_resolution_combo->addItem( "240p", RetrieveYoutubeUrl::R240p );
	playback_resolution_combo->addItem( "360p", RetrieveYoutubeUrl::R360p );
	playback_resolution_combo->addItem( "480p", RetrieveYoutubeUrl::R480p );
	playback_resolution_combo->addItem( "720p", RetrieveYoutubeUrl::R720p );
	playback_resolution_combo->addItem( "1080p", RetrieveYoutubeUrl::R1080p );
#else
	playback_group->hide();
#endif

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

#ifndef USE_SITES
	sites_group->hide();
#endif

#ifndef USE_YT_DL
	youtube_app_group->hide();
#endif

#ifndef FONT_CHANGE
	font_label->hide();
	default_font_edit->hide();
	change_font_button->hide();
#endif

#if !defined(SELECT_RESOLUTION) && !defined(D_BUTTON) && !defined(USE_SITES)
	tabs->removeTab(find_tab("general_tab"));
#endif

#if !defined(STYLE_SWITCHING) && !defined(FONT_CHANGE)
	tabs->removeTab(find_tab("interface_tab"));
#endif
}

ConfigDialog::~ConfigDialog() {
}

int ConfigDialog::find_tab(const QString & object_name) {
	for (int n = 0; n < tabs->count(); n++) {
		if (tabs->widget(n)->objectName() == object_name) {
			return n;
		}
	}
	return -1;
}

#ifdef SELECT_RESOLUTION
void ConfigDialog::setPlaybackResolution(int resolution) {
	playback_resolution_combo->setCurrentIndex(playback_resolution_combo->findData(resolution));
}

int ConfigDialog::playbackResolution() {
	int index = playback_resolution_combo->currentIndex();
	return playback_resolution_combo->itemData(index).toInt();
}
#endif

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

#ifdef USE_SITES
void ConfigDialog::setSites(QList<Site> list) {
	site_list = list;
	refreshSitesCombo();
}

void ConfigDialog::refreshSitesCombo() {
	QString current = sites_combo->currentText();
	sites_combo->clear();
	for (int n = 0; n < site_list.count(); n++) {
		if (site_list[n].isVisible()) {
			sites_combo->addItem(site_list[n].name());
		}
	}
	setCurrentSite(current);

	delete_site_button->setEnabled(sites_combo->count() > 1);
}

void ConfigDialog::setCurrentSite(const QString & name) {
	int index = sites_combo->findText(name);
	if (index == -1) index = 0;
	sites_combo->setCurrentIndex(index);
}

QString ConfigDialog::currentSite() {
	return sites_combo->currentText();
}

int ConfigDialog::find_site(const QString & name) {
	for (int n = 0; n < site_list.count(); n++) {
		if (site_list[n].name() == name) {
			return n;
		}
	}
	return -1;
}

void ConfigDialog::on_edit_site_button_clicked() {
	qDebug("ConfigDialog::on_edit_site_button_clicked");

	int index = find_site(sites_combo->currentText());
	if (index != -1) {
		SiteDialog d(this);
		d.setSite(site_list[index]);

		if (d.exec() == QDialog::Accepted) {
			site_list[index] = d.site();
			refreshSitesCombo();
		}
	}
}

void ConfigDialog::on_add_site_button_clicked() {
	qDebug("ConfigDialog::on_add_site_button_clicked");

	SiteDialog d(this);
	if (d.exec() == QDialog::Accepted) {
		site_list << d.site();
		refreshSitesCombo();
	}
}

void ConfigDialog::on_delete_site_button_clicked() {
	qDebug("ConfigDialog::on_delete_site_button_clicked");

	if (sites_combo->count() > 1) {
		int index = find_site(sites_combo->currentText());
		if (index != -1) site_list[index].setVisible(false);
		refreshSitesCombo();
	}
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

#ifdef FONT_CHANGE
void ConfigDialog::setDefaultFont(const QFont & f) {
	default_font_edit->setText(f.toString());
}

QFont ConfigDialog::defaultFont() {
	QFont f;
	f.fromString(default_font_edit->text());
	return f;
}
#endif

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

#ifdef FONT_CHANGE
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
#endif

#ifdef USE_YT_DL
void ConfigDialog::setYtdlBin(const QString & path) {
	ytdl_bin_combo->clear();
	ytdl_bin_combo->addItem("youtube-dl", "youtube-dl");
	ytdl_bin_combo->addItem(tr("yt-dlp (based on youtube-dl with improvements)"), "yt-dlp");

	QString ytdl_bin = path;
	if (ytdl_bin.isEmpty()) ytdl_bin = YTDL_DEFAULT_BIN;
	QString basename = QFileInfo(ytdl_bin).baseName();

	int selected = 0;
	if (basename == "youtube-dl") {
		selected = 0;
		ytdl_bin_combo->setItemData(0, path);
	}
	else
	if (basename == "yt-dlp") {
		selected = 1;
		ytdl_bin_combo->setItemData(1, path);
	}
	else {
		selected = 2;
		ytdl_bin_combo->addItem(tr("Other") + " (" + basename + ")", path);
	}

	ytdl_bin_combo->setCurrentIndex(selected);
}

QString ConfigDialog::ytdlBin() {
	QString ytdl_bin = ytdl_bin_combo->itemData(ytdl_bin_combo->currentIndex()).toString();
	if (ytdl_bin == YTDL_DEFAULT_BIN) ytdl_bin = ""; // Default
	return ytdl_bin;
}

#endif

#include "moc_configdialog.cpp"
