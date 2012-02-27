#ifndef MYICON_H
#define MYICON_H

#include <QObject>
#include <QPixmap>

class MyIcon
{
public:
    enum Mode
    {
        Normal,
        MouseOver,
        MouseDown,
        Disabled
    };

    enum State
    {
        Off,
        On
    };

    explicit MyIcon();
    QSize size(Mode mode, State state = Off );
    void setPixmap(QPixmap pix, Mode mode, State state = Off);
    QPixmap pixmap(Mode mode, State state =Off) const;

private:
    QPixmap normalPixOff;
    QPixmap mouseOverPixOff;
    QPixmap mouseDownPixOff;
    QPixmap disabledPixOff;
    QPixmap normalPixOn;
    QPixmap mouseOverPixOn;
    QPixmap mouseDownPixOn;
    QPixmap disabledPixOn;
    QPixmap actualPixmap(Mode mode, State state =Off) const;

signals:

public slots:

};

#endif // MYICON_H
