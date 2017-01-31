;Language: Catalan (1027)
;Catalan language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Catalan" "Català"
!else
!insertmacro LANGFILE "Catalan" = "Català" "Catala"
!endif

; Startup
${LangFileString} Installer_No_Admin "Heu d'iniciar la sessió com a administrador quan instal·leu aquest programa."

${LangFileString} Win64_Required "Es requereix un sistema operatiu Windows de 64 bits per instal·lar aquest programari."

; Welcome page
${LangFileString} WelcomePage_Title "Preparació $(^NameDA)"
${LangFileString} WelcomePage_Text "La preparació us guiarà a través de la instal·lació de $(^NameDA).$\r$\n$\r$\nEs recomana que tanqueu totes les instàncies de SMTube abans de començar la preparació. Això farà que sigui possible l'actualització dels de programa pertinents sense haver de reiniciar el vostre ordinador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (requerit)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube ha de ser instal·lat en un directori que contingui una instal·lació ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} de SMPlayer."
${LangFileString} DirectoryIsNotWritable "El directori d'instal·lació escollit no és modificable i pot requerir reiniciar el programa d'instal·lació amb privilegis d'administrador.$\r$\n$\r$\nVoleu continuar amb la instal·lació?"

; Misc
${LangFileString} Info_QtArch_Bypassed "La comprovació de SMPlayer, de l'arquitectura i de la versió de Qt passades amb la línia d'ordres."
${LangFileString} Info_Portable_Bypassed "La comprovació de la portabilitat ha estat anul·lada per la línia d'ordres, es força la portabilitat."
${LangFileString} Info_Portable_Detected "S'ha trobat una versió portable de SMPlayer."
