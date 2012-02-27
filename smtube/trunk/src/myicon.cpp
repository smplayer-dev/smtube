#include "myicon.h"

MyIcon::MyIcon()
{
}


void MyIcon::setPixmap(QPixmap pix, Mode mode, State state )
{
    switch(mode)
    {
    case Normal:
        if(state == Off) normalPixOff = pix;
        else normalPixOn = pix; break;
    case MouseOver:
        if(state == Off) mouseOverPixOff = pix;
        else mouseOverPixOn = pix; break;
    case MouseDown:
        if(state == Off) mouseDownPixOff = pix;
        else mouseDownPixOn = pix; break;
    case Disabled:
        if(state == Off) disabledPixOff = pix;
        else disabledPixOn = pix;break;
    }
}

QSize MyIcon::size(Mode mode, State state)
{
    return pixmap(mode, state).size();
}

QPixmap MyIcon::pixmap(Mode mode, State state) const
{
    QPixmap p = actualPixmap(mode, state);
    if(p.isNull())
    {
        if(state == Off) return normalPixOff;
        else return normalPixOn;
    }
    else return p;
}

QPixmap MyIcon::actualPixmap(Mode mode, State state ) const
{
    switch(mode)
    {
    case Normal:
        if(state == Off) return normalPixOff;
        else return normalPixOn;
    case MouseOver:
        if(state == Off) return mouseOverPixOff;
        else return mouseOverPixOn;
    case MouseDown:
        if(state == Off) return mouseDownPixOff;
        else return mouseDownPixOn;
    case Disabled:
        if(state == Off) return disabledPixOff;
        else return disabledPixOn;
    }
    return QPixmap();
}
