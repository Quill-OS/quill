QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 c++17

QMAKE_CXXFLAGS += -Wno-unused-function -Wno-unused-parameter

# Thanks to https://github.com/mrbindraw/TestVersion/blob/master/TestVersion.pro
GIT_VERSION = $$system(git describe --always --tags)
GIT_COMMIT = $$system(git rev-parse HEAD)
COMMAND_REMOVE_MAKEFILES=$$quote(rm $$system_path($$OUT_PWD\Makefile*))
PRE_BUILD_TARGET = .dummyfile
updatemakefiles.target = $$PRE_BUILD_TARGET
updatemakefiles.commands = $$COMMAND_REMOVE_MAKEFILES
updatemakefiles.depends = FORCE
PRE_TARGETDEPS += $$PRE_BUILD_TARGET
QMAKE_EXTRA_TARGETS += updatemakefiles
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
DEFINES += GIT_COMMIT=\\\"$$GIT_COMMIT\\\"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/apps/todo.cpp \
    src/audio/audiothread.cpp \
    src/splash/alert.cpp \
    src/apps/apps.cpp \
    src/widgets/dialogs/audio/audiodialog.cpp \
    src/widgets/dialogs/audio/audiofile.cpp \
    src/widgets/dialogs/audio/audiofilequeue.cpp \
    src/widgets/dialogs/library/bookinfodialog.cpp \
    src/widgets/dialogs/library/bookoptionsdialog.cpp \
    src/widgets/dialogs/brightnessdialog.cpp \
    src/apps/calendarapp.cpp \
    src/widgets/dialogs/reader/highlightslistdialog.cpp \
    src/widgets/dialogs/reader/textdialog.cpp \
    src/widgets/dialogs/wifi/connectiondialog.cpp \
    src/widgets/dialogs/wifi/network.cpp \
    src/widgets/dialogs/wifi/wifilogger.cpp \
    src/widgets/reader/dictionarywidget.cpp \
    src/encfs/encryptionmanager.cpp \
    src/widgets/dialogs/generaldialog.cpp \
    src/homeWidget/homepagewidget.cpp \
    src/widgets/misc/hourglassanimationwidget.cpp \
    src/widgets/dialogs/kobox/koboxappsdialog.cpp \
    src/settings/koboxsettings.cpp \
    src/onlineLibrary/librarywidget.cpp \
    src/localLibrary/locallibrarywidget.cpp \
    src/main.cpp \
    src/homeWidget/mainwindow.cpp \
    src/otaUpdate/otamanager.cpp \
    src/widgets/interfaceWidgets/qclickablelabel.cpp \
    src/widgets/interfaceWidgets/qtooltiplabel.cpp \
    src/quit/quit.cpp \
    src/reader/reader.cpp \
    src/apps/savedwords.cpp \
    src/widgets/text/searchresultswidget.cpp \
    src/settings/settings.cpp \
    src/settings/settingschooser.cpp \
    src/widgets/text/textwidget.cpp \
    src/widgets/interfaceWidgets/toast.cpp \
    src/splash/usbmsSplash.cpp \
    src/apps/userapps.cpp \
    src/widgets/virtualKeyboard/virtualkeyboard.cpp \
    src/widgets/virtualKeyboard/virtualkeypad.cpp \
    src/widgets/dialogs/wifi/wifidialog.cpp \
    src/widgets/dialogs/powerDaemon/sleepdialog.cpp \
    src/powerDaemon/sleepthread.cpp \
    src/settings/powerdaemonsettings.cpp

HEADERS += \
    src/apps/todo.h \
    src/audio/audiothread.h \
    src/splash/alert.h \
    src/apps/apps.h \
    src/widgets/dialogs/audio/audiodialog.h \
    src/widgets/dialogs/audio/audiofile.h \
    src/widgets/dialogs/audio/audiofilequeue.h \
    src/widgets/dialogs/library/bookinfodialog.h \
    src/widgets/dialogs/library/bookoptionsdialog.h \
    src/widgets/dialogs/brightnessdialog.h \
    src/apps/calendarapp.h \
    src/widgets/dialogs/reader/highlightslistdialog.h \
    src/widgets/dialogs/reader/textdialog.h \
    src/widgets/dialogs/wifi/connectiondialog.h \
    src/widgets/dialogs/wifi/network.h \
    src/widgets/dialogs/wifi/wifilogger.h \
    src/widgets/reader/dictionarywidget.h \
    src/encfs/encryptionmanager.h \
    src/functions.h \
    src/widgets/dialogs/generaldialog.h \
    src/homeWidget/homepagewidget.h \
    src/widgets/misc/hourglassanimationwidget.h \
    src/widgets/dialogs/kobox/koboxappsdialog.h \
    src/settings/koboxsettings.h \
    src/onlineLibrary/librarywidget.h \
    src/localLibrary/locallibrarywidget.h \
    src/homeWidget/mainwindow.h \
    src/otaUpdate/otamanager.h \
    src/widgets/interfaceWidgets/qclickablelabel.h \
    src/widgets/interfaceWidgets/qtooltiplabel.h \
    src/quit/quit.h \
    src/reader/reader.h \
    src/apps/savedwords.h \
    src/widgets/text/searchresultswidget.h \
    src/settings/settings.h \
    src/settings/settingschooser.h \
    src/widgets/text/textwidget.h \
    src/widgets/interfaceWidgets/toast.h \
    src/splash/usbmsSplash.h \
    src/apps/userapps.h \
    src/widgets/virtualKeyboard/virtualkeyboard.h \
    src/widgets/virtualKeyboard/virtualkeypad.h \
    src/widgets/dialogs/wifi/wifidialog.h \
    src/widgets/dialogs/powerDaemon/sleepdialog.h \
    src/powerDaemon/sleepthread.h \
    src/settings/powerdaemonsettings.h

FORMS += \
    src/apps/todo.ui \
    src/splash/alert.ui \
    src/apps/apps.ui \
    src/widgets/dialogs/audio/audiodialog.ui \
    src/widgets/dialogs/audio/audiofile.ui \
    src/widgets/dialogs/audio/audiofilequeue.ui \
    src/widgets/dialogs/library/bookinfodialog.ui \
    src/widgets/dialogs/library/bookoptionsdialog.ui \
    src/widgets/dialogs/brightnessdialog.ui \
    src/apps/calendarapp.ui \
    src/widgets/dialogs/reader/highlightslistdialog.ui \
    src/widgets/dialogs/reader/textdialog.ui \
    src/widgets/dialogs/wifi/connectiondialog.ui \
    src/widgets/dialogs/wifi/network.ui \
    src/widgets/dialogs/wifi/wifilogger.ui \
    src/widgets/reader/dictionarywidget.ui \
    src/encfs/encryptionmanager.ui \
    src/widgets/dialogs/generaldialog.ui \
    src/homeWidget/homepagewidget.ui \
    src/widgets/misc/hourglassanimationwidget.ui \
    src/widgets/dialogs/kobox/koboxappsdialog.ui \
    src/settings/koboxsettings.ui \
    src/onlineLibrary/librarywidget.ui \
    src/localLibrary/locallibrarywidget.ui \
    src/homeWidget/mainwindow.ui \
    src/otaUpdate/otamanager.ui \
    src/quit/quit.ui \
    src/reader/reader.ui \
    src/apps/savedwords.ui \
    src/widgets/text/searchresultswidget.ui \
    src/settings/settings.ui \
    src/settings/settingschooser.ui \
    src/widgets/text/textwidget.ui \
    src/widgets/interfaceWidgets/toast.ui \
    src/splash/usbmsSplash.ui \
    src/apps/userapps.ui \
    src/widgets/virtualKeyboard/virtualkeyboard.ui \
    src/widgets/virtualKeyboard/virtualkeypad.ui \
    src/widgets/dialogs/wifi/wifidialog.ui \
    src/widgets/dialogs/powerDaemon/sleepdialog.ui \
    src/settings/powerdaemonsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/eink.qrc

INCLUDEPATH += $$system(find ./ -type d -print -path ./.git -prune | grep -v "./.git")

INCLUDEPATH += $$PWD/libs/libsndfile/include/
DEPENDPATH += $$PWD/libs/libsndfile/include/
LIBS += -L$$PWD/libs/prebuild -lsndfile
