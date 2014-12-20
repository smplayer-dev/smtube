/*  smtube, a small youtube browser.
    Copyright (C) 2012-2014 Ricardo Villalba <rvm@users.sourceforge.net>

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
#include "ytdialog.h"
#include "retrieveyoutubeurl.h"
#include "regions.h"
#include <QNetworkProxy>

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
    setupUi(this);

    directory_edit->setDialogType(FileChooser::GetDirectory);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);

	// Populate the region_combo combobox
	QMap<QString,QString> regions_list = Regions::list();
	QMapIterator<QString,QString> it(regions_list);
	while (it.hasNext()) {
		it.next();
		region_combo->addItem( it.value(), it.key() );
	}

    time_combo->addItem( tr("All time"), "all_time");
    time_combo->addItem( tr("This month"), "this_month");
    time_combo->addItem( tr("This week"), "this_week");
    time_combo->addItem( tr("Today"), "today");

    // Sort items in region_combo
    region_combo->model()->sort(0);

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

	record_quality_combo->addItem( "240p (flv)", RetrieveYoutubeUrl::FLV_240p );
	record_quality_combo->addItem( "360p (flv)", RetrieveYoutubeUrl::FLV_360p );
	record_quality_combo->addItem( "360p (mp4)", RetrieveYoutubeUrl::MP4_360p );
	record_quality_combo->addItem( "360p (webm)", RetrieveYoutubeUrl::WEBM_360p );
	record_quality_combo->addItem( "480p (flv)", RetrieveYoutubeUrl::FLV_480p );
	record_quality_combo->addItem( "480p (webm)", RetrieveYoutubeUrl::WEBM_480p );
	record_quality_combo->addItem( "720p (mp4)", RetrieveYoutubeUrl::MP4_720p );
	record_quality_combo->addItem( "720p (webm)", RetrieveYoutubeUrl::WEBM_720p );
	record_quality_combo->addItem( "1080p (mp4)", RetrieveYoutubeUrl::MP4_1080p );
	record_quality_combo->addItem( "1080p (webm)", RetrieveYoutubeUrl::WEBM_1080p );

	proxy_type_combo->addItem( tr("HTTP"), QNetworkProxy::HttpProxy);
	proxy_type_combo->addItem( tr("SOCKS5"), QNetworkProxy::Socks5Proxy);

#ifdef Q_OS_WIN
	playback_group->hide();
#else
#ifndef USE_PLAYERS
	player_widget->hide();
#endif
#endif

#ifndef YT_DL
	recording_group->hide();
#endif

#ifdef USE_PLAYERS
    connect(player_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(playerChanged(int)));
#endif

    adjustSize();
}

ConfigDialog::~ConfigDialog() {
}

void ConfigDialog::setRecordingDirectory(const QString & folder) 
{
    directory_edit->setText(folder);
}

QString ConfigDialog::recordingDirectory() 
{
    return directory_edit->text();
}

void ConfigDialog::setRecordingQuality(int quality) {
	record_quality_combo->setCurrentIndex(record_quality_combo->findData(quality));
}

int ConfigDialog::recordingQuality() {
	int index = record_quality_combo->currentIndex();
	return record_quality_combo->itemData(index).toInt();
}

#ifdef USE_PLAYERS
void ConfigDialog::setPlayerNames(QStringList names)
{
    player_combo->clear();
    player_combo->addItems(names);
}

void ConfigDialog::setPlayer(QString name)
{
    int i = player_combo->findText(name);
    if (i == -1) i = 0;
    player_combo->setCurrentIndex(i);
}

QString ConfigDialog::player()
{
    return player_combo->currentText();
}

void ConfigDialog::playerChanged(int i) {
	if (player_combo->itemText(i) == "SMPlayer") {
		playback_quality_combo->setEnabled(false);
		playback_quality_label->setEnabled(false);
	} else {
		playback_quality_combo->setEnabled(true);
		playback_quality_label->setEnabled(true);
	}
}
#endif

void ConfigDialog::setRegion(const QString & region) {
    int i = region_combo->findData(region);
    if (i != -1) region_combo->setCurrentIndex(i);
}

void ConfigDialog::setPeriod(const QString & period) {
    int i = time_combo->findData(period);
    if (i != -1) time_combo->setCurrentIndex(i);
}

QString ConfigDialog::region() {
    return region_combo->itemData(region_combo->currentIndex()).toString();
}

QString ConfigDialog::period() {
    return time_combo->itemData(time_combo->currentIndex()).toString();
}

void ConfigDialog::setPlaybackQuality(int quality) {
	playback_quality_combo->setCurrentIndex(playback_quality_combo->findData(quality));
}

int ConfigDialog::playbackQuality() {
	int index = playback_quality_combo->currentIndex();
	return playback_quality_combo->itemData(index).toInt();
}

void ConfigDialog::setUseProxy(bool b) {
	use_proxy_check->setChecked(b);
}

bool ConfigDialog::useProxy() {
	return use_proxy_check->isChecked();
}

void ConfigDialog::setProxyHostname(const QString & host) {
	proxy_hostname_edit->setText(host);
}

QString ConfigDialog::proxyHostname() {
	return proxy_hostname_edit->text();
}

void ConfigDialog::setProxyPort(int port) {
	proxy_port_spin->setValue(port);
}

int ConfigDialog::proxyPort() {
	return proxy_port_spin->value();
}

void ConfigDialog::setProxyUsername(const QString & username) {
	proxy_username_edit->setText(username);
}

QString ConfigDialog::proxyUsername() {
	return proxy_username_edit->text();
}

void ConfigDialog::setProxyPassword(const QString & pass) {
	proxy_password_edit->setText(pass);
}

QString ConfigDialog::proxyPassword() {
	return proxy_password_edit->text();
}

void ConfigDialog::setProxyType(int type) {
	int index = proxy_type_combo->findData(type);
	if (index == -1) index = 0;
	proxy_type_combo->setCurrentIndex(index);
}

int ConfigDialog::proxyType() {
	int index = proxy_type_combo->currentIndex();
	return proxy_type_combo->itemData(index).toInt();
}

#include "moc_configdialog.cpp"
