/*  smtube, a small youtube browser.
    Copyright (C) 2012-2013 Ricardo Villalba <rvm@users.sourceforge.net>
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

#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include "lineedit_with_icon.h"

class SearchBox : public LineEditWithIcon
{
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent = 0);
    QSize sizeHint() const;

signals:
    void search(QString);

public slots:
    void startSearch();

protected:
    virtual void setupButton();
};

#endif // SEARCHBOX_H
