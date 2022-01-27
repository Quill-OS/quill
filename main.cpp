/*
    InkBox: Open-source Qt-based eBook reader
    Copyright (C) 2021-2022 Nicolas Mailloux <nicolecrivain@gmail.com>
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

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRect>
#include <QScreen>

int main(int argc, char *argv[])
{
    setDefaultWorkDir();
    if(checkconfig(".config/18-encrypted_storage/status") == true and checkconfig("/external_root/run/encfs_mounted") == false) {
        // Open Encryption Manager to unlock encrypted storage
        QApplication a(argc, argv);
        encryptionManager w;
        const QScreen * screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();
    }
    else if(checkconfig("/external_root/run/encfs_mounted") == true and checkconfig("/external_root/run/encfs_repack") == true) {
        QApplication a(argc, argv);
        encryptionManager w;
        const QScreen * screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();
    }
    else {
        // Tell scripts that we're currently running
        string_writeconfig("/tmp/inkbox_running", "true");

        // Variables
        global::reader::startBatteryWatchdog = false;
        global::reader::startUsbmsPrompt = false;

        // Checking if battery level is critical; if true (and if it is not charging), then display a "Please charge your eReader" splash and power off.
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val != "emu\n") {
            if(isBatteryCritical() == true) {
                string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
                if(checkconfig_str_val == "Charging\n") {
                    ;
                }
                else {
                    global::battery::showCriticalBatteryAlert = true;
                    QApplication a(argc, argv);
                    alert w;

                    const QScreen* screen = qApp->primaryScreen();
                    w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
                    w.show();
                    return a.exec();
                }
            }
        }

        // Checking if there has been an ALERT flag set up, and if there is, show a big warning
        if(checkconfig("/external_root/boot/flags/ALERT") == true) {
            QApplication a(argc, argv);
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

            string_writeconfig("/inkbox/skip_opendialog", "true");
            string_checkconfig_ro("/opt/inkbox_device");
            if(checkconfig_str_val == "n705\n") {
                global::isN705 = true;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n905\n") {
                global::isN705 = false;
                global::isN905C = true;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n613\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = true;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n873\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = true;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n236\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = true;
            }
            else {
                ;
            }

            QApplication a(argc, argv);
            reader w;

            const QScreen* screen = qApp->primaryScreen();
            w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
            w.show();
            return a.exec();

        }
        else {
            if(checkconfig("/inkbox/bookIsEpub") == true) {
                global::reader::bookIsEpub = true;
            }
            else {
                global::reader::bookIsEpub = false;
            }

            QApplication a(argc, argv);
            MainWindow w;

            QApplication::setStyle("windows");
            QFile stylesheetFile(":/resources/eink.qss");
            stylesheetFile.open(QFile::ReadOnly);
            w.setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();

            string_checkconfig_ro("/opt/inkbox_device");
            if(checkconfig_str_val == "n705\n") {
                global::isN705 = true;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n905\n") {
                global::isN705 = false;
                global::isN905C = true;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n613\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = true;
                global::isN873 = false;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n873\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = true;
                global::isN236 = false;
            }
            else if(checkconfig_str_val == "n236\n") {
                global::isN705 = false;
                global::isN905C = false;
                global::isN613 = false;
                global::isN873 = false;
                global::isN236 = true;
            }
            else {
                ;
            }

            const QScreen * screen = qApp->primaryScreen();
            w.setGeometry(QRect(QPoint(0,0), screen->geometry ().size()));
            w.show();
            return a.exec();
        }
    }
}
