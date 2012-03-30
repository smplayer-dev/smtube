call getrev.cmd

cd src
lrelease smtube.pro
qmake DEFINES+=PORTABLE_APP
mingw32-make
