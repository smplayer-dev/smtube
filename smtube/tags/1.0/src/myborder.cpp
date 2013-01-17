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
