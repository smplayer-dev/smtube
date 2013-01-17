#ifndef MYBORDER_H
#define MYBORDER_H

#include <QWidget>
#include <QColor>

class MyBorder : public QWidget
{
    Q_OBJECT
public:
    explicit MyBorder(QWidget *parent = 0);
    void setBGColor(QColor color);
protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // MYBORDER_H
