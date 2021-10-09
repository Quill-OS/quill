QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 c++17

QMAKE_CXXFLAGS += -Wno-unused-function -Wno-unused-parameter

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alert.cpp \
    apps.cpp \
    brightnessdialog.cpp \
    calendarapp.cpp \
    dictionarywidget.cpp \
    encryptionmanager.cpp \
    generaldialog.cpp \
    hourglassanimationwidget.cpp \
    koboxappsdialog.cpp \
    koboxsettings.cpp \
    main.cpp \
    mainwindow.cpp \
    otamanager.cpp \
    quit.cpp \
    reader.cpp \
    savedwords.cpp \
    searchresultswidget.cpp \
    settings.cpp \
    settingschooser.cpp \
    textwidget.cpp \
    toast.cpp \
    usbms_splash.cpp \
    virtualkeyboard.cpp \
    virtualkeypad.cpp \
    wifidialog.cpp

HEADERS += \
    alert.h \
    apps.h \
    brightnessdialog.h \
    calendarapp.h \
    dictionarywidget.h \
    encryptionmanager.h \
    functions.h \
    generaldialog.h \
    hourglassanimationwidget.h \
    koboxappsdialog.h \
    koboxsettings.h \
    mainwindow.h \
    otamanager.h \
    quit.h \
    reader.h \
    savedwords.h \
    searchresultswidget.h \
    settings.h \
    settingschooser.h \
    textwidget.h \
    toast.h \
    usbms_splash.h \
    virtualkeyboard.h \
    virtualkeypad.h \
    wifidialog.h

FORMS += \
    alert.ui \
    apps.ui \
    brightnessdialog.ui \
    calendarapp.ui \
    dictionarywidget.ui \
    encryptionmanager.ui \
    generaldialog.ui \
    hourglassanimationwidget.ui \
    koboxappsdialog.ui \
    koboxsettings.ui \
    mainwindow.ui \
    otamanager.ui \
    quit.ui \
    reader.ui \
    savedwords.ui \
    searchresultswidget.ui \
    settings.ui \
    settingschooser.ui \
    textwidget.ui \
    toast.ui \
    usbms_splash.ui \
    virtualkeyboard.ui \
    virtualkeypad.ui \
    wifidialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    eink.qrc
