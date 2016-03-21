;Language: Basque (1069)
;Basque language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Basque" "Euskara"

; Startup
${LangFileString} Installer_No_Admin "Administratzaile bezala saioa hasita egon behar duzu programa hau ezartzerakoan."

${LangFileString} Win64_Required "64-biteko Windows sistema eragile bat behar da software hau ezartzeko."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Ezarpena"
${LangFileString} WelcomePage_Text "Ezartzaileak $(^NameDA)-ren ezarpenean zehar gidatuko zaitu.$\r$\n$\r$\nGomendagarria da SMPlayer eskabide guztiak istea ezarpena hasi aurretik. Honek zure ordenagailua berrabiarazi gabe programa agiri garrantzitsuak eguneratu ahal izatea egiten du.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (beharrezkoa)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} SMPlayer ezarpena duen zuzenbide batean ezarri behar da."
${LangFileString} DirectoryIsNotWritable "Hautaturiko ezarpen zuzenbidea ez da idazgarria eta ezartzailea administrari eskubideekin berrabiaraztea behar izan dezake.$\r$\n$\r$\nJarraitu ezarpenarekin?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, arkitektura eta Qt bertsio egiaztapena igarota agindu-lerrotik."
${LangFileString} Info_Portable_Bypassed "Eramangari egiaztapena agindu-lerroak ezeztatuta, eramangarria behartzen."
${LangFileString} Info_Portable_Detected "SMPlayer bertsio eramangarri bat aurkitu da."
