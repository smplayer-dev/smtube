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

#include "playerdialog.h"
#include "players.h"
#include "retrieveyoutubeurl.h"

PlayerDialog::PlayerDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
	setupUi(this);

	media_combo->addItem( tr("None"), Player::None );
	media_combo->addItem( tr("Video"), Player::Video );
	media_combo->addItem( tr("Audio"), Player::Audio );
	media_combo->addItem( tr("Video and audio"), Player::VideoAudio );

	quality_combo->addItem( tr("Default"), -1 );
	quality_combo->addItem( "240p (flv)", RetrieveYoutubeUrl::FLV_240p );
	quality_combo->addItem( "360p (flv)", RetrieveYoutubeUrl::FLV_360p );
	quality_combo->addItem( "360p (mp4)", RetrieveYoutubeUrl::MP4_360p );
	quality_combo->addItem( "360p (webm)", RetrieveYoutubeUrl::WEBM_360p );
	quality_combo->addItem( "480p (flv)", RetrieveYoutubeUrl::FLV_480p );
	quality_combo->addItem( "480p (webm)", RetrieveYoutubeUrl::WEBM_480p );
	quality_combo->addItem( "720p (mp4)", RetrieveYoutubeUrl::MP4_720p );
	quality_combo->addItem( "720p (webm)", RetrieveYoutubeUrl::WEBM_720p );
	quality_combo->addItem( "1080p (mp4)", RetrieveYoutubeUrl::MP4_1080p );
	quality_combo->addItem( "1080p (webm)", RetrieveYoutubeUrl::WEBM_1080p );

	connect(streamingsites_check, SIGNAL(toggled(bool)), this, SLOT(streamingSitesChanged(bool)));
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

void PlayerDialog::setSupportStreamingSites(bool b) {
	streamingsites_check->setChecked(b);
}

void PlayerDialog::setSupportOnlineTV(bool b) {
	onlinetv_check->setChecked(b);
}

void PlayerDialog::setMedia(int m) {
	int i = media_combo->findData(m);
	if (i == -1) i = 0;
	media_combo->setCurrentIndex(i);
}

void PlayerDialog::setQuality(int q) {
	int i = quality_combo->findData(q);
	if (i == -1) i = 0;
	quality_combo->setCurrentIndex(i);
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

bool PlayerDialog::supportStreamingSites() {
	return streamingsites_check->isChecked();
}

bool PlayerDialog::supportOnlineTV() {
	return onlinetv_check->isChecked();
}

int PlayerDialog::media() {
	int i = media_combo->currentIndex();
	return media_combo->itemData(i).toInt();
}

int PlayerDialog::quality() {
	int i = quality_combo->currentIndex();
	return quality_combo->itemData(i).toInt();
}

void PlayerDialog::streamingSitesChanged(bool b) {
	quality_label->setEnabled(!b);
	quality_combo->setEnabled(!b);
}

#include "moc_playerdialog.cpp"


