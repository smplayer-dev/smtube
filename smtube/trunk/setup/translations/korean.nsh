;Language: Korean (1042)
;Korean language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Korean" "한국어"

; Startup
${LangFileString} Installer_No_Admin "이 프로그램을 설치하려면 관리자 권한이 필요합니다."

${LangFileString} Win64_Required "이 프로그램은 윈도우 64비트 운영 체계에서만 설치할 수 있습니다."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Setup"
${LangFileString} WelcomePage_Text "Setup이 $(^NameDA)의 설치를 도와줄 것입니다.$\r$\n$\r$\nSetup을 시작하기 전에 SMTube를 모두 종료하세요. 그러면 재부팅 없이 관련 프로그램 파일을 업데이트할 수 있습니다.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (필수)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube는 반드시 ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer가 설치된 폴더에 설치해야 합니다."
${LangFileString} DirectoryIsNotWritable "선택한 폴더에 설치할 수가 없어서 관리자 권한으로 설치 프로그램을 재시작해야 합니다.$\r$\n$\r$\n설치를 계속 진행할까요?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer의 구조와 Qt 버전 검사를 명령행에 의해 건너뜁니다."
${LangFileString} Info_Portable_Bypassed "포터블 검사는 포터블 강제 사용 명령행에 의해 취소됨."
${LangFileString} Info_Portable_Detected "포터블 버전의 SMPlayer를 발견했습니다."
