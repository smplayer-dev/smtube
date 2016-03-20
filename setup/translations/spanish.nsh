;Language: Spanish (1034)
;Spanish language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Spanish" "Español"

; Startup
${LangFileString} Installer_No_Admin "Debes tener permisos de administrador para instalar este programa."

${LangFileString} Win64_Required "Se necesita un Windows de 64 bits para instalar este programa."

; Welcome page
${LangFileString} WelcomePage_Title "Instalación de $(^NameDA)"
${LangFileString} WelcomePage_Text "A continuación se procederá a la instalación de $(^NameDA).$\r$\n$\r$\nSe recomienda cerrar todos los procesos de SMTube antes de comenzar con la instalación. De esta manera será posible actualizar los ficheros del programa sin tener que reiniciar el ordenador.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (requerido)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSe debe instalar SMTube en una carpeta que contenga una instalación de SMPlayer de ${SMTUBE_INST_ARCH} y ${SMTUBE_QT_VER}."
${LangFileString} DirectoryIsNotWritable "La carpeta seleccionada para la instalación no tiene permisos de escritura y puede requerir reiniciar el instalador con permisos de administrador.$\r$\n$\r$\n¿Seguir con la instalación?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer, la comprobación de arquitectura y versión de Qt han sido anulados por la línea de comandos."
${LangFileString} Info_Portable_Bypassed "La comprobación de versión portable ha sido anulada por la línea de comandos, forzando portable."
${LangFileString} Info_Portable_Detected "Se ha encontrado una versión portable de SMPlayer."
