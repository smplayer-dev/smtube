;Language: Galician (1110)
;Galician language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Galician" "Galego"
!else
!insertmacro LANGFILE "Galician" = "Galego" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Debe acceder como administrador para instalar este programa."

${LangFileString} Win64_Required "Precísase un sistema operativo Windows de 64-bits para instalar esta aplicación."

; Welcome page
${LangFileString} WelcomePage_Title "Configurar $(^NameDA) "
${LangFileString} WelcomePage_Text "O instalador guiarate na instalación de $(^NameDA).$\r$\n$\r$\n Recoméndase que se pechen todas as instancias de SMTube antes de comezar coa instalación. Así pódese actualizar os ficheiros da aplicación que son de relevancia sen logo precisar de reiniciar o ordenador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (requirido)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube debe instalarse nun cartafol que conteña unha instalación ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} do SMPlayer."
${LangFileString} DirectoryIsNotWritable "The chosen installation directory is not writable and may require restarting the installer with administrator privileges.$\r$\n$\r$\nProceed with installation?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, architecture and Qt version check bypassed by command-line."
${LangFileString} Info_Portable_Bypassed "Portable check overriden by command-line, forcing portable."
${LangFileString} Info_Portable_Detected "Atopouse unha versión portátil do SMPlayer."
