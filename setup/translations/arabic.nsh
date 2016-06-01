;Language: Arabic (1025)
;Arabic language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Arabic" "العربية"
!else
!insertmacro LANGFILE "Arabic" = "العربية" "Al-Arabiyyah"
!endif

; Startup
${LangFileString} Installer_No_Admin "يجب أن تكون مسجّل الدخول كمدير عند تثبيت هذا البرنامج."

${LangFileString} Win64_Required "يتطلب وجود نظام تشغيلي ويندوز ذي 64 بت لتثبيت هذا البرنامج."

; Welcome page
${LangFileString} WelcomePage_Title "إعداد $(^NameDA)"
${LangFileString} WelcomePage_Text "سيرشدك برنامج الإعداد عبر تثبيت $(^NameDA).$\r$\n$\r$\n من المستحسن إغلاق كل نوافذ SMTube قبل بدء الإعداد. سيمكنّك هذا من تحديث ملفات البرنامج المعنيّة دون الحاجة الى إعادة تشغيل حاسوبك.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "البرنامج SMTube (مطلوب)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nالبرنامج  يجب أن يُثبَّت في دليل ملفات يحتوي تنصيبة ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} من SMPlayer."
${LangFileString} DirectoryIsNotWritable "دليل الملفات المختار للثبيت غير قابل للكتابة و قد يتطلب إعادة تشغيل برنامج التثبيت بصلاحيات إدارية.$\r$\n$\r$\nالإستمرار بالتثبيت؟"

; Misc
${LangFileString} Info_QtArch_Bypassed "فحص المشغل و البنية و إصدار Qt متجاوَزة من قبَل سطر الأوامر."
${LangFileString} Info_Portable_Bypassed "فحص الإصدار المحمول متجاوًز من قبَل سطر الأوامر، فرض المحمول."
${LangFileString} Info_Portable_Detected "وُجد إصدار محمول من SMPlayer."
