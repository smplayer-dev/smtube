;Language: Dutch (1043)
;Dutch language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Dutch" "Nederlands"
!else
!insertmacro LANGFILE "Dutch" = "Nederlands" =
!endif

; Startup
${LangFileString} Installer_No_Admin "U moet ingelogd zijn als beheerder tijdens het installeren van het programma."

${LangFileString} Win64_Required "Een 64-bits-versie van Windows is benodigd om deze software te installeren."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Installatie"
${LangFileString} WelcomePage_Text "Setup zal u door de installatie van $(^NameDA) begeleiden.$\r$\n$\r$\nHet wordt aanbevolen dat u alle instanties van SMTube sluit voordat u de setup start. Dit maakt het mogelijk om relevante programmabestanden te updaten zonder uw computer te herstarten.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (vereist)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube moet geïnstalleerd worden in een map die een ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} installatie van SMPlayer bevat."
${LangFileString} DirectoryIsNotWritable "De gekozen map voor installatie is niet schrijfbaar, hierdoor is het mogelijk dat de installer herstart moet worden met beheerdersrechten.$\r$\n$\r$\nDoorgaan met de installatie?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Versiecontrole van SMPlayer, architectuur en Qt omzeild door opdrachtregel."
${LangFileString} Info_Portable_Bypassed "Draagbare check overschreven door opdrachtregel, draagbaar geforceerd."
${LangFileString} Info_Portable_Detected "Draagbare versie van SMPlayer gevonden"
