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
${LangFileString} DirectoryIsNotWritable "O cartafol da instalación seleccionada non é escribíbel e pode necesitar o reinicio do instalador con privilexios de administrador.$\r$\n$\r$\nDesexa continuar coa instalación?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, a comprobación da arquitectura e versión Qt foron anuladas por liña de ordes."
${LangFileString} Info_Portable_Bypassed "A comprobación da versión portátil foi anulada por liña de ordes, forzando portátil."
${LangFileString} Info_Portable_Detected "Atopouse unha versión portátil do SMPlayer."
