#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T15:34:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iClient
TEMPLATE = app


SOURCES += main.cpp\
        ChatClientDlg.cpp \
    LoginDlg.cpp \
    ChatDlg.cpp \
    ConfigDlg.cpp \
    lib/url_client.cpp

HEADERS  += ChatClientDlg.h \
    LoginDlg.h \
    ChatDlg.h \
    AppDef.h \
    ConfigDlg.h \
    lib/url_client.h

FORMS    += ChatClientDlg.ui \
    Login.ui \
    ChatDlg.ui \
    ConfigDlg.ui

RC_FILE += iChatClient.rc

QT      += xml

win32: LIBS += -L$$PWD/lib/curl/ -llibcurl

INCLUDEPATH += $$PWD/lib/curl
DEPENDPATH += $$PWD/lib/curl

