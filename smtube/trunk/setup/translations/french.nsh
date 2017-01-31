;Language: French (1036)
;French language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "French" "Français"
!else
!insertmacro LANGFILE "French" = "Français" "Francais"
!endif

; Startup
${LangFileString} Installer_No_Admin "Vous devez être connecté comme administrateur pour installer ce programme."

${LangFileString} Win64_Required "Ce programme nécessite une version 64bits de Windows pour pouvoir être installé."

; Welcome page
${LangFileString} WelcomePage_Title "Configuration $(^NameDA)"
${LangFileString} WelcomePage_Text "Ce programme va vous guider à travers l'installation de $(^NameDA).$\r$\n$\r$\nIl est recommandé de quitter toutes les instances de SMTube avant de continuer. Ceci permet de mettre à jour les fichiers concernés sans avoir besoin de redémarrer votre ordinateur.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (requis)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube doit être installé dans un répertoire contentant une installation ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} de SMPlayer."
${LangFileString} DirectoryIsNotWritable "Le répertoire choisi pour l'installation n'est pas accessible en écriture et pourrait nécessiter le redémarrage du programme d'installation avec les privilèges d'administrateur.$\r$\n$\r$\nContinuer l'installation ?"

; Misc
${LangFileString} Info_QtArch_Bypassed "Vérification de SMPlayer, l'architecture et la version de Qt contournée par la ligne de commande."
${LangFileString} Info_Portable_Bypassed "Vérification de la portabilité remplacée par la ligne de commande, forcer la portabilité."
${LangFileString} Info_Portable_Detected "La version portable de SMPlayer a été trouvée."
