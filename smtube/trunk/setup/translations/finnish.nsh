;Language: Finnish (1035)
;Finnish language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Finnish" "Suomi"
!else
!insertmacro LANGFILE "Finnish" = "Suomi" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Sinun on oltava kirjautuneena järjestelmänvalvojana kun asennat tätä ohjelmaa."

${LangFileString} Win64_Required "Tämän ohjelmiston asentaminen edellyttää 64-bittistä käyttöjärjestelmää."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Asennus"
${LangFileString} WelcomePage_Text "Asennusohjelma opastaa sinua $(^NameDA).$\r$\n$\r$\nOn suositeltavaa sulkea kaikki SMTube-osat ennen asennuksen aloittamista. Tämä mahdollistaa ohjelmatiedostojen päivittämisen ilman, että tietokonetta tarvitsee käynnistää uudelleen.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (vaaditaan)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube on asennettava hakemistoon, joka sisältää ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer-asennuksen."
${LangFileString} DirectoryIsNotWritable "Valittuun asennushakemistoon ei voi kirjoittaa. Tämä voi merkitä asennuksen käynnistämistä uudelleen järjestelmänvalvojan oikeuksilla.$\r$\n$\r$\nJatka asennusta?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer-, arkkitehtuuri- ja Qt-version tarkistus ohitetaan komentorivillä."
${LangFileString} Info_Portable_Bypassed "Kannettava tarkistus ohitetaan komentorivillä, pakottaen kannettavaksi."
${LangFileString} Info_Portable_Detected "Löytyi kannettava versio SMPlayer-ohjelmasta."
