cd getrev
call mingw32-make distclean
cd ..

cd src
call mingw32-make distclean

cd ..
del src\translations\smtube_*.qm
