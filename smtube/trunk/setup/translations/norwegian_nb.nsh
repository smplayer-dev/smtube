;Language: Norwegian (1044)
;Norwegian language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Norwegian" "Norwegian"
!else
!insertmacro LANGFILE "Norwegian" = "Norsk" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Du må være logget inn som administrator når du installerer dette programmet."

${LangFileString} Win64_Required "Et 64-bits Windows-operativsystem er påkrevd for å installere denne programvaren."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA)-oppsett"
${LangFileString} WelcomePage_Text "Oppsettet vil lede deg gjennom installasjonen av $(^NameDA).$\r$\n$\r$\nDet er anbefalt at du lukker all SMTube-prosesser før du begynner oppsettet. Dette vil gjøre det mulig å oppdatere relevante programfiler uten at du behøver å starte maskinen din på nytt.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (påkrevd)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube må installeres i en filplassering som inneholder en ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} installasjon av SMPlayer."
${LangFileString} DirectoryIsNotWritable "Den ønskede installasjonsplasseringen kan ikke skrives til, og kan kreve en omstart av veiviseren med administratorrettigheter.$\r$\n$\r$\nVil du fortsette med installeringen?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Letinger etter SMPlayer, arkitektur og Qt-versjoner ble hoppet over gjennom kommandolinjen."
${LangFileString} Info_Portable_Bypassed "Flyttbarhetssjekk ble deaktivert gjennom kommandolinjen, fremtvinger flyttbarhet."
${LangFileString} Info_Portable_Detected "Fant flyttbar versjon av SMPlayer."
