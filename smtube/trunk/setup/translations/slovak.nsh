;Language: Slovak (1051)
;Slovak language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Slovak" "Slovenčina"
!else
!insertmacro LANGFILE "Slovak" = "Slovenčina" "Slovencina"
!endif

; Startup
${LangFileString} Installer_No_Admin "Pri inštalovaní tohoto programu musíte byť prihlásení ako administrátor."

${LangFileString} Win64_Required "Pre inštaláciu tohoto softvéru je požadovaný 64-bitový operačný systém Windows."

; Welcome page
${LangFileString} WelcomePage_Title "Nastavenie $(^NameDA)"
${LangFileString} WelcomePage_Text "Inštalátor vás prevedie inštaláciou $(^NameDA).$\r$\n$\r$\nPredtým, než spustíte inštalátor, sa odporúča zavrieť všetky spustené inštancie SMTube. Toto umožní aktualizovať príslušné súbory programu bez nutnosti reštartovať váš počítač.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (vyžadované)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube musí byť inštalovaný do adresára obsahujúceho ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} inštaláciu SMPlayer-u."
${LangFileString} DirectoryIsNotWritable "Do zvoleného inštalačného adresára nemožno zapisovať a môže byť vyžadované reštartovanie inštalátora s administrátorskými oprávneniami.$\r$\n$\r$\nPokračovať v inštalácii?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Kontrola SMPlayer-u, architektúry a verzie Qt obídená cez príkazový riadok."
${LangFileString} Info_Portable_Bypassed "Kontrola prenosnej verzie obídená cez príkazový riadok, požadujúc prenosnú verziu."
${LangFileString} Info_Portable_Detected "Nájdená prenosná verzia SMPlayer-u."
