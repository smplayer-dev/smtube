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

#include "myborder.h"
#include <QPainter>
#include <QPen>

MyBorder::MyBorder(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(2);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setAttribute(Qt::WA_StyledBackground, true);
}

void MyBorder::setBGColor(QColor color)
{
    QPalette pal = palette();
    pal.setColor(backgroundRole(), color);
    setPalette(pal);
}

void MyBorder::paintEvent(QPaintEvent *pe)
{
    QWidget::paintEvent(pe);
    QPainter p(this);
    QPen pen(Qt::white);
    pen.setWidth(1);
    p.setPen(pen);
    p.drawLine(0, 1, width(), 1);
}

#include "moc_myborder.cpp"
