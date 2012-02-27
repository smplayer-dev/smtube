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

#ifndef YTDELEGATE_H
#define YTDELEGATE_H

#include <QStyledItemDelegate>
#include <QDate>
#include <QTextLayout>


class SingleVideoItem
{
public:
    SingleVideoItem(){
        static QPixmap temp(":/Control/image-placeholder.png");
        pix = temp;
    };
    ~SingleVideoItem(){};
    SingleVideoItem(const SingleVideoItem& copy){
        pix = copy.pix;
        totalTime = copy.totalTime;
        header = copy.header;
        desc = copy.desc;
        date = copy.date;
        views = copy.views;
        pixUrl = copy.pixUrl;
        videoid = copy.videoid;
    }
    QPixmap pix;
    int totalTime;
    QString header;
    QString desc;
    QDate date;
    int views;
    QString pixUrl;
    QString videoid;
};

Q_DECLARE_METATYPE(SingleVideoItem*);

class YTDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:    
    explicit YTDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

private:
    QString timeString(int sec) const;
    void layoutText(QTextLayout& layout, QString text, QSize constraint) const;
    QPixmap playOverlay;
    QPixmap pixBorderOverlay1;
    QPixmap pixBorderOverlay2;
    QPixmap pixBorderOverlayHover;
    QPixmap hoverBackground;
    QPixmap durationBackground;
    inline QColor cr(QColor color, bool white) const { return white ? Qt::white : color ;}


};

#endif // YTDELEGATE_H
