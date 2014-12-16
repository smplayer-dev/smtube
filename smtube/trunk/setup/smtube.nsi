;Installer script for win32/win64 SMTube

!ifndef VER_MAJOR | VER_MINOR | VER_BUILD
  !error "Version information not defined (or incomplete). You must define: VER_MAJOR, VER_MINOR, VER_BUILD."
!endif

;--------------------------------
;Compressor

  SetCompressor /SOLID lzma
  SetCompressorDictSize 32

;--------------------------------
;Additional plugin folders

  !addplugindir .
  !addincludedir .

;--------------------------------
;Defines

!ifdef VER_REVISION
  !define SMTUBE_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
  !define SMTUBE_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.${VER_REVISION}"
!else ifndef VER_REVISION
  !define SMTUBE_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
  !define SMTUBE_PRODUCT_VERSION "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}.0"
!endif

  !define SMPLAYER_REG_KEY "Software\SMPlayer"
  
  !define SMPLAYER_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SMPlayer"

;--------------------------------
;General

  ;Name and file
  Name "SMTube ${SMTUBE_VERSION}"
  BrandingText "SMTube for Windows v${SMTUBE_VERSION}"
!ifdef WIN64
    OutFile "output\smtube-${SMTUBE_VERSION}-x64.exe"
!else
    OutFile "output\smtube-${SMTUBE_VERSION}-win32.exe"
!endif

  ;Version tab properties
  VIProductVersion "${SMTUBE_PRODUCT_VERSION}"
  VIAddVersionKey "ProductName" "SMTube"
  VIAddVersionKey "ProductVersion" "${SMTUBE_VERSION}"
  VIAddVersionKey "FileVersion" "${SMTUBE_VERSION}"
  VIAddVersionKey "LegalCopyright" ""
!ifdef WIN64
  VIAddVersionKey "FileDescription" "SMTube Installer (64-bit)"
!else
  VIAddVersionKey "FileDescription" "SMTube Installer (32-bit)"
!endif

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "${SMPLAYER_REG_KEY}" "Path"

  ;Vista+ XML manifest, does not affect older OSes
  RequestExecutionLevel admin

  ShowInstDetails show
  ShowUnInstDetails show

;--------------------------------
;Variables

  Var InstType_Is_Portable
  Var SMPlayer_StartMenuFolder

;--------------------------------
;Interface Settings

  ;Installer/Uninstaller icons
  !define MUI_ICON "smtube-orange-installer.ico"
  !define MUI_UNICON "smtube-orange-uninstaller.ico"

  ;Misc
  !define MUI_WELCOMEFINISHPAGE_BITMAP "smtube-orange-wizard.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "smtube-orange-wizard-un.bmp"
  !define MUI_ABORTWARNING

  ;License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS

  ;Components page
  !define MUI_COMPONENTSPAGE_SMALLDESC

  ;Directory page
  !define MUI_DIRECTORYPAGE_TEXT_TOP "$(^DirText)$\r$\n$\r$\nSMTube must be installed to the same directory as SMPlayer."

  ;Finish page
  !define MUI_FINISHPAGE_LINK "http://smplayer.sourceforge.net"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://smplayer.sourceforge.net"
  !define MUI_FINISHPAGE_NOREBOOTSUPPORT
  !define MUI_FINISHPAGE_RUN $INSTDIR\smtube.exe
  !define MUI_FINISHPAGE_RUN_NOTCHECKED

  ;Language Selection Dialog Settings
  !define MUI_LANGDLL_REGISTRY_ROOT HKLM
  !define MUI_LANGDLL_REGISTRY_KEY "${SMPLAYER_UNINST_KEY}"
  !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

  ;Memento Settings
  !define MEMENTO_REGISTRY_ROOT HKLM
  !define MEMENTO_REGISTRY_KEY "${SMPLAYER_REG_KEY}"

  ;Start Menu Settings
  !define MUI_STARTMENUPAGE_DEFAULTFOLDER "SMPlayer"
  !define MUI_STARTMENUPAGE_NODISABLE
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "${SMPLAYER_UNINST_KEY}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenu"

;--------------------------------
;Include Modern UI and functions

  !include MUI2.nsh
  !include FileFunc.nsh
  !include Memento.nsh
  !include nsDialogs.nsh
  !include Sections.nsh
  !include WinVer.nsh
  !include WordFunc.nsh
  !include x64.nsh

;--------------------------------
;Pages

  ;Install pages
  #Welcome
  !insertmacro MUI_PAGE_WELCOME

  #License
  !insertmacro MUI_PAGE_LICENSE "license.txt"

  #Components
  !insertmacro MUI_PAGE_COMPONENTS

  #Install Directory
  !insertmacro MUI_PAGE_DIRECTORY

  #Start Menu
  !define MUI_PAGE_CUSTOMFUNCTION_PRE PageStartMenuPre
  !insertmacro MUI_PAGE_STARTMENU "SMP_SMenu" $SMPlayer_StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Basque"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Croatian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Malay"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Ukrainian"

;Custom translations for setup
/*
  !insertmacro LANGFILE_INCLUDE "translations\english.nsh"
  /*
  !insertmacro LANGFILE_INCLUDE "translations\albanian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\arabic.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\basque.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\bulgarian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\catalan.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\croatian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\czech.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\danish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\dutch.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\finnish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\french.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\german.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hebrew.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\hungarian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\italian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\japanese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\korean.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\malay.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\norwegian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\polish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\portuguese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\portuguesebrazil.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\russian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\serbian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\simpchinese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovak.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\slovenian.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\spanish.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\thai.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\tradchinese.nsh"
  !insertmacro LANGFILE_INCLUDE "translations\ukrainian.nsh"
*/
;--------------------------------
;Reserve Files

  ;These files should be inserted before other files in the data block
  ;Keep these lines before any File command
  ;Only for solid compression (by default, solid compression is enabled for BZIP2 and LZMA)

  !insertmacro MUI_RESERVEFILE_LANGDLL
  ReserveFile "${NSISDIR}\Plugins\UserInfo.dll"

;--------------------------------
;Installer Sections

;--------------------------------
;Main SMTube files
Section "SMTube (required)" SecSMTube

  SectionIn RO

  SetOutPath "$INSTDIR"
  ${If} $InstType_Is_Portable == 1
!ifdef WIN64
    File /oname=smtube.exe "portable\smtube-portable64.exe"
!else
    File /oname=smtube.exe "portable\smtube-portable.exe"
!endif
  ${Else}
    File "..\src\release\smtube.exe"
  ${EndIf}

  SetOutPath "$INSTDIR\translations"
  File "..\src\translations\*.qm"

  SetOutPath "$INSTDIR\docs\smtube"
  File "..\*.txt"

  SetShellVarContext all
  SetOutPath "$INSTDIR"
  !insertmacro MUI_STARTMENU_WRITE_BEGIN SMP_SMenu
    CreateShortCut "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMTube.lnk" "$INSTDIR\smtube.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Installer functions

Function .onInit

  ;Gets start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMPlayer_StartMenuFolder

FunctionEnd

Function .onVerifyInstDir

  ${If} ${FileExists} "$INSTDIR\smplayer_orig.ini"
    StrCpy $InstType_Is_Portable 1
  ${Else}
    StrCpy $InstType_Is_Portable 0
  ${EndIf}

  IfFileExists "$INSTDIR\smplayer.exe" +2
    Abort

FunctionEnd

Function PageStartMenuPre

  ${If} $InstType_Is_Portable == 1
    Abort
  ${EndIf}

FunctionEnd
