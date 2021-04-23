/*
    InkBox: Open-source Qt-based eBook reader
    Copyright (C) 2021 Nicolas Mailloux <nicolecrivain@gmail.com>
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

#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRect>
#include <QScreen>

int main(int argc, char *argv[])
{
    // Checking if battery level is critical; if true (and if it is not charging), then display a "Please charge your eReader" splash and power off.
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

    // Checking if there has been an ALERT flag set up, and if there is, show a big warning
    if(checkconfig("/external_root/boot/flags/ALERT") == true) {
        QApplication a(argc, argv);
        alert w;

        const QScreen* screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();
    }
    // If we're waking from sleep and we have the lockscreen enabled, we'll "resume" the book from scratch
    if(checkconfig("/tmp/suspendBook") == true) {
        // Start the low/critical battery alert timer from the Reader framework since MainWindow is not going to be shown
        global::reader::startBatteryWatchdog = true;
        global::reader::skipOpenDialog = true;

        string_writeconfig("/inkbox/skip_opendialog", "true");
        QApplication a(argc, argv);
        reader w;

        const QScreen* screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry().size()));
        w.show();
        return a.exec();

    }
    else {
        QApplication a(argc, argv);
        MainWindow w;

        QApplication::setStyle("windows");
        QFile stylesheetFile(":/resources/eink.qss");
        stylesheetFile.open(QFile::ReadOnly);
        w.setStyleSheet(stylesheetFile.readAll());
        stylesheetFile.close();

        const QScreen* screen = qApp->primaryScreen();
        w.setGeometry(QRect(QPoint(0,0), screen->geometry ().size()));
        w.show();
        return a.exec();
    }
}
