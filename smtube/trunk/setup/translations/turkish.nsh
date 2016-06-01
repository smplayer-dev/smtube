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

${LangFileString} Win64_Required "Bu yazılım kurulumu için 64-bit Windows işletim sistemi gerekli."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Kurulum"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMTube before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube " SMTube (gerekli)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube must be installed to a directory containing a ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} installation of SMPlayer."
${LangFileString} DirectoryIsNotWritable "The chosen installation directory is not writable and may require restarting the installer with administrator privileges.$\r$\n$\r$\nProceed with installation?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, architecture and Qt version check bypassed by command-line."
${LangFileString} Info_Portable_Bypassed "Portable check overriden by command-line, forcing portable."
${LangFileString} Info_Portable_Detected "Taşınabilir SMPlayer sürümü bulundu."
