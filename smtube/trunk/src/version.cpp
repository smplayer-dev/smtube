/*  smtube, a small youtube browser.
    Copyright (C) 2012-2015 Ricardo Villalba <rvm@users.sourceforge.net>

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

#include "version.h"

#define USE_SVN_VERSIONS 1

#define VERSION "15.5.17"

#if USE_SVN_VERSIONS
#include "svn_revision.h"
#endif

#ifdef Q_OS_WIN
#if defined( _WIN64 )
#define SMTWIN_ARCH "(64-bit)"
#elif defined( _WIN32 )
#define SMTWIN_ARCH "(32-bit)"
#endif
#endif

#ifdef Q_WS_AMIGA // zzd10h
#include "svn_revision.h"
#include "amiga_date.h"
#define VERSTAG_AMIGA "\0$VER: SMTube " VERSION " " DATE_AMIGA " " SVN_REVISION
char * ver = (char *)VERSTAG_AMIGA; 
#endif

QString smtubeVersion() {
#if USE_SVN_VERSIONS
#ifdef Q_OS_WIN
    return QString(QString(VERSION) + "+" + QString(SVN_REVISION) + " " + QString(SMTWIN_ARCH));
#else
    return QString(QString(VERSION) + "+" + QString(SVN_REVISION));
#endif
#else
#ifdef Q_OS_WIN
    return QString(QString(VERSION) + " " + QString(SMTWIN_ARCH));
#else
    return QString(VERSION);
#endif
#endif
}
