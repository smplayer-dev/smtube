CONFIG += release
#CONFIG += debug

QT += network

#DEFINES += SMTUBE_LIB

!contains(DEFINES, SMTUBE_LIB) {
	TEMPLATE = app
	DEFINES += USE_PLAYERS
	#DEFINES += D_BUTTON
	DEFINES += STYLE_SWITCHING
	DEFINES += CODEDOWNLOADER
	DEFINES += USE_SITES
	DEFINES += FONT_CHANGE
	DEFINES += USE_YT_DL
	DEFINES += USE_QPROCESS
	DEFINES += ABOUT_DIALOG
} else {
	TEMPLATE = lib
	CONFIG += staticlib
}
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00

contains(DEFINES, NO_USE_PLAYERS) {
	DEFINES -= USE_PLAYERS
}

!contains(DEFINES, USE_YT_DL) {
	DEFINES -= USE_PLAYERS
	DEFINES -= CODEDOWNLOADER
}

contains(DEFINES, USE_PLAYERS) {
	DEFINES += SELECT_RESOLUTION
}

contains(DEFINES, USE_PLAYERS|D_BUTTON|STYLE_SWITCHING|USE_SITES|FONT_CHANGE) {
	DEFINES += USE_CONFIG_DIALOG
}

# If Qt >= 5.4
greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 3) {
	DEFINES += HDPI_SUPPORT
}

isEqual(QT_MAJOR_VERSION, 5) {
	QT += webenginewidgets widgets gui
} else {
	QT += webkit
}

HEADERS = links.h \
          mywebview.h \
          mywebpage.h \
          mycookiejar.h \
          supportedurls.h \
          browserwindow.h

SOURCES = mywebview.cpp \
          mywebpage.cpp \
          mycookiejar.cpp \
          supportedurls.cpp \
          browserwindow.cpp

RESOURCES = smtube_icons.qrc

!contains(DEFINES, SMTUBE_LIB) {
	HEADERS += desktopinfo.h version.h
	SOURCES += desktopinfo.cpp version.cpp main.cpp
}

contains(DEFINES, USE_PLAYERS) {
	HEADERS += players.h
	SOURCES += players.cpp
} else {
	HEADERS += hcplayer.h
}

contains(DEFINES, USE_SITES) {
	HEADERS += sites.h sitedialog.h
	SOURCES += sites.cpp sitedialog.cpp
	FORMS += sitedialog.ui
}

contains( DEFINES, HDPI_SUPPORT ) {
	HEADERS += hdpisupport.h
	SOURCES += hdpisupport.cpp
}

contains( DEFINES, CODEDOWNLOADER ) {
	HEADERS += codedownloader.h
	SOURCES += codedownloader.cpp
}

contains(DEFINES, USE_CONFIG_DIALOG) {
	HEADERS += configdialog.h
	SOURCES += configdialog.cpp
	FORMS += configdialog.ui playerdialog.ui

	contains(DEFINES, USE_PLAYERS) {
		HEADERS += myscroller.h lineedit_with_icon.h filechooser.h playerdialog.h
		SOURCES += myscroller.cpp lineedit_with_icon.cpp filechooser.cpp playerdialog.cpp
	}
}

contains(DEFINES, USE_YT_DL) {
	HEADERS += retrieveyoutubeurl.h
	SOURCES += retrieveyoutubeurl.cpp

	isEqual(QT_MAJOR_VERSION, 4) {
		HEADERS += qt-json/json.h
		SOURCES += qt-json/json.cpp
	}

	win32 {
		!contains( DEFINES, PORTABLE_APP ) {
			DEFINES += YT_BIN_ON_CONFIG_DIR
		}
	}
}

contains(DEFINES, ABOUT_DIALOG) {
	HEADERS += about.h
	SOURCES += about.cpp
	FORMS += about.ui
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

mac {
	ICON = smtube.icns
	translations.files += $$files(translations/*.qm)
	translations.path = Contents/Resources/translations
	QMAKE_BUNDLE_DATA += translations
}

unix:!macx {
	DEFINES += OS_UNIX_NOT_MAC
}

TRANSLATIONS = translations/smtube_es.ts \
               translations/smtube_es_ES.ts \
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
               translations/smtube_sq.ts \
               translations/smtube_tr.ts \
               translations/smtube_hu.ts \
               translations/smtube_he_IL.ts \
               translations/smtube_el.ts \
               translations/smtube_nb_NO.ts \
               translations/smtube_ca.ts

