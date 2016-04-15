@echo off

:: Reset working dir especially when using 'Run as administrator'
@cd /d "%~dp0"

::                                       ::
::        Command-line Parsing           ::
::                                       ::

set start_dir=%~dp0
set all_script=build_all.cmd
set build_pe=
set runsvnup=yes
set runnsiscmd=
set qmake_defs=

:cmdline_parsing
if "%1" == ""               goto build_env_info
if "%1" == "-h"             goto usage
if "%1" == "-all"           goto cfgAll
if "%1" == "-portable"      goto cfgPE
if "%1" == "-makeinst"      goto cfgInst
if "%1" == "-noupdate"      goto cfgUpdate


echo Unknown option: "%1"
echo.
goto usage

:usage
echo Usage: compile_windows2.cmd [-portable] [-makeinst] [-noupdate] [-all]
echo.
echo Configuration:
echo   -h                     display this help and exit
echo.
echo Optional Features:
echo   -portable              Compile portable executables
echo.
echo Miscellaneous Options:
echo   -all                   Build normal and portable exes
echo   -makeinst              Make NSIS Installer afer compiling
echo   -noupdate              Do not update before compiling
echo.
goto end

:cfgAll

echo call clean_windows.cmd>%all_script%
echo call compile_windows.cmd -portable ^&^& call clean_windows.cmd>>%all_script%
echo call compile_windows.cmd -makeinst -noupdate>>%all_script%
echo call :deleteSelf^&exit /b>>%all_script%
echo :deleteSelf>>%all_script%
echo start /b "" cmd ^/c del "%all_script%" ^&exit /b>>%all_script%
%all_script%

:cfgPE

set qmake_defs=%qmake_defs% PORTABLE_APP
set build_pe=yes
set runinstcmd=
shift

goto cmdline_parsing

:cfgInst

set runnsiscmd=yes
shift

goto cmdline_parsing

:cfgUpdate
set runsvnup=
shift

goto cmdline_parsing

::                                       ::
::        Build Environment Info         ::
::                                       ::

:build_env_info

:: GCC Target
for /f "usebackq tokens=2" %%i in (`"gcc -v 2>&1 | find "Target""`) do set gcc_target=%%i
if [%gcc_target%]==[x86_64-w64-mingw32] (
  set X86_64=yes
) else if [%gcc_target%]==[i686-w64-mingw32] (
  set X86_64=no
) else if [%gcc_target%]==[mingw32] (
  set X86_64=no
)

:: Qt locations from QMAKE
for /f "tokens=*" %%i in ('qmake -query QT_INSTALL_PREFIX') do set QT_DIR=%%i
for /f "tokens=*" %%i in ('qmake -query QT_VERSION') do set QT_VER=%%i
set QT_DIR=%QT_DIR:/=\%

if [%runsvnup%]==[yes] (
  svn up
  echo.
)

set SMTUBE_DIR=%start_dir%
:: Does string have a trailing slash? if so remove it 
if %SMTUBE_DIR:~-1%==\ set SMTUBE_DIR=%SMTUBE_DIR:~0,-1%

if %QT_VER% lss 5.0.0 (

  echo File "%QT_DIR%\bin\QtWebKit4.dll">"%SMTUBE_DIR%\setup\qt_files.nsh"

) else if %QT_VER% geq 5.0.0 (

  echo File "%QT_DIR%\bin\Qt5WebKit.dll">"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Sql.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Qml.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Quick.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Positioning.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Multimedia.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5Sensors.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5WebChannel.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5WebKitWidgets.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5OpenGL.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5PrintSupport.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\Qt5MultimediaWidgets.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\icuin*.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\icuuc*.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
  echo File "%QT_DIR%\bin\icudt*.dll">>"%SMTUBE_DIR%\setup\qt_files.nsh"
)

:compile

call getrev.cmd

cd src
lrelease smtube.pro
qmake "DEFINES += %qmake_defs%"
mingw32-make

:: Installation
if not ERRORLEVEL 1 (
  if [%runnsiscmd%]==[yes] (
    mkdir "%SMTUBE_DIR%\setup\output"
    if [%X86_64%]==[yes] (
      call "%SMTUBE_DIR%\setup\scripts\make_pkgs.cmd" 2
    ) else (
      call "%SMTUBE_DIR%\setup\scripts\make_pkgs.cmd" 1
    )
  )
)

if not ERRORLEVEL 1 (
  if [%build_pe%]==[yes] (
    mkdir "%SMTUBE_DIR%\setup\portable"

    if [%X86_64%]==[yes] (
      copy /y release\smtube.exe "%SMTUBE_DIR%\setup\portable\smtube-portable64.exe"
    ) else ( 
      copy /y release\smtube.exe "%SMTUBE_DIR%\setup\portable\smtube-portable.exe"
    )
  )
)

:: Return to starting directory
cd /d "%start_dir%"

:end
