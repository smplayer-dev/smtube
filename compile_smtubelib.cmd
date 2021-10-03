@echo off

cd src
lrelease smtube.pro
qmake DEFINES+="SMTUBE_LIB"
mingw32-make
cd ..
