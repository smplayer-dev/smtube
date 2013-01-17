/*  smtube, a small youtube browser.
    Copyright (C) 2012 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "yttabbar.h"
#include <QPainter>
#include <QMouseEvent>

YTTabBar::YTTabBar(QWidget *parent) :
    QTabBar(parent), hoveredIndex(-1), pressedIndex(-1)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), Qt::black);
    p.setColor(foregroundRole(), Qt::white);
    setPalette(p);
    setAutoFillBackground(true);
    setMouseTracking(true);
    QPixmap masterPix = QPixmap(":/Control/bg-tabs.png");
    selectedPix = masterPix.copy(0, 0, masterPix.width(), masterPix.height()/4);
    nonSelectedPix= masterPix.copy(0, masterPix.height()/4, masterPix.width(), masterPix.height()/4);
    hoverPix = masterPix.copy(0, masterPix.height()*2/4, masterPix.width(), masterPix.height()/4);
    clickedPix = masterPix.copy(0, masterPix.height()*3/4, masterPix.width(), masterPix.height()/4);
    sepPix = QPixmap(":/Control/bg-tabs-sep.png");
    setExpanding(true);
    setFixedHeight(masterPix.height() / 4);
}

void YTTabBar::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    for(int i=0; i < count(); ++i)
    {
        QRect r = tabRect(i);
        if(i != count() -1)
            r.adjust(0,0, -1, 0);
        if(i == pressedIndex)
        {
            p.drawPixmap(r, clickedPix);
        }
        else if(i == currentIndex())
        {
            p.drawPixmap(r, selectedPix);
        }
        else if(i == hoveredIndex)
        {
            p.drawPixmap(r, hoverPix);
        }
        else
        {
            p.drawPixmap(r, nonSelectedPix);
        }
        if(i != count() -1)
            p.drawPixmap(r.right()+1, r.top(), sepPix.width(), r.height() ,sepPix);
        p.drawText(r, Qt::AlignCenter ,tabText(i) );
    }
}

QSize YTTabBar::tabSizeHint(int index) const
{
    QSize sz;
    sz.setHeight(height());
    sz.setWidth(width()/count());
    return sz;
}

void YTTabBar::mousePressEvent(QMouseEvent *m)
{    
    pressedIndex = tabAt(m->pos());
    if(pressedIndex == currentIndex())
        pressedIndex = -1;
    QTabBar::mousePressEvent(m);
    update();
}

void YTTabBar::mouseMoveEvent(QMouseEvent *m)
{
    QTabBar::mouseMoveEvent(m);
    hoveredIndex = tabAt(m->pos());
    update();
}

void YTTabBar::mouseReleaseEvent(QMouseEvent *m)
{
    QTabBar::mouseReleaseEvent(m);
    pressedIndex = -1;
    update();
}

void YTTabBar::enterEvent(QEvent *e)
{
    QTabBar::enterEvent(e);

}

void YTTabBar::leaveEvent(QEvent *e)
{
    QTabBar::leaveEvent(e);
    hoveredIndex = -1;
    update();
}

#include "moc_yttabbar.cpp"
