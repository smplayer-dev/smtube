
TEMPLATE = app
QT += network

CONFIG += release
#CONFIG += debug

DEFINES += USE_PLAYERS
DEFINES += YT_USE_SCRIPT

isEqual(QT_MAJOR_VERSION, 5) {
	QT += webkitwidgets widgets gui
} else {
	QT += webkit
}

HEADERS = mywebview.h \
          mywebpage.h \
          mycookiejar.h \
          ytsig.h \
          retrieveyoutubeurl.h \
          supportedurls.h \
          version.h \
          lineedit_with_icon.h \
          filechooser.h \
          playerdialog.h \
          configdialog.h \
          browserwindow.h \
          about.h

SOURCES = mywebview.cpp \
          mywebpage.cpp \
          mycookiejar.cpp \
          ytsig.cpp \
          retrieveyoutubeurl.cpp \
          supportedurls.cpp \
          version.cpp \
          lineedit_with_icon.cpp \
          filechooser.cpp \
          playerdialog.cpp \
          configdialog.cpp \
          browserwindow.cpp \
          about.cpp \
          main.cpp

FORMS = playerdialog.ui configdialog.ui about.ui

RESOURCES = icons.qrc

TRANSLATIONS = translations/smtube_es.ts \
               translations/smtube_en.ts \
               translations/smtube_de.ts \
               translations/smtube_fr.ts \
               translations/smtube_it.ts \
               translations/smtube_ko.ts \
               translations/smtube_pt.ts \
               translations/smtube_sr.ts \
               translations/smtube_eu.ts \
               translations/smtube_gl.ts \
               translations/smtube_ja.ts \
               translations/smtube_pt_BR.ts \
               translations/smtube_ru.ts \
               translations/smtube_zh_TW.ts \
               translations/smtube_cs.ts \
               translations/smtube_pl.ts \
               translations/smtube_nn_NO.ts \
               translations/smtube_hr.ts \
               translations/smtube_bg.ts \
               translations/smtube_da.ts \
               translations/smtube_en_GB.ts \
               translations/smtube_ms.ts \
               translations/smtube_zh_CN.ts \
               translations/smtube_uk.ts \
               translations/smtube_sq.ts

contains( DEFINES, YT_USE_SCRIPT ) {
	QT += script
	HEADERS += codedownloader.h
	SOURCES += codedownloader.cpp
}

contains(DEFINES, USE_PLAYERS) {
	HEADERS += players.h
	SOURCES += players.cpp
} else {
	HEADERS += hcplayer.h
}

unix {
	UI_DIR = .ui
	MOC_DIR = .moc
	OBJECTS_DIR = .obj
	DEFINES += TRANSLATION_PATH=$(TRANSLATION_PATH)
}

win32 {
	RC_FILE = smtube.rc
	CONFIG(debug, debug|release) {
		CONFIG += console
	}
}
