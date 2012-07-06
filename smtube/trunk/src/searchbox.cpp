/*  smtube, a small youtube browser.
    Copyright (C) 2012 Ricardo Villalba <rvm@users.sourceforge.net>
    Copyright (C) 2010 Ori Rejwan

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

#include "searchbox.h"
#include <QToolButton>

SearchBox::SearchBox(QWidget *parent) :
    LineEditWithIcon(parent)
{
    setupButton();
    connect(button, SIGNAL(clicked()), this, SLOT(startSearch()));
    connect(this, SIGNAL(returnPressed()), this, SLOT(startSearch()));
}

void SearchBox::setupButton()
{
    setIcon( QPixmap(":/Control/search-icon.png") );
}

void SearchBox::startSearch()
{
    if(!text().isEmpty())
        emit search(text());
}

QSize SearchBox::sizeHint() const
{
    return QSize(150, LineEditWithIcon::sizeHint().height());
}

#include "moc_searchbox.cpp"
