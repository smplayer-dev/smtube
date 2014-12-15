@echo off

:: Some SVN clients can use localized messages (e.g. SlikSVN), force English
set LC_ALL=C
set SVN_REV=

for /f "tokens=2" %%i in ('svn info ^| find "Revision:"') do set SVN_REV=%%i

:: Set to UNKNOWN if no svn or working copy
if "%SVN_REV%"=="" (
  set SVN_REV=UNKNOWN
  echo Unknown SVN revision. SVN missing in PATH or not a working copy.
) else (
  echo SVN Revision: %SVN_REV%
)
echo.

echo #define SVN_REVISION "%SVN_REV%">src\svn_revision.h

:::: Extract version for pkg_version
:: Get value of #define USE_SVN_VERSIONS
for /f "tokens=3" %%j in ('type src\version.cpp ^| find "USE_SVN_VERSIONS"') do set use_svn_revision=%%j

:: Get version from version.cpp
for /f "tokens=3" %%k in ('type src\version.cpp ^| find "#define VERSION"') do set CPP_VERSION=%%k

:: Remove quotes
SET CPP_VERSION=###%CPP_VERSION%###
SET CPP_VERSION=%CPP_VERSION:"###=%
SET CPP_VERSION=%CPP_VERSION:###"=%
SET CPP_VERSION=%CPP_VERSION:###=%

for /f "tokens=2" %%k in ('svn info ^| find "Revision:"') do set SVN_REV=%%k

if [%use_svn_revision%]==[1] (
  echo %CPP_VERSION%.%SVN_REV%>setup\scripts\pkg_version
) else (
  echo %CPP_VERSION%>setup\scripts\pkg_version
)
:::: 

set SVN_REV=
