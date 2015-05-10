
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
          browserwindow.h

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
          main.cpp

FORMS = playerdialog.ui configdialog.ui

RESOURCES = icons.qrc

TRANSLATIONS = translations/smtube_es.ts \
               translations/smtube_en.ts

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
