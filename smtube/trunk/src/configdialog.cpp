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

ConfigDialog::ConfigDialog(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f) 
{
    setupUi(this);
    connect(recordFormatCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setRecordingFormat(int)));

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

#ifdef Q_OS_WIN
    playback_group->hide();
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

void ConfigDialog::setRecordingQuality(int quality)
{
    if(recordingFormat() == (int) YTDialog::MP4)
    {
        switch(quality)
        {
        case YTDialog::FullHD:
            recordQualityCombo->setCurrentIndex(2);break;
        case YTDialog::HD:
            recordQualityCombo->setCurrentIndex(1);break;
        case YTDialog::Normal:
            recordQualityCombo->setCurrentIndex(0);break;
        }

    }
    else if(recordingFormat() == (int) YTDialog::FLV)
    {
        switch(quality)
        {
        case YTDialog::NormalFlv:
            recordQualityCombo->setCurrentIndex(2);break;
        case YTDialog::BelowNormalFlv:
            recordQualityCombo->setCurrentIndex(1);break;
        case YTDialog::LowFlv:
            recordQualityCombo->setCurrentIndex(0);break;
        }
    }
}

int ConfigDialog::recordingQuality()
{
    if(recordingFormat() == (int) YTDialog::MP4)
    {
        switch(recordQualityCombo->currentIndex())
        {
        case 0:
            return YTDialog::Normal;
        case 1:
            return YTDialog::HD;
        case 2:
            return YTDialog::FullHD;
        }

    }
    else if(recordingFormat() == (int) YTDialog::FLV)
    {
        switch(recordQualityCombo->currentIndex())
        {
        case 0:
            return YTDialog::LowFlv;
        case 1:
            return YTDialog::BelowNormalFlv;
        case 2:
            return YTDialog::NormalFlv;
        }
    }
    return 0;
}

void ConfigDialog::setRecordingFormat(int format)
{
    if(recordFormatCombo->currentIndex() != format)
        recordFormatCombo->setCurrentIndex(format);

    if(format == (int) YTDialog::MP4)
    {
        recordQualityCombo->clear();
        recordQualityCombo->insertItems(0, QStringList() << tr("Normal (360p)") << tr("HD (720p)") << tr("Full HD (1080p)"));
        switch(recordingQuality())
        {
        case 37:
            recordQualityCombo->setCurrentIndex(2);break;
        case 22:
            recordQualityCombo->setCurrentIndex(1);break;
        case 18:
            recordQualityCombo->setCurrentIndex(0);break;
        }
    }
    else if(format == (int) YTDialog::FLV)
    {
        recordQualityCombo->clear();
        recordQualityCombo->insertItems(0, QStringList() << tr("Low Quality (226p)") << tr("Medium Quality (360p)") << tr("High Quality (480p)"));
        switch(recordingQuality())
        {
        case 35:
            recordQualityCombo->setCurrentIndex(2);break;
        case 34:
            recordQualityCombo->setCurrentIndex(1);break;
        case 5:
            recordQualityCombo->setCurrentIndex(0);break;
        }
    }
}

int ConfigDialog::recordingFormat()
{
    return recordFormatCombo->currentIndex();
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

#include "moc_configdialog.cpp"
