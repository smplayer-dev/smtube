;Language: German (1031)
;German language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "German" "Deutsch"
!else
!insertmacro LANGFILE "German" = "Deutsch" =
!endif

; Startup
${LangFileString} Installer_No_Admin "Sie müssen als Systemverwalter angemeldet sein, um das Programm zu installieren."

${LangFileString} Win64_Required "Ein 64-bit-Windows-Betriebssystem wird benötigt, um das Programm zu installieren."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA)-Einrichtung"
${LangFileString} WelcomePage_Text "Setup will guide you through the installation of $(^NameDA).$\r$\n$\r$\nIt is recommended that you close all instances of SMTube before starting setup. This will make it possible to update relevant program files without having to reboot your computer.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (erforderlich)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube muss in ein Verzeichniss installiert werden, das eine ${SMTUBE_INST_ARCH}-${SMTUBE_QT_VER}-Installation vom SMPlayer enthält."
${LangFileString} DirectoryIsNotWritable "Das ausgewählte Installationsverzeichnis ist nicht beschreibbar und benötigt den Neustart des Installationsprogramms mit Systemverwaltungsrechten.$\r$\n$\r$\nMit der Installation fortfahren?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer-, Architektur- und Qt-Versionsprüfung durch Befehlszeile umgangen."
${LangFileString} Info_Portable_Bypassed "Portable check overriden by command-line, forcing portable."
${LangFileString} Info_Portable_Detected "Portable Version vom SMPlayer gefunden."
