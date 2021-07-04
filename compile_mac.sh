#! /bin/sh
./get_svn_revision.sh

cd src
lrelease smtube.pro
qmake DEFINES+="NO_DEBUG_ON_CONSOLE NO_USE_PLAYERS"
make -j4
macdeployqt smtube.app
