;Language: Serbian (3098)
;Serbian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Serbian" "Српски"
!else
!insertmacro LANGFILE "Serbian" "Serbian (Cyrillic)" "Српски" "Srpski (Cyrillic)"
!endif

; Startup
${LangFileString} Installer_No_Admin "Морате бити пријављени као администратор када инсталирате овај програм."

${LangFileString} Win64_Required "64-битни Виндоуз оперативни систем је потребан за инсталацију овог програма."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) програм инсталације"
${LangFileString} WelcomePage_Text "Програм инсталације ће вас водити кроз инсталацију $(^NameDA).$\r$\n$\r$\nПрепорука је да затворите претходно отворени СМТјуб пре почетка инсталације. То ће омогућити ажурирање битних фајлова без потребе за поновним покретањем вашег рачунара.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "СМТјуб (потребно)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nСМТјуб мора бити инсталиран у директоријум који садржи ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} инсталацију СМПлејера."
${LangFileString} DirectoryIsNotWritable "Изабрани директоријум није уписив и може захтевати поновно покретање инсталације са администраторским овлашћењима.$\r$\n$\r$\nНаставити са инсталацијом?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Провера архитектуре, КуТ верзије и СМПлејера је избегнута из командне линије."
${LangFileString} Info_Portable_Bypassed "Провера портабилности избегнута из командне линије. Форсирам портабилну"
${LangFileString} Info_Portable_Detected "Нађена је портабилна верзија СМПлејера."
