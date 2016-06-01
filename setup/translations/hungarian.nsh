;Language: Hungarian (1038)
;Hungarian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Hungarian" "Magyar"
!else
!insertmacro LANGFILE "Hungarian" = "Magyar" =
!endif

; Startup
${LangFileString} Installer_No_Admin "A program telepítéséhez rendszergazdaként kell bejelentkeznie."

${LangFileString} Win64_Required "A szoftver telepítéséhez 64-bites Windows operációs rendszerre van szükség."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Telepítő"
${LangFileString} WelcomePage_Text "A Telepítő végig fogja vezeti az $(^NameDA) telpítési folyamatán.$\r$\n$\r$\nJavasoljuk, hogy a telepítés megkezdése előtt zárja be az SMTube minden megnyitott példányát. Ez lehetővé teszi az adott program fájlok frissítését anélkül, hogy a számítógépét újra kellene indítania.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (nélkülözhetetlen)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube telepítéséhez szükség van egy ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} telepítésű SMPlayer mappára."
${LangFileString} DirectoryIsNotWritable "A választott telepítési könyvtár nem írható. Újra kell indítani a telepítőt rendszergazdai jogosultsággal.$\r$\n$\r$\nFolytatja a telepítést?"

; Misc
${LangFileString} Info_QtArch_Bypassed " Parancssorból megkerülheti az SMPlayer, architektúra és Qt verzió ellenőrzést."
${LangFileString} Info_Portable_Bypassed "Parancssorból felülbírálható a hordozható változat ellenőrzése."
${LangFileString} Info_Portable_Detected "Az SMPlayer hordozható változatát találtam."
