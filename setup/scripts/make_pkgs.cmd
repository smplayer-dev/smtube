@echo off
if exist "turnon_echo" (
  @echo on
)

:: Reset working dir especially when using 'Run as administrator'
@cd /d "%~dp0"

rem echo This batch file can help you to create a packages for SMTube.
rem echo.
rem echo 1 - NSIS
rem echo 2 - NSIS [64-bit]
rem echo.

:: Relative directory of all the source files to this script
set TOP_LEVEL_DIR=..

set OUTPUT_DIR=%TOP_LEVEL_DIR%\output

:: Reset in case ran again in same command prompt instance
set ALL_PKG_VER=
set VER_MAJOR=
set VER_MINOR=
set VER_BUILD=
set VER_REVISION=
set VER_REV_CMD=
set MAKENSIS_EXE_PATH=

:: NSIS path
if exist "nsis_path" (
  for /f "tokens=*" %%y in ('type nsis_path') do if exist %%y set MAKENSIS_EXE_PATH="%%y"
)

if not defined MAKENSIS_EXE_PATH (
  for %%x in ("%PROGRAMFILES(X86)%\NSIS\Unicode\makensis.exe" "%PROGRAMFILES%\NSIS\Unicode\makensis.exe") do if exist %%x set MAKENSIS_EXE_PATH=%%x
)

if not defined MAKENSIS_EXE_PATH (
  echo Warning: Unable to locate NSIS in the default path, create the file ^'nsis_path^' with the full correct path
  echo to makensis.exe or the existing ^'nsis_path^' may be incorrect.
  echo.
)

:cmdline_parsing
if "%1" == ""  goto end
if "%1" == "-1" (
  set USER_CHOICE=1
  goto pkgver
)
if "%1" == "-2" (
  set USER_CHOICE=2
  goto pkgver
)

echo Unknown option: "%1"
echo.

goto end

:pkgver
if exist "pkg_version" (
  for /f "tokens=*" %%i in ('type pkg_version') do set ALL_PKG_VER=%%i
  goto parse_version
)

goto end

:parse_version
for /f "tokens=1 delims=." %%j in ("%ALL_PKG_VER%")  do set VER_MAJOR=%%j
for /f "tokens=2 delims=." %%k in ("%ALL_PKG_VER%")  do set VER_MINOR=%%k
for /f "tokens=3 delims=." %%l in ("%ALL_PKG_VER%")  do set VER_BUILD=%%l
for /f "tokens=4 delims=." %%m in ("%ALL_PKG_VER%")  do set VER_REVISION=%%m

if [%VER_MAJOR%]==[] (
  echo Major Version # must be specified [#.x.x]
  echo.
  goto end
)

if [%VER_MINOR%]==[] (
  echo Minor Version # must be specified [x.#.x]
  echo.
  goto end
)

if [%VER_BUILD%]==[] (
  echo Build Version # must be specified [x.x.#]
  echo.
  goto end
)

if [%VER_REVISION%]==[] (
  set VER_REV_CMD=
) else (
  set VER_REV_CMD= /DVER_REVISION=%VER_REVISION%
)

if "%USER_CHOICE%" == "1"  goto nsispkg
if "%USER_CHOICE%" == "2"  goto nsispkg64
:: Should not happen
goto end

:nsispkg

echo --- SMTube NSIS Package [32-bit] ---
echo.

%MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% %TOP_LEVEL_DIR%\smtube.nsi

goto end

:nsispkg64
echo --- SMTube NSIS Package [64-bit] ---
echo.

%MAKENSIS_EXE_PATH% /V3 /DVER_MAJOR=%VER_MAJOR% /DVER_MINOR=%VER_MINOR% /DVER_BUILD=%VER_BUILD%%VER_REV_CMD% /DWIN64 %TOP_LEVEL_DIR%\smtube.nsi

goto end

:end
