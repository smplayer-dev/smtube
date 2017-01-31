;Language: Greek (1032)
;Greek language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!if ! ${NSIS_PACKEDVERSION} > 0x2999999
!insertmacro LANGFILE "Greek" "Ελληνικά"
!else
!insertmacro LANGFILE "Greek" = "Ελληνικά" "Ellinika"
!endif

; Startup
${LangFileString} Installer_No_Admin "Θα πρέπει να συνδεθείτε ως διαχειριστής κατά την εγκατάσταση του προγράμματος."

${LangFileString} Win64_Required "Απαιτείται ένα λειτουργικό σύστημα Windows 64 bit ούτως ώστε να εγκαταστήσετε αυτό το λογισμικό."

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) Εγκατάσταση"
${LangFileString} WelcomePage_Text "Το πρόγραμμα εγκατάστασης θα σας καθοδηγήσει κατά την εγκατάσταση του $(^NameDA).$\r$\n$\r$\nΣυνιστάται να κλείσετε όλα τα παράθυρα του SMTube πριν την εκκίνηση της εγκατάστασης. Αυτό θα σας επιτρέψει να ενημερώσετε τα σχετικά αρχεία του προγράμματος χωρίς την ανάγκη να επανεκκινήσετε τον υπολογιστή.$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (απαιτείται)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nΤο SMTube πρέπει να εγκατασταθεί σε έναν κατάλογο που περιέχει μια ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} εγκατάσταση του SMPlayer."
${LangFileString} DirectoryIsNotWritable "Ο επιλεγμένος κατάλογος εγκατάστασης δεν είναι εγγράψιμος και ίσως χρειάζεται να επανεκκινήσετε το πρόγραμμα εγκατάστασης με προνόμια διαχειριστή.$\r$\n$\r$\nΝα συνεχίσει η εγκατάσταση;"

; Misc
${LangFileString} Info_QtArch_Bypassed "Το SMPlayer, ο έλεγχος της αρχιτεκτονικής και της έκδοσης της Qt παρακάμφθηκαν από την γραμμή εντολών."
${LangFileString} Info_Portable_Bypassed "Ο έλεγχος της φορητής έκδοσης παρακάμφθηκε από την γραμμή εντολών, εξαναγκασμός της φορητής έκδοσης."
${LangFileString} Info_Portable_Detected "Βρέθηκε μια φορητή έκδοση του SMPlayer."
