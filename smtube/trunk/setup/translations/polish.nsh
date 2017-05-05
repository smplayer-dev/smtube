;Language: Polish (1045)
;Polish language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Polish" "Polski"
!else
!insertmacro LANGFILE "Polish" = "Polski" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Do instalacji tego programu wymagane są uprawnienia administratora."

${LangFileString} Win64_Required "Do zainstalowania tego programu wymagany jest system operacyjny Windows w wersji 64-bitowej."

; Welcome page
${LangFileString} WelcomePage_Title "Instalator $(^NameDA)"
${LangFileString} WelcomePage_Text "Instalator poprowadzi Cię przez proces instalacji $(^NameDA).$\r$\n$\r$\nZalecane jest zamknięcie wszystkich instancji SMTube przed rozpoczęciem instalacji. Dzięki temu możliwe będzie zaktualizowanie odpowiednich plików programu, bez konieczności ponownego uruchamiania komputera.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (wymagany)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube musi być zainstalowany do katalogu zawierającego ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} instalację SMPlayer."
${LangFileString} DirectoryIsNotWritable "Wybrany katalog instalacyjny nie ma prawa zapisu i może wymagać ponownego uruchomienia instalatora z uprawnieniami administratora.$\r$\n$\r$\nKontynuować instalację?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, architektura i kontrola wersji Qt pominięte przez wiersz poleceń."
${LangFileString} Info_Portable_Bypassed "Kontrola przenośna nadpisywana przez wiersz poleceń, wymuszając tryb przenośny."
${LangFileString} Info_Portable_Detected "Znaleziono przenośną wersję SMPlayer."
