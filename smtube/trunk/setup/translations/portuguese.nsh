;Language: Portuguese (2070)
;Portuguese language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Portuguese" "Português"
!else
!insertmacro LANGFILE "Portuguese" = "Português" "Portugues"
!endif

; Startup
${LangFileString} Installer_No_Admin "Tem que iniciar sessão como administrador para poder instalar esta aplicação."

${LangFileString} Win64_Required "Para poder instalar esta aplicação, tem que possuir um sistema de 64 bits."

; Welcome page
${LangFileString} WelcomePage_Title "Configuração do $(^NameDA)"
${LangFileString} WelcomePage_Text "Esta configuração irá ajudá-lo a instalar o $(^NameDA).$\r$\n$\r$\nÉ altamente recomendado o encerramento de quaisquer instâncias do SMTube antes de iniciar a instalação. Deste modo, será possível atualizar os ficheiros do programa sem ter que reiniciar o computador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (obrigatório)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube must be installed to a directory containing a ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} installation of SMPlayer."
${LangFileString} DirectoryIsNotWritable "Não tem permissões para escrever o diretório escolhido e pode ser necessário executar a instalação com poderes de administrador.$\r$\n$\r$\nContinuar com a instalação?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, architecture and Qt version check bypassed by command-line."
${LangFileString} Info_Portable_Bypassed "Portable check overriden by command-line, forcing portable."
${LangFileString} Info_Portable_Detected "Found portable version of SMPlayer."
