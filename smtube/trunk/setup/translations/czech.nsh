;Language: Czech (1029)
;Czech language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Czech" "Čeština"
!else
!insertmacro LANGFILE "Czech" = "Čeština" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Instalaci tohoto programu je potřeba provést s právy administrátora."

${LangFileString} Win64_Required "Pro instalaci tohoto software je zapotřebí mít 64-bitový oprační systém Windows."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Nastavení"
${LangFileString} WelcomePage_Text "Nastavení vás provede instalací $(^NameDA).$\r$\n$\r$\nJe doporučeno zavřít všechny běžící instance SMPlayeru před zahájením nastavení. Toto umožní aktualizovat příslušné programové soubory, aniž byste museli počítač restartovat.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (vyžadován)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube musí být nainstalován do adresáře obsahujícího ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} instalaci SMPlayeru."
${LangFileString} DirectoryIsNotWritable "Do zvoleného instalačního adresáře není možné zapisovat a může to znamenat restart instalátoru s oprávněními správce."

; Misc
${LangFileString} Info_QtArch_Bypassed "Kontrola SMPlayeru, architektury a Qt verze obejita pomocí příkazového řádku."
${LangFileString} Info_Portable_Bypassed "Přenosná kontrola přepsána příkazovým řádkem, vynutit přenosnou."
${LangFileString} Info_Portable_Detected "Nalezena přenosná verze SMPlayeru."
