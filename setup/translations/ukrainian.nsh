;Language: Ukrainian (1058)
;Ukrainian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Ukrainian" "Українська"
!else
!insertmacro LANGFILE "Ukrainian" = "Українська" "Ukrayins'ka"
!endif

; Startup
${LangFileString} Installer_No_Admin "Для встановлення програми потрібно увійти до системи як розпорядник."

${LangFileString} Win64_Required "Для встановлення цього ПЗ потрібна 64-бітова ОС Windows."

; Welcome page
${LangFileString} WelcomePage_Title "Встановлення $(^NameDA) "
${LangFileString} WelcomePage_Text "Майстер встановлення допоможе встановити $(^NameDA).$\r$\n$\r$\nПеред початком встановлення радиться закрити всі активні копії SMTube. Це дозволить оновити супутні програмні файли без перезавантаження комп’ютера.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (необхідний)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube має бути встановлений у директорію, що містить ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} встановлення SMPlayer'а."
${LangFileString} DirectoryIsNotWritable "Обрана директорія для встановлення недоступна для запису, й може знадобитися перезапуск встановлювача з правами розпорядника.$\r$\n$\r$\nПродовжити встановлення?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, перевірку архітектури та Qt версії обійдено командним рядком."
${LangFileString} Info_Portable_Bypassed "Перевірку на портативність перевизначено командним рядком, примушуючи до портативности."
${LangFileString} Info_Portable_Detected "Знайдено портативну версію SMPlayer."
