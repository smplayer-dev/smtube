;Language: Portuguese Brazil (1046)
;Portuguese language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "PortugueseBR" "Português (Brasil)"

; Startup
${LangFileString} Installer_No_Admin "Você precisa estar logado como administrador quando instalar esse programa."

${LangFileString} Win64_Required "Um sistema operacional Windows 64-bit é necessário para instalar esse software."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Configuração"
${LangFileString} WelcomePage_Text "A configuração lhe guiará através da instalação do $(^NameDA).$\r$\n$\r$\nÉ recomendado fechar todas as instâncias do SMTube antes de iniciar a configuração. Isso tornará possível atualizar arquivos importantes do programa sem ter que reiniciar seu computador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (necessário)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nO SMTube deve ser instalado em uma pasta contendo uma ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} instalação do SMPlayer."
${LangFileString} DirectoryIsNotWritable "A pasta de instalação escolhida não pode ser alterada e requer que você reinicie o instalador como administrador do sistema.$\r$\n$\r$\nContinuar com a instalação?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Verificação da versão do Qt, arquitetura e SMPlayer contornada pela linha de comando."
${LangFileString} Info_Portable_Bypassed "Verificação portátil invalidada pela linha de comando, forçando portátil."
${LangFileString} Info_Portable_Detected "Encontrada versão portátil do SMPlayer."
