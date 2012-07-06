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

#ifndef YTTABBAR_H
#define YTTABBAR_H

#include <QTabBar>

class YTTabBar : public QTabBar
{
Q_OBJECT
public:
    explicit YTTabBar(QWidget *parent = 0);    

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
    QSize tabSizeHint(int index) const;
    void mousePressEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *m);
    void mouseMoveEvent(QMouseEvent *m);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);



private:
    QPixmap selectedPix;
    QPixmap nonSelectedPix;
    QPixmap clickedPix;
    QPixmap hoverPix;    
    QPixmap sepPix;
    int hoveredIndex;
    int pressedIndex;


};

#endif // YTTABBAR_H
