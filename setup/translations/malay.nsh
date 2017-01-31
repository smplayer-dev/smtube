;Language: Malay (1086)
;Malay language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Malay" "Melayu"
!else
!insertmacro LANGFILE "Malay" = "Bahasa Melayu" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Anda mesti mendaftar masuk sebagai pentadbir bila memasang program ini."

${LangFileString} Win64_Required "Sistem pengoperasian Windows 64-bit diperlukan untuk memasang perisian ini."

; Welcome page
${LangFileString} WelcomePage_Title "Persediaan $(^NameDA)"
${LangFileString} WelcomePage_Text "Persediaan akan membantu anda melalui pemasangan $(^NameDA).$\r$\n$\r$\nAdalah disarankan anda tutup semua kejadian SMTube sebelum memulakan persediaan. Ia bertujuan untuk mengemaskini fail program yang berkaitan tanpa perlu membut semula komputer anda.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (diperlukan)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube mesti dipasang ke direktori yang mengandungi pemasangan ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer."
${LangFileString} DirectoryIsNotWritable "Direktori pemasangan pilihan tidak boleh ditulis dan perlu memulakan semula pemasang dengan kelayakan pentadbir.$\r$\n$\r$\nTeruskan pemasangan?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Semakan versi SMPlayer, senibina dan Qt dengan baris-perintah."
${LangFileString} Info_Portable_Bypassed "Semakan boleh alih dibatalkan oleh baris-peintah, memaksa boleh alih."
${LangFileString} Info_Portable_Detected "Temui versi boleh alih SMPlayer."
