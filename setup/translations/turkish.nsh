;Language: Turkish (1055)
;Turkish language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Turkish" "Türkçe"
!else
!insertmacro LANGFILE "Turkish" = "Türkçe" "Turkce"
!endif

; Startup
${LangFileString} Installer_No_Admin "Bu programı yüklerken yönetici olarak oturum açmanız gerekir."

${LangFileString} Win64_Required "Bu yazılımın kurulumu için 64-bit Windows işletim sistemi gerekli."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Kurulum"
${LangFileString} WelcomePage_Text "Kurulum $(^NameDA) 'ın kurulumunda size rehberlik edecek.$\r$\n$\r$\nKurulum başlamadan önce SMTube'un tüm benzerlerini kapatmanız önerilir. Bu bilgisayarınızı yeniden başlatmak zorunda kalmadan ilgili program dosyalarını güncellemek mümkün olacaktır.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube " SMTube (gerekli)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube, bir smplayer ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} kurulumunu içeren bir dizine kurulmalıdır."
${LangFileString} DirectoryIsNotWritable "Seçilen kurulrm dizini yazılabilir değil ve kurucuyu yönetici yetkisiyle tekrar başlatmanız gerekiyor. $\r$\n$\r$\nKurulum devam etsin mi?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, mimari ve Qt sürüm kontrolü komut satırı ile atlanabilir."
${LangFileString} Info_Portable_Bypassed "Kurulumsuz kontrol komut satırından iptal edilmiş,kurulumsuzu zorla."
${LangFileString} Info_Portable_Detected "Taşınabilir SMPlayer sürümü bulundu."
