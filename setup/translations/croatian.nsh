;Language: Croatian (1050)
;Croatian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Croatian" "Hrvatski"
!else
!insertmacro LANGFILE "Croatian" = "Hrvatski" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Morate biti prijavljeni kao administrator kada instaliravate ovaj program."

${LangFileString} Win64_Required "64-bitni Windows operativni sustav je potreban za instalaciju ovog softvera."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) program instalacije"
${LangFileString} WelcomePage_Text "Program instalacije će vas voditi kroz instalaciju $(^NameDA).$\r$\n$\r$\nPreporučljivo je da zatvorite sve primjerke SMTuba prije pokretanja instalacije. To će omogućiti ažuriranje bitnih datoteka programa bez potrebe za ponovnim pokretanjem vašeg računala.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (potrebno)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube mora biti instaliran u direktorij koji sadrži ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} instalaciju SMPlayera."
${LangFileString} DirectoryIsNotWritable "Odabrani direktorij instalacije nema dozvolu zapisivanja i zahtjeva ponovno pokretanje instalacijskog programa s administratorskim ovlastima.$\r$\n$\r$\nNastavi s instalacijom?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Provjera SMPlayer arhitekture i Qt inačice zaobiđena je u naredbenom redku."
${LangFileString} Info_Portable_Bypassed "Provjera je li aplikacija samostojeća zaobiđena je u naredbenom redku, prisilno je samostojeća."
${LangFileString} Info_Portable_Detected "Dostupna je samostojeća inačica SMPlayera."
