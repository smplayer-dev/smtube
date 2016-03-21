;Language: Japanese (1041)
;Japanese language strings for the Windows SMTube NSIS installer.
;
;Save file as UTF-8 w/ BOM
;

!insertmacro LANGFILE "Japanese" "日本語"

; Startup
${LangFileString} Installer_No_Admin "このプログラムをインストールするときは管理者としてログインする必要があります。"

${LangFileString} Win64_Required "このソフトウェアをインストールするには 64 ビットの Windows オペレーティング システムが必要です。"

; Welcome page
${LangFileString} WelcomePage_Title "$(^NameDA) のセットアップ"
${LangFileString} WelcomePage_Text "セットアップは $(^NameDA) のインストールをご案内します。$\r$\n$\r$\nセットアップの開始前にすべての SMTube のインスタンスを閉じることが推奨されます。これによりお使いのコンピューターを再起動する必要なく関連するプログラム ファイルを更新することが可能になります。$\r$\n$\r$\n$_CLICK"

; Components Page
${LangFileString} Section_SMTube "SMTube (必須)"

; Directory Page
${LangFileString} DirectoryPage_Text "$(^DirText)$\r$\n$\r$\nSMTube は SMPlayer の ${SMTUBE_INST_ARCH} ${SMTUBE_QT_VER} を含むディレクトリにインストールされなければなりません。"
${LangFileString} DirectoryIsNotWritable "選択されたインストール ディレクトリは書き込み可能ではなく管理者特権でインストールを再起動することが必要となる可能性があります。$\r$\n$\r$\nインストールを続行しますか?"

; Misc
${LangFileString} Info_QtArch_Bypassed "SMPlayer、アーキテクチャおよび Qt のバージョン チェックがコマンドラインによってバイパスされました。"
${LangFileString} Info_Portable_Bypassed "ポータブル チェックがコマンドラインによって上書きされました、ポータブルを強制します。"
${LangFileString} Info_Portable_Detected "SMPlayer のポータブル バージョンを見つけました。"
