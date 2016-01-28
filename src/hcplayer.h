/*  smtube, a small youtube browser.
    Copyright (C) 2012-2016 Ricardo Villalba <rvm@users.sourceforge.net>

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

#ifndef HCPLAYERS_H
#define HCPLAYERS_H

#ifdef Q_OS_WIN

#define HCPLAYER_NAME "SMPlayer"
#define HCPLAYER_EXECUTABLE qApp->applicationDirPath() + "/smplayer.exe"
#define HCPLAYER_ARGUMENTS "%u"
#define HCPLAYER_STREAMINGSITES true

#else

#ifdef Q_OS_AMIGA //zzd10h

#define HCPLAYER_NAME "MPlayer"
#define HCPLAYER_EXECUTABLE "Amiga/PlayVideo.rexx"
#define HCPLAYER_ARGUMENTS "%u"
#define HCPLAYER_STREAMINGSITES false

#else

#define HCPLAYER_NAME "SMPlayer"
#define HCPLAYER_EXECUTABLE "smplayer"
#define HCPLAYER_ARGUMENTS "%u"
#define HCPLAYER_STREAMINGSITES true

#endif

/*
#define HCPLAYER_NAME "MPlayer"
#define HCPLAYER_EXECUTABLE "mplayer"
#define HCPLAYER_ARGUMENTS "%u -title %t"
#define HCPLAYER_STREAMINGSITES false
*/

#endif

#endif // HCPLAYERS_H

