#! /bin/sh
./get_svn_revision.sh

cd src
qmake DEFINES+="NO_DEBUG_ON_CONSOLE NO_USE_PLAYERS"
make -j4
lrelease smtube.pro
mkdir smtube.app/Contents/MacOS/translations
cp translations/*.qm smtube.app/Contents/MacOS/translations/
macdeployqt smtube.app
