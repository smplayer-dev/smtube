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

#ifndef PLAYERDIALOG_H
#define PLAYERDIALOG_H

#include "ui_playerdialog.h"

#include <QDialog>

class PlayerDialog : public QDialog, public Ui::PlayerDialog
{
	Q_OBJECT

public:
	PlayerDialog( QWidget * parent = 0, Qt::WindowFlags f = QFlag(0) );
	~PlayerDialog();

	void setName(const QString & name);
	void setBinary(const QString & binary);
	void setParameters(const QString & parms);
	void setSupportStreamingSites(bool b);
	void setSupportOnlineTV(bool b);
	void setMedia(int m);
	void setResolution(int r);

	QString name();
	QString binary();
	QString parameters();
	bool supportStreamingSites();
	bool supportOnlineTV();
	int media();
	int resolution();

protected slots:
	void streamingSitesChanged(bool);
};

#endif

