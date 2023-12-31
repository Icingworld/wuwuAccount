QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32-msvc* {
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
SOURCES += \
    add.cpp \
    charts.cpp \
    checkgroup.cpp \
    database.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    add.h \
    charts.h \
    checkgroup.h \
    database.h \
    login.h \
    mainwindow.h

FORMS += \
    add.ui \
    login.ui \
    mainwindow.ui

TRANSLATIONS += \
    wuwuAcount_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logo.rc

RC_FILE += \
    logo.rc

RESOURCES += \
    resources.qrc
