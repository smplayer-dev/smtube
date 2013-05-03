######################################################################
# Automatically generated by qmake (2.01a) mar feb 21 00:12:34 2012
######################################################################

TEMPLATE = app
QT += network xml
TARGET = 
DEPENDPATH += . qtsingleapplication
INCLUDEPATH += . qtsingleapplication

CONFIG += qt warn_on release

RESOURCES = icons.qrc

DEFINES += NO_SMPLAYER_SUPPORT

# Input
HEADERS += myborder.h \
           myicon.h \
           downloadfile.h \
           recordingdialog.h \
           retrievevideourl.h \
           searchbox.h \
           ytdataapi.h \
           ytdelegate.h \
           ytdialog.h \
           yttabbar.h \
           lineedit_with_icon.h \
           filechooser.h \
           configdialog.h \
           players.h \
           version.h \
           about.h

SOURCES += myborder.cpp \
           myicon.cpp \
           downloadfile.cpp \
           recordingdialog.cpp \
           retrievevideourl.cpp \
           searchbox.cpp \
           ytdataapi.cpp \
           ytdelegate.cpp \
           ytdialog.cpp \
           yttabbar.cpp \
           lineedit_with_icon.cpp \
           filechooser.cpp \
           configdialog.cpp \
           players.cpp \
           version.cpp \
           about.cpp \
           main.cpp

FORMS += configdialog.ui about.ui

TRANSLATIONS = translations/smtube_es.ts \
               translations/smtube_en.ts \
               translations/smtube_ru_RU.ts \
               translations/smtube_lt.ts \
               translations/smtube_ja.ts \
               translations/smtube_zh_CN.ts \
               translations/smtube_eu.ts \
               translations/smtube_pt.ts \
               translations/smtube_hu.ts \
               translations/smtube_pl.ts \
               translations/smtube_gl.ts \
               translations/smtube_el.ts \
               translations/smtube_he_IL.ts \
               translations/smtube_pt_BR.ts \
               translations/smtube_uk.ts \
               translations/smtube_nn_NO.ts \
               translations/smtube_cs.ts \
               translations/smtube_ms_MY.ts \
               translations/smtube_sv.ts \
               translations/smtube_tr.ts \
               translations/smtube_fi.ts


# qtsingleapplication
SOURCES += qtsingleapplication.cpp qtlocalpeer.cpp
HEADERS += qtsingleapplication.h qtlocalpeer.h

unix {
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj

    DEFINES += TRANSLATION_PATH=$(TRANSLATION_PATH)
}

win32 {
    RC_FILE = smtube.rc
}
