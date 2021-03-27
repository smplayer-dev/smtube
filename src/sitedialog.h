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

#ifndef SITEDIALOG_H
#define SITEDIALOG_H

#include "ui_sitedialog.h"

#include <QDialog>
#include "sites.h"

class SiteDialog : public QDialog, public Ui::SiteDialog
{
	Q_OBJECT

public:
	SiteDialog( QWidget * parent = 0, Qt::WindowFlags f = QFlag(0) );
	~SiteDialog();

	void setSite(Site s);
	Site site();
};

#endif
