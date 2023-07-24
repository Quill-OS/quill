/*
    InkBox: Open-source Qt-based eBook reader
    Copyright (C) 2021-2023 Nicolas Mailloux <nicolecrivain@gmail.com>
    SPDX-License-Identifier: GPL-3.0-only

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "alert.h"
#include "generaldialog.h"
#include "functions.h"
#include "reader.h"
#include "encryptionmanager.h"
#include "sleepthread.h"
#include "sleepdialog.h"
#include "audiothread.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QRect>
#include <QScreen>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    if(char * debug = std::getenv("DEBUG")) {
        if(std::atoi(debug) == 1) {
            global::logger::status = true;
        }
    }

    global::deviceID = readFile("/opt/inkbox_device");
    log("Running on device " + global::deviceID, "main", true);

    // Tell the OS that we're currently running
    writeFile("/tmp/inkbox_running", "true");

    setDefaultWorkDir();
    if(checkconfig("/run/wifi_able") == true) {
        log("Device has Wi-Fi capabilities", "main");
        global::device::isWifiAble = true;
    }
    else {
        log("Device does not have Wi-Fi capabilities", "main");
        global::device::isWifiAble = false;
    }

    if(QFile::exists("/tmp/currentlyRunningUserApplication")) {
        QFile::remove("/tmp/currentlyRunningUserApplication");
    }

    if(QFile::exists("/tmp/rescan_userapps")) {
        QFile::remove("/tmp/rescan_userapps");
        log("Re-scanning user applications from explicit request", "main");
        writeFile("/opt/ibxd", "gui_apps_stop\n");
        while(true) {
            if(QFile::exists("/tmp/gui_apps_stopped")) {
                log("there");
                QFile::remove("/tmp/gui_apps_stopped");
                break;
            }
        }
        writeFile("/opt/ibxd", "gui_apps_start\n");
        while(true) {
            if(QFile::exists("/tmp/gui_apps_started")) {
                if(checkconfig("/tmp/gui_apps_started") == true) {
                    log("GUI apps service started successfully", "main");
                    QFile::remove("/tmp/gui_apps_started");
                    updateUserAppsMainJsonFile();
                    break;
                }
                else {
                    log("GUI apps service failed to start", "main");
                    QFile::remove("/tmp/gui_apps_started");
                    break;
                }
            }
        }
        updateUserAppsMainJsonFile();
    }

    // Power daemon-related stuff
    sleepDialog * sleepDialogWindow = new sleepDialog;
    // https://wiki.qt.io/QThreads_general_usage
    QThread * IPDThread = new QThread();
    sleepThread * sleepThreadWindow = new sleepThread();
    sleepThreadWindow->moveToThread(IPDThread);
    QObject::connect(IPDThread, &QThread::started, sleepThreadWindow, &sleepThread::start);
    QObject::connect(sleepThreadWindow, &sleepThread::startDialog, sleepDialogWindow, &sleepDialog::launchSleepDialog);
    QObject::connect(sleepThreadWindow, &sleepThread::stopDialog, sleepDialogWindow, &sleepDialog::hideSleepDialog);
    IPDThread->start();

    // Audio stuff
    if(checkconfig(".config/e-2-audio/enabled") == true) {
        log("Audio is enabled", "main");
        global::audio::enabled = true;
        QThread * audioThread = new QThread();
        audiothread * audioObject = new audiothread();
        audioObject->moveToThread(audioThread);
        QObject::connect(audioThread, &QThread::started, audioObject, &audiothread::start);
        audioThread->start();
    }
    else {
        log("Audio is disabled", "main");
    }

    if(checkconfig(".config/18-encrypted_storage/status") == true and checkconfig("/external_root/run/encfs_mounted") == false) {
        // Open Encryption Manager to unlock encrypted storage
        encryptionManager w;
        const QScreen * screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();
    }
    else if(checkconfig("/external_root/run/encfs_mounted") == true and checkconfig("/external_root/run/encfs_repack") == true) {
        log("Launching encryptionManager", "main");
        encryptionManager w;
        const QScreen * screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();
    }
    else {
        // Variables
        global::reader::startBatteryWatchdog = false;
        global::reader::startUsbmsPrompt = false;

        // Checking if battery level is critical; if true (and if it is not charging), then display a "Please charge your eReader" splash and power off.
        if(global::deviceID != "emu\n") {
            if(isBatteryCritical() == true) {
                if(!isUsbPluggedIn()) {
                    global::battery::showCriticalBatteryAlert = true;
                    alert w;

                    const QScreen * screen = qApp->primaryScreen();
                    w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
                    w.show();
                    return a.exec();
                }
            }
        }

        // Checking if there has been an ALERT flag set up, and if there is, show a big warning
        if(checkconfig("/external_root/boot/flags/ALERT") == true) {
            alert w;

            const QScreen * screen = qApp->primaryScreen();
            w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
            w.show();
            return a.exec();
        }
        // If we're waking from sleep and we have the lockscreen enabled, we'll "resume" the book from scratch
        else if(checkconfig("/tmp/suspendBook") == true && checkconfig("/inkbox/bookIsEpub") == false) {
            // Start the low/critical battery alert timer from the Reader framework since MainWindow is not going to be shown
            global::runningInstanceIsReaderOnly = true;
            global::reader::startBatteryWatchdog = true;
            global::reader::startUsbmsPrompt = true;
            global::reader::skipOpenDialog = true;

            writeFile("/inkbox/skip_opendialog", "true");
            if(global::deviceID == "n705\n") {
                global::isN705 = true;
            }
            else if(global::deviceID == "n905\n") {
                global::isN905C = true;
            }
            else if(global::deviceID == "n613\n") {
                global::isN613 = true;
            }
            else if(global::deviceID == "n873\n") {
                global::isN873 = true;
            }
            else if(global::deviceID == "n236\n") {
                global::isN236 = true;
            }
            else if(global::deviceID == "n437\n") {
                global::isN437 = true;
            }
            else if(global::deviceID == "n306\n") {
                global::isN306 = true;
            }
	    else if(global::deviceID == "n249\n") {
                global::isN249 = true;
            }
            else if(global::deviceID == "kt\n") {
                global::isKT = true;
            }
            else {
                ;
            }

            reader w;

            const QScreen* screen = qApp->primaryScreen();
            w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
            w.show();
            return a.exec();

        }
        else {
            QProcess::execute("remount_tmpfs_launch.sh", QStringList());

            MainWindow w;

            QApplication::setStyle("windows");
            QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
            stylesheetFile.open(QFile::ReadOnly);
            w.setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();

            if(global::deviceID == "n705\n") {
                global::isN705 = true;
            }
            else if(global::deviceID == "n905\n") {
                global::isN905C = true;
            }
            else if(global::deviceID == "n613\n") {
                global::isN613 = true;
            }
            else if(global::deviceID == "n873\n") {
                global::isN873 = true;
            }
            else if(global::deviceID == "n236\n") {
                global::isN236 = true;
            }
            else if(global::deviceID == "n437\n") {
                global::isN437 = true;
            }
	    else if(global::deviceID == "n306\n") {
                global::isN306 = true;
            }
	    else if(global::deviceID == "n249\n") {
                global::isN249 = true;
            }
            else if(global::deviceID == "kt\n") {
                global::isKT = true;
            }
            else {
                ;
            }

            const QScreen * screen = qApp->primaryScreen();
            w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
            w.show();
            return a.exec();
        }
    }
}
