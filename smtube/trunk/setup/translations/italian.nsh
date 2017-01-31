;Language: Italian (1040)
;Italian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Italian" "Italiano"
!else
!insertmacro LANGFILE "Italian" = "Italiano" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Devi essere loggato come Amministratore quando installi questo programma."

${LangFileString} Win64_Required "Un SO Windows a 64-bit è richiesto per poter installare questo programma."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "Il Setup ti guiderà attraverso l'installazione di $(^NameDA).$\r$\n$\r$\nE' consigliato chiudere tutte le istanze di SMTube prima di proseguire. Questo renderà possibile aggiornare ogni files rilevanti senza dover riavviare il computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (richiesto)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube deve essere installato in una cartella contenente ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer."
${LangFileString} DirectoryIsNotWritable "La directory scelta per l'installazione non è scrivibile e potrebbe essere richiesto di riavviare l'installer con privilegi di amministratore.$\r$\n$\r$\nProcedere con l'installazione?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Controllo versione SMPlayer, architettura e Qt bypassato dalla riga di comando."
${LangFileString} Info_Portable_Bypassed "Portable check bypassato dalla linea di comando, forzatura portatile."
${LangFileString} Info_Portable_Detected "Trovata versione portable di SMPlayer."
