cd src
call mingw32-make distclean

cd ..
del src\translations\smtube_*.qm
del src\object_script.smtube.Release
del src\object_script.smtube.Debug
del src\svn_revision.h
rd src\release
rd src\debug