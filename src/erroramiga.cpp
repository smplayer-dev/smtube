/*  smtube, a small youtube browser.
    Copyright (C) 2012-2015 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2014 zzd10h

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

#include <QVBoxLayout>
#include <QDesktopWidget>

#include "erroramiga.h"

ErrorAmiga * ErrorAmiga::window = 0;

ErrorAmiga::ErrorAmiga(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f)
{
	// put the window at the center of the Workbench screen
	QDesktopWidget workbench;
	QRect dimension_workbench = workbench.screenGeometry();
	int x = dimension_workbench.width()/2 - this->width()/2;
	int y = dimension_workbench.height()/2 - this->height()/2;
	move(x,y);

	QVBoxLayout *layoutMain = new QVBoxLayout;

	QGridLayout *layoutGrid = new QGridLayout; 

	QLabel *labelEmpty1 = new QLabel("", this);
	QLabel *labelEmpty2 = new QLabel("", this);
	QPushButton *buttonQuit = new QPushButton(tr("OK"));
	QWidget::connect(buttonQuit, SIGNAL(clicked()), this, SLOT(close())); 

	layoutGrid->addWidget(labelEmpty1, 0, 0);
	layoutGrid->addWidget(buttonQuit, 0, 1);
	layoutGrid->addWidget(labelEmpty2, 0, 2);

	QHBoxLayout *layout = new QHBoxLayout;
	QLabel *picture = new QLabel;
	picture->setPixmap( QPixmap(":/icons/logo.png").scaledToHeight(64, Qt::SmoothTransformation) );

	labelTitle = new QLabel("", this);

	layout->addWidget(picture);
	layout->addWidget(labelTitle);
	layout->addWidget(labelEmpty1);

	layoutMain->addLayout(layout);
	layoutMain->addLayout(layoutGrid);

	setLayout(layoutMain);
}

void ErrorAmiga::setText(const QString & text) {
	QString errorAmiga = text;

	errorAmiga.insert(errorAmiga.indexOf('"'),"\n");

	int errorlength = errorAmiga.length() ;

	int i = errorAmiga.indexOf('"') ;
	int j = 0 ;

	for (i = errorAmiga.indexOf('"'); i < errorlength ; i++)
	{
		j++ ;
		if (j == 50)
		{
			errorAmiga.insert(i,"\n");
			j = 0 ;
		}
	}

	labelTitle->setText(errorAmiga);
}

void ErrorAmiga::showError(const QString & title, const QString & error)
{
	if (window == 0) {
		window = new ErrorAmiga(0);
	}
	window->setWindowTitle(title);
	window->setText(error);
	window->show();
}

#include "moc_erroramiga.cpp"
