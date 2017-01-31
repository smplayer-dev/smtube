;Language: Chinese (Simplified) (2052)
;Chinese (Simplified) language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "SimpChinese" "简体中文"
!else
!insertmacro LANGFILE "SimpChinese" "Chinese (Simplified)" "中文(简体)" "Hanyu (Jiantizi)"
!endif

; Startup
${LangFileString} Installer_No_Admin "必须以管理员身份安装此程序。"

${LangFileString} Win64_Required "安装此软件需要 64 位 Windows 操作系统。"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) 设置"
${LangFileString} WelcomePage_Text "设置程序将引导您完成安装 $(^NameDA).$\r$\n$\r$\n 建议您在开始安装前关闭所有的 SMTube 实例。此操作将允许不重启电脑而完成软件安装.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (必需)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube 必须安装到包含 ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer的目录中。"
${LangFileString} DirectoryIsNotWritable "选中的安装目录不可写，可能需要管理员权限才能执行，$\r$\n$\r$\n是否继续安装?"

; Misc
${LangFileString} Info_QtArch_Bypassed "通过命令行可以跳过 SMPlayer、架构及 Qt 版本检查。"
${LangFileString} Info_Portable_Bypassed "通过命令行可以跳过便携性检查，强制使便携。"
${LangFileString} Info_Portable_Detected "查看绿色版 SMPlayer。"
