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
