;Language: Russian (1049)
;Russian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Russian" "Русский"

; Startup
${LangFileString} Installer_No_Admin "Для установки программы необходимо войти в систему как администратор."

${LangFileString} Win64_Required "Для установки данного ПО требуется 64-битная ОС Windows."

; Welcome page
${LangFileString} WelcomePage_Title "Установка $(^NameDA) "
${LangFileString} WelcomePage_Text "Мастер установки поможет установить $(^NameDA).$\r$\n$\r$\nПеред началом установки рекомендуется закрыть все работающие копии SMTube. Это позволит обновить сопутствующие программные файлы без перезагрузки компьютера.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (необходим)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube должен быть установлен в директорию, содержащую ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} установку SMPlayer'а."
${LangFileString} DirectoryIsNotWritable "Выбранная директория для установки не доступна для записи, и может потребоваться перезапуск инсталлятора с правами администратора.$\r$\n$\r$\nПродолжить установку?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, проверка архитектуры и Qt версии обойдены командной строкой."
${LangFileString} Info_Portable_Bypassed "Проверка на портативность переопределена командной строкой, принуждая к портативности."
${LangFileString} Info_Portable_Detected "Найдена переносная версия SMPlayer."
