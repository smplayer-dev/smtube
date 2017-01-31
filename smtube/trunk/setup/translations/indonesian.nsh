;Language: Indonesian (1057)
;Indonesian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Indonesian" "Bahasa Indonesia"
!else
!insertmacro LANGFILE "Indonesian" = "Bahasa Indonesia" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Anda harus masuk sebagai pengurus ketika memasang program ini."

${LangFileString} Win64_Required "Sistem operasi Windows 64-bit diperlukan untuk memasang perangkat lunak ini."

; Welcome page
${LangFileString} WelcomePage_Title "Penyiapan %(^NameDA)"
${LangFileString} WelcomePage_Text "Persiapan akan memandu kamu melalui pemasangan $(^NameDA).$\r$\n$\r$\nKamu dianjurkan untuk menutup semua misalnya SMTube sebelum memulai persiapan. Ini mungkin akan membuat pembaruan file-file program yang relevan tanpa harus memboot ulang komputermu.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (diperlukan)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube harus dipasang ke sebuah direktori berisi ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} pemasangan dari SMPlayer."
${LangFileString} DirectoryIsNotWritable "Pilihan direktori pemasangan tidak dapat ditulis dan mungkin perlu memulai ulang pemasang dengan hak pengurus.$\r$\n$\r$\nLanjutkan pemasangan?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, arsitektur dan versi Qt periksa bypassed dengan baris perintah."
${LangFileString} Info_Portable_Bypassed "Portable periksa penimpaan dengan baris perintah, memaksa portable."
${LangFileString} Info_Portable_Detected "Ditemukan versi portable dari SMPlayer."
