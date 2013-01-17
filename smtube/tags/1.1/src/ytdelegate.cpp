/*  umplayer, GUI front-end for mplayer.
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

#include "ytdelegate.h"
#include "ytdialog.h"
#include <QPainter>
#include <QFont>
#include <QVariant>
#include <QTextLayout>
#include <QTextLine>
#include <QDebug>

#define PIXWIDTH 120



YTDelegate::YTDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    playOverlay = QPixmap(":/Control/button-youtube-play-overlay.png");
    hoverBackground = QPixmap(":/Control/sc-hover-background.png");
    durationBackground = QPixmap(":/Control/yt-duration-bg.png");
    QPixmap pixBorder(":/Control/yt-thumb-overlay.png");
    pixBorderOverlay1 = pixBorder.copy(0, 0, pixBorder.width(), pixBorder.height() / 3 );
    pixBorderOverlay2 = pixBorder.copy(0, pixBorder.height() / 3  , pixBorder.width(), pixBorder.height() / 3 );
    pixBorderOverlayHover = pixBorder.copy(0, pixBorder.height()*2/3, pixBorder.width(), pixBorder.height()/3 );
}

void YTDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    bool white = false;    
    if(index.data(YTDialog::Clicked).toBool())
    {
        painter->drawPixmap(option.rect.adjusted(0,0,0, -1), hoverBackground );
        white = true;
    }
    else
    {
        if(index.row() % 2 == 0)
        {
            painter->fillRect(option.rect, QColor("#f0f6ff"));
        }
        else
        {
            painter->fillRect(option.rect, Qt::white);
        }
    }
    SingleVideoItem* item = qVariantValue<SingleVideoItem*>(index.data());    
    QSize sz = option.rect.size(); --sz.rheight();
    painter->translate(option.rect.x() , option.rect.y());
    painter->drawPixmap(0, 0, PIXWIDTH, sz.height(), item->pix );    
    if(index.data(YTDialog::Clicked).toBool())
    {
        painter->drawPixmap(0, 0, PIXWIDTH, sz.height(), pixBorderOverlayHover);
        QPoint pop = QRect(0,0,PIXWIDTH, sz.height()).center() - playOverlay.rect().center();
        painter->drawPixmap(pop, playOverlay);
    }
    else
    {
        painter->drawPixmap(0, 0, PIXWIDTH, sz.height(), index.row() %2 == 0 ? pixBorderOverlay1 : pixBorderOverlay2 );
    }
    QFont font = option.font;    
    font.setPixelSize(11);
    QFontMetrics fm(font);

    // total time
    QString totalTime = timeString(item->totalTime);
    QRect ttRect = fm.boundingRect(totalTime);        
    ttRect.adjust(-2 , 0, 4, 2);
    ttRect.moveBottomRight(QPoint(PIXWIDTH -9, sz.height() -11 ));
    painter->setPen(Qt::white);
    painter->setFont(font);
    painter->setOpacity(0.7);
    painter->drawPixmap(ttRect, durationBackground);
    ttRect.adjust(1, 1, 1, 0);
    painter->drawText(ttRect, Qt::AlignHCenter | Qt::AlignTop, totalTime);
    painter->setOpacity(1);

    // Header

    font.setPixelSize(14);
    fm = QFontMetrics(font);
    painter->setFont(font);
    painter->setPen(cr(Qt::black, white));
    QRect headerRect = fm.boundingRect(item->header);
    headerRect = QRect( PIXWIDTH + 4 , 7, sz.width() - PIXWIDTH -  8, headerRect.height() + 4  );
    painter->drawText(headerRect, Qt::AlignTop | Qt::AlignLeft | Qt::TextSingleLine,
                      fm.elidedText(item->header, Qt::ElideRight, headerRect.width()), &headerRect );


    // Footer
    font.setPixelSize(11);
    fm = QFontMetrics(font);    
    painter->setPen(cr(QColor("#636363"), white));
    painter->setFont(font);    
    QRect footerRect(PIXWIDTH + 4, ttRect.top(), sz.width() - PIXWIDTH - 8, ttRect.height());
    painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignLeft, item->date.toString("yyyy-MM-dd"));
    painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, tr("%L1 views").arg(item->views));

    //Description    
    font.setPixelSize(12);
    painter->setFont(font);    
    QRect descRect( PIXWIDTH + 4, headerRect.bottom() + 7, sz.width() - PIXWIDTH - 8, ttRect.top() - headerRect.bottom() - 7);
    QTextLayout textLayout(item->desc, font);
    layoutText(textLayout, item->desc, descRect.size());
    textLayout.draw(painter, descRect.topLeft());

    QPen sepLinePen(QColor("#e3e3e3"));
    painter->setPen(sepLinePen);
    painter->drawLine(0, 90, sz.width(), 90);
    painter->restore();
}

QString YTDelegate::timeString(int sec) const
{
    int hour = sec/3600 ;
    sec = sec%3600;
    int minute = sec/60;
    sec = sec%60;
    if(hour >0)
    {
        return QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(minute, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    }
    else
    {
        return QString("%1:%2").arg(minute, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    }
}

QSize YTDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize( option.rect.width(), 91 );
}

void YTDelegate::layoutText(QTextLayout& textLayout, QString text, QSize constraint) const
{
    QTextOption textOption(Qt::AlignJustify);
    textLayout.setTextOption(textOption);
    textLayout.setText(text);
    textLayout.beginLayout();
    int lHeight = 0;
    while(true){
        QTextLine line = textLayout.createLine();
        if(!line.isValid())
            break;
        line.setLineWidth(constraint.width());
        line.setPosition(QPointF(0, lHeight));
        if(lHeight + line.height() > constraint.height())
        {
            QTextLine lastLine = textLayout.lineAt(textLayout.lineCount() - 2);
            QString lastString = text.mid(lastLine.textStart());
            QFontMetrics fm(textLayout.font());
            text.chop(lastString.length());
            text += fm.elidedText(lastString, Qt::ElideRight, constraint.width()-1);
            textLayout.endLayout();
            layoutText(textLayout, text, constraint);
            return;
        }
        lHeight += line.height();
        lHeight += line.leading();
    }
    textLayout.endLayout();
}

#include "moc_ytdelegate.cpp"
