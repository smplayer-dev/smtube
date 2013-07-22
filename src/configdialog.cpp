/*  smtube, a small youtube browser.
    Copyright (C) 2012-2013 Ricardo Villalba <rvm@users.sourceforge.net>

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

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
    setupUi(this);
 
    directory_edit->setDialogType(FileChooser::GetDirectory);
    //layout()->setSizeConstraint(QLayout::SetFixedSize);

    region_combo->addItem( tr("Argentina"), "AR");
    region_combo->addItem( tr("Australia"), "AU");
    //region_combo->addItem( tr("Austria"), "AT"); // Invalid code :?
    region_combo->addItem( tr("Belgium"), "BE");
    region_combo->addItem( tr("Brazil"), "BR");
    region_combo->addItem( tr("Canada"), "CA");
    region_combo->addItem( tr("Chile"), "CL");
    region_combo->addItem( tr("Colombia"), "CO");
    region_combo->addItem( tr("Czech Republic"), "CZ");
    region_combo->addItem( tr("Egypt"), "EG");
    region_combo->addItem( tr("France"), "FR");
    region_combo->addItem( tr("Germany"), "DE");
    region_combo->addItem( tr("Great Britain"), "GB");
    region_combo->addItem( tr("Hong Kong"), "HK");
    region_combo->addItem( tr("Hungary"), "HU");
    region_combo->addItem( tr("India"), "IN");
    region_combo->addItem( tr("Ireland"), "IE");
    region_combo->addItem( tr("Israel"), "IL");
    region_combo->addItem( tr("Italy"), "IT");
    region_combo->addItem( tr("Japan"), "JP");

    region_combo->addItem( tr("Jordan"), "JO");
    region_combo->addItem( tr("Malaysia"), "MY");
    region_combo->addItem( tr("Mexico"), "MX");
    region_combo->addItem( tr("Morocco"), "MA");
    region_combo->addItem( tr("Netherlands"), "NL");
    region_combo->addItem( tr("New Zealand"), "NZ");
    region_combo->addItem( tr("Peru"), "PE");
    region_combo->addItem( tr("Philippines"), "PH");
    region_combo->addItem( tr("Poland"), "PL");
    region_combo->addItem( tr("Russia"), "RU");
    region_combo->addItem( tr("Saudi Arabia"), "SA");
    region_combo->addItem( tr("Singapore"), "SG");
    region_combo->addItem( tr("South Africa"), "ZA");
    region_combo->addItem( tr("South Korea"), "KR");
    region_combo->addItem( tr("Spain"), "ES");
    region_combo->addItem( tr("Sweden"), "SE");
    //region_combo->addItem( tr("Switzerland"), "CH"); // Invalid code :?
    region_combo->addItem( tr("Taiwan"), "TW");
    region_combo->addItem( tr("United Arab Emirates"), "AE");
    region_combo->addItem( tr("United States"), "US");

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

#ifdef Q_OS_WIN
    playback_group->hide();
#endif

    connect(player_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(playerChanged(int)));

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

#include "moc_configdialog.cpp"
