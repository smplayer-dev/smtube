;Language: Danish (1030)
;Danish language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Danish" "Dansk"
!else
!insertmacro LANGFILE "Danish" = "Dansk" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Du skal være logget ind som administrator når dette program installeres."

${LangFileString} Win64_Required "Det kræver et 64-bit Windows-styresystem at installere dette software."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) installation"
${LangFileString} WelcomePage_Text "Installationsprogrammet guider dig gennem installationen af $(^NameDA).$\r$\n$\r$\nDet anbefales at du lukker alle forekomster af SMTube før installationen startes. Dermed kan de relevante programfiler opdateres uden at skulle genstarte din computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (krævet)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube skal være installeret i en mappe som indeholder en ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} installation af SMPlayer."
${LangFileString} DirectoryIsNotWritable "Den valgte installationsmappe er skrivebeskyttet og det kan være at installationen skal startes igen med administratorrettigheder.$\r$\n$\r$\nFortsæt med installationen?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer-, arkitektur- og Qt versionstjek forbigået af kommandolinje."
${LangFileString} Info_Portable_Bypassed "Portabletjek tilsidesat af kommandolinje, gennemtvinger portable."
${LangFileString} Info_Portable_Detected "Fandt portable version af SMPlayer."
