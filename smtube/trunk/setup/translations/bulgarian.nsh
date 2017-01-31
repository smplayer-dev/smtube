;Language: Bulgarian (1026)
;Bulgarian language strings for the Windows SMPlayer NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Bulgarian" "Български"
!else
!insertmacro LANGFILE "Bulgarian" = "Български" "Balgarski"
!endif

; Startup
${LangFileString} Installer_No_Admin "Трябва да сте вписан като администратор, при инсталирането на тази програма."

${LangFileString} Win64_Required "Изисква се 64-битов Windows за инсталирането на тази програма."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Съветник"
${LangFileString} WelcomePage_Text "Съветника ще Ви преведе през инсталацията на $(^NameDA).$\r$\n$\r$\nПрепоръчително е да затворите всички работещи истанции на SMTube преди инсталиране. Това ще позволи обновяването на значими за програмата файлове, без необходимостта от рестартиране на Вашия компютър.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (изисква се)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube трябва да бъде инсталиран в папка, съдържаща ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} инсталация на SMPlayer."
${LangFileString} DirectoryIsNotWritable "Не е позволен записа в избраната папка и може да е необходим рестарт на инсталатора с администраторски права.$\r$\n$\r$\nПродължавате ли с инсталацията?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, архитектурата и версията на Qt са прехвърлени от командния ред."
${LangFileString} Info_Portable_Bypassed "Проверката за преносимост е презаписана от командния ред, принуждаване на преносимост."
${LangFileString} Info_Portable_Detected "Открита е преносима версия на SMPlayer."
