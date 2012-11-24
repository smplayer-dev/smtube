@echo off

set script_name=%0
set qmake_defs=

:arg_loop2
if [%1]==[] (

	goto compile

) else if [%1]==[pe] (

	set qmake_defs="DEFINES+=PORTABLE_APP"

) else if [%1]==[-h] (

	echo How to use:
	echo.
	echo Add ^`pe^' to compile portable.
	echo.
	echo To compile SMTube non-portable, enter no arguments.
	echo.
	echo ex: %script_name% pe

) else (

	echo configure: error: unrecognized option: `%1'
	echo Try `%script_name% -h' for more information
	goto end

)

shift
goto arg_loop2

:compile

call getrev.cmd

cd src
lrelease smtube.pro
qmake %qmake_defs%
mingw32-make

:end