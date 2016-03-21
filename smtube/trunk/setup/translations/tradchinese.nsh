;Language: Chinese (Traditional) (1028)
;Chinese (Traditional) language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "TradChinese" "漢語(繁體)"

; Startup
${LangFileString} Installer_No_Admin "安裝此程式時，您必須先登入為管理員。"

${LangFileString} Win64_Required "要安裝此軟體，系統必須是 64 位元的 Windows。"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) 安裝"
${LangFileString} WelcomePage_Text "安裝程式將會引導您完成安裝 $(^NameDA)。 $\r$\n$\r$\n建議您在開始安裝前，先關閉所有的 SMTube 執行實體。這將使它能夠更新相關的程式檔案，而無需重新啟動您的電腦。 $\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (必要的)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube 必須安裝至包含 ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer 的目錄。"
${LangFileString} DirectoryIsNotWritable "選定的安裝目錄無法寫入，可能需要以管理員權限重新啟動安裝程式。$\r$\n$\r$\n繼續安裝？"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer，架構與 Qt 版本檢查由命令列繞過。"
${LangFileString} Info_Portable_Bypassed "由命令列覆寫可攜性檢查，強制可攜性。"
${LangFileString} Info_Portable_Detected "找到可攜版的 SMPlayer。"
