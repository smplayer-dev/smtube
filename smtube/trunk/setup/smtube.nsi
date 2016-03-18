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

!ifdef WIN64
  !define SMTUBE_INST_ARCH "64-bit"
  !define SMTUBE_PE_EXE "smtube-portable64.exe"
!else
  !define SMTUBE_INST_ARCH "32-bit"
  !define SMTUBE_PE_EXE "smtube-portable.exe"
!endif

!ifdef QT5
  !define SMTUBE_QT_VER "Qt5"
!else
  !define SMTUBE_QT_VER "Qt4"
!endif

;--------------------------------
;General

  ;Name and file
  Name "SMTube ${SMTUBE_VERSION}"
  BrandingText "SMTube for Windows v${SMTUBE_VERSION}"
!ifdef WIN64
  !ifdef QT5
  OutFile "output\Qt5\smtube-${SMTUBE_VERSION}-x64-qt5.exe"
  !else
  OutFile "output\smtube-${SMTUBE_VERSION}-x64.exe"
  !endif
!else
  !ifdef QT5
  OutFile "output\Qt5\smtube-${SMTUBE_VERSION}-win32-qt5.exe"
  !else
  OutFile "output\smtube-${SMTUBE_VERSION}-win32.exe"
  !endif
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
  RequestExecutionLevel highest

  ShowInstDetails show
  ShowUnInstDetails show

;--------------------------------
;Variables

  Var InstType_Is_Portable
!ifdef WIN64
  Var SMPlayer_Path
!endif
  Var SMPlayer_StartMenuFolder

  Var SMPlayer_FileDescription
  Var SMPlayer_ProductName

  Var SkippedChecks

  Var OverrideAsPortable

;--------------------------------
;Interface Settings

  ;Installer/Uninstaller icons
  !define MUI_ICON "smtube-orange-installer.ico"
  !define MUI_UNICON "smtube-orange-uninstaller.ico"

  ;Misc
  !define MUI_WELCOMEFINISHPAGE_BITMAP "smtube-orange-wizard.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "smtube-orange-wizard-un.bmp"
  !define MUI_ABORTWARNING

  ;Welcome page
  !define MUI_WELCOMEPAGE_TITLE $(WelcomePage_Title)
  !define MUI_WELCOMEPAGE_TEXT $(WelcomePage_Text)

  ;License page
  !define MUI_LICENSEPAGE_RADIOBUTTONS

  ;Components page
  !define MUI_COMPONENTSPAGE_SMALLDESC

  ;Directory page
  !define MUI_DIRECTORYPAGE_TEXT_TOP $(DirectoryPage_Text)

  ;Finish page
  !define MUI_FINISHPAGE_LINK "http://www.smplayer.info"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://www.smplayer.info"
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
  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE PageComponentsLeave
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

  !insertmacro LANGFILE_INCLUDE "translations\english.nsh"
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
Section $(Section_SMTube) SecSMTube

  SectionIn RO

  ${If} $SkippedChecks == 1
    DetailPrint $(Info_QtArch_Bypassed)
  ${EndIf}

  ${If} $OverrideAsPortable == 1
    DetailPrint $(Info_Portable_Bypassed)
  ${EndIf}

  SetOutPath "$INSTDIR"
  ${If} $InstType_Is_Portable == 1
    DetailPrint $(Info_Portable_Detected)
    File /oname=smtube.exe "portable\${SMTUBE_PE_EXE}"
  ${Else}
    File "..\src\release\smtube.exe"
  ${EndIf}

  ; Generated by compile_windows.cmd
  !include qt_files.nsh

  SetOutPath "$INSTDIR\translations"
  File "..\src\translations\*.qm"

  SetOutPath "$INSTDIR\docs\smtube"
  File "..\*.txt"

  ${IfNot} $InstType_Is_Portable == 1
    SetShellVarContext all
    SetOutPath "$INSTDIR"

    !insertmacro MUI_STARTMENU_WRITE_BEGIN SMP_SMenu
      CreateShortCut "$SMPROGRAMS\$SMPlayer_StartMenuFolder\SMTube.lnk" "$INSTDIR\smtube.exe"
    !insertmacro MUI_STARTMENU_WRITE_END
  ${EndIf}

  SetAutoClose false

SectionEnd

;--------------------------------
;Macros

; StrContains
; This function does a case sensitive searches for an occurrence of a substring in a string. 
; It returns the substring if it is found. 
; Otherwise it returns null(""). 
; Written by kenglish_hi
; Adapted from StrReplace written by dandaman32

Var STR_HAYSTACK
Var STR_NEEDLE
Var STR_CONTAINS_VAR_1
Var STR_CONTAINS_VAR_2
Var STR_CONTAINS_VAR_3
Var STR_CONTAINS_VAR_4
Var STR_RETURN_VAR
 
Function StrContains
  Exch $STR_NEEDLE
  Exch 1
  Exch $STR_HAYSTACK
  ; Uncomment to debug
  ;MessageBox MB_OK 'STR_NEEDLE = $STR_NEEDLE STR_HAYSTACK = $STR_HAYSTACK '
    StrCpy $STR_RETURN_VAR ""
    StrCpy $STR_CONTAINS_VAR_1 -1
    StrLen $STR_CONTAINS_VAR_2 $STR_NEEDLE
    StrLen $STR_CONTAINS_VAR_4 $STR_HAYSTACK
    loop:
      IntOp $STR_CONTAINS_VAR_1 $STR_CONTAINS_VAR_1 + 1
      StrCpy $STR_CONTAINS_VAR_3 $STR_HAYSTACK $STR_CONTAINS_VAR_2 $STR_CONTAINS_VAR_1
      StrCmp $STR_CONTAINS_VAR_3 $STR_NEEDLE found
      StrCmp $STR_CONTAINS_VAR_1 $STR_CONTAINS_VAR_4 done
      Goto loop
    found:
      StrCpy $STR_RETURN_VAR $STR_NEEDLE
      Goto done
    done:
   Pop $STR_NEEDLE ;Prevent "invalid opcode" errors and keep the
   Exch $STR_RETURN_VAR  
FunctionEnd
 
!macro _StrContainsConstructor OUT NEEDLE HAYSTACK
  Push `${HAYSTACK}`
  Push `${NEEDLE}`
  Call StrContains
  Pop `${OUT}`
!macroend
 
!define StrContains '!insertmacro "_StrContainsConstructor"'

;--------------------------------
;Required functions

!insertmacro GetParameters
!insertmacro GetOptions

;--------------------------------
;Installer functions

Function .onInit

!ifdef WIN64
  ${IfNot} ${RunningX64}
    MessageBox MB_OK|MB_ICONSTOP $(Win64_Required)
    Abort
  ${EndIf}

  SetRegView 64

  ;Workaround for InstallDirRegKey on 64-bit
  ReadRegStr $SMPlayer_Path HKLM "${SMPLAYER_REG_KEY}" "Path"
  StrCpy $INSTDIR $SMPlayer_Path
!endif

  ;Setup language selection
  !insertmacro MUI_LANGDLL_DISPLAY

  ;Gets start menu folder name
  !insertmacro MUI_STARTMENU_GETFOLDER "SMP_SMenu" $SMPlayer_StartMenuFolder

  SetShellVarContext all

FunctionEnd

Function .onVerifyInstDir

  ${GetParameters} $R0

  ${GetOptions} $R0 "/SKIPCHECKS" $R1
  ${Unless} ${Errors}
    StrCpy $SkippedChecks 1
    Goto skipchecks
  ${EndUnless}

  IfFileExists "$INSTDIR\smplayer.exe" +2
    Abort

!ifdef QT5
  IfFileExists "$INSTDIR\Qt5Core.dll" +2
    Abort
!else
  IfFileExists "$INSTDIR\QtCore4.dll" +2
    Abort
!endif

!ifdef USE_MOREINFO
  MoreInfo::GetProductName "$INSTDIR\smplayer.exe"
  Pop $SMPlayer_ProductName
!else
  Push ProductName
  Push "$INSTDIR\smplayer.exe"
  Call GetFileVerFirstNamedLangEntryOnWindowsNT
  Pop $SMPlayer_ProductName
!endif

  ${StrContains} $0 "(${SMTUBE_INST_ARCH})" "$SMPlayer_ProductName"
  StrCmp $0 "" 0 +2
    Abort

  skipchecks:

FunctionEnd

Function PageComponentsLeave

  ClearErrors

  FileOpen $0 "$INSTDIR\write_test" "w"
  FileWriteByte $0 "0"

  IfErrors NotWritable Writable
  NotWritable:
    MessageBox MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2 "The chosen installation directory is not writable and may require administrator privileges.$\r$\n$\r$\nProceed with installation?" /SD IDNO IDYES +2
    Abort
  Writable:
    FileClose $0
    Delete "$INSTDIR\write_test"

!ifdef USE_MOREINFO
  MoreInfo::GetFileDescription "$INSTDIR\smplayer.exe"
  Pop $SMPlayer_FileDescription
!else
  Push FileDescription
  Push "$INSTDIR\smplayer.exe"
  Call GetFileVerFirstNamedLangEntryOnWindowsNT
  Pop $SMPlayer_FileDescription
!endif

  ${GetParameters} $R0

  ${GetOptionsS} $R0 "/P" $R1
  ${Unless} ${Errors}
    StrCpy $InstType_Is_Portable 1
    StrCpy $OverrideAsPortable 1
  ${Else}
    ${StrContains} $1 "Portable" "$SMPlayer_FileDescription"
    StrCmp $1 "" 0 +3
      StrCpy $InstType_Is_Portable 0
      Goto +2
    StrCpy $InstType_Is_Portable 1
  ${EndUnless}

  ; For troubleshooting
  ; MessageBox MB_OK "$SMPlayer_ProductName"
  ; MessageBox MB_OK "$SMPlayer_FileDescription - Portable? - $InstType_Is_Portable - Override: $OverrideAsPortable"

/*
  ; Simple way to figure out portable/non-portable w/o MoreInfo
  ${If} ${FileExists} "$INSTDIR\smplayer_orig.ini"
    StrCpy $InstType_Is_Portable 1
  ${Else}
    StrCpy $InstType_Is_Portable 0
  ${EndIf}
*/

FunctionEnd

Function PageStartMenuPre

  ${If} $InstType_Is_Portable == 1
    Abort
  ${EndIf}

FunctionEnd

Function GetFileVerFirstNamedLangEntryOnWindowsNT

  ; Written by Anders (anders_k)
  ; http://forums.winamp.com/archive/index.php/t-271175.html
  System::Store S
  pop $3
  pop $4
  push "" ;failed ret

  System::Call 'version::GetFileVersionInfoSizeW(w"$3",i.r2)i.r0'

  ${If} $0 <> 0
    System::Alloc $0
    System::Call 'version::GetFileVersionInfoW(w"$3",ir2,ir0,isr1)i.r0 ? e'
    pop $2
    ${If} $0 <> 0
    ${AndIf} $2 = 0 ;a user comment on MSDN said you should check GLE to avoid crash
      System::Call 'version::VerQueryValueW(i r1,w "\VarFileInfo\Translation",*i0r2,*i0)i.r0'
      ${If} $0 <> 0
        System::Call '*$2(&i2.r2,&i2.r3)'
        IntFmt $2 %04x $2
        IntFmt $3 %04x $3
        System::Call 'version::VerQueryValueW(i r1,w "\StringFileInfo\$2$3\$4",*i0r2,*i0r3)i.r0'
        ${If} $0 <> 0
          pop $0
          System::Call *$2(&w$3.s)
        ${EndIf}
      ${EndIf}
    ${EndIf}
    System::Free $1
  ${EndIf}

  System::Store L

FunctionEnd
