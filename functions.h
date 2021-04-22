#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <fstream>
#include <QDir>
#include <QProcess>
#include <regex>

// WoW, static variables and namespaces are awesome
namespace global_static {
    namespace battery {
        static bool showLowBatteryDialog = true;
        static bool showCriticalBatteryAlert = false;
    }
    namespace reader {
        static int pageNumber;
        static bool skipOpenDialog = false;
    }
}

// https://stackoverflow.com/questions/6080853/c-multiple-definition-error-for-global-functions-in-the-header-file/20679534#20679534
namespace {
    QString checkconfig_str_val;
    QString batt_level;
    int batt_level_int;
    bool checked_box = false;
    bool checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        const QString content = in.readAll();
        std::string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
        return 0;
    };
    bool checkconfig_rw(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        std::string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
        return 0;
    };
    int brightness_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        int content_int = content.toInt();
        return content_int;
        config.close();
        return 0;
    };
    int get_brightness() {
        QFile brightness("/var/run/brightness");
        brightness.open(QIODevice::ReadOnly);
        QString valuestr = brightness.readAll();
        int value = valuestr.toInt();
        brightness.close();
        return value;
        return 0;
    }
    void set_brightness(int value) {
        std::ofstream fhandler;
        fhandler.open("/var/run/brightness");
        fhandler << value;
        fhandler.close();
    }
    int int_checkconfig(QString file) {
        QFile int_config(file);
        int_config.open(QIODevice::ReadOnly);
        QString valuestr = int_config.readAll();
        int value = valuestr.toInt();
        int_config.close();
        return value;
        return 0;
    }
    int display_quote() {
        QDir::setCurrent(".config/05-quote");
        QString quote_prog ("sh");
        QStringList quote_args;
        quote_args << "quote.sh";
        QProcess *quote_proc = new QProcess();
        quote_proc->start(quote_prog, quote_args);
        quote_proc->waitForFinished();
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");

        int quote_value = int_checkconfig(".config/05-quote/quote");
        return quote_value;
        return 0;
    }
    void string_writeconfig(std::string file, std::string config_option) {
        std::ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    void string_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void string_checkconfig_ro(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void brightness_writeconfig(int value) {
        std::ofstream fhandler;
        fhandler.open(".config/03-brightness/config");
        fhandler << value;
        fhandler.close();
    }
    void get_battery_level() {
        QFile batt_level_file("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/capacity");
        batt_level_file.open(QIODevice::ReadOnly);
        batt_level = batt_level_file.readAll();
        batt_level = batt_level.trimmed();
        batt_level_int = batt_level.toInt();
        batt_level = batt_level.append("%");
        batt_level_file.close();
    }
    void writeconfig(std::string file, std::string config) {
            std::ofstream fhandler;
            fhandler.open(file);
            fhandler << config << std::boolalpha << checked_box << endl;
            fhandler.close();
    }
    bool checkconfig_match(QString file, std::string pattern) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        std::string contentstr = content.toStdString();

        // Thanks to https://stackoverflow.com/questions/22516463/how-do-i-find-a-complete-word-not-part-of-it-in-a-string-in-c
        std::regex r("\\b" + pattern + "\\b");
        std::smatch m;

        if(std::regex_search(contentstr, m, r)) {
            return true;
        }
        else {
            return false;
        }
        config.close();
        return 0;
    };
    bool isBatteryLow() {
        // Checks if battery level is under 15% of total capacity.
        get_battery_level();
        if(batt_level_int <= 15) {
            return true;
        }
        else {
            return false;
        }
        return 0;
    }
    bool isBatteryCritical() {
        // Checks if the battery level is critical (i.e. <= 5%)
        get_battery_level();
        if(batt_level_int <= 5) {
            return true;
        }
        else {
            return false;
        }
        return 0;
    }
    void poweroff(bool splash) {
        if(splash == true) {
            QString prog ("poweroff");
            QStringList args;
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
        else {
            QString prog ("busybox");
            QStringList args;
            args << "poweroff";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
    }
    void reboot(bool splash) {
        if(splash == true) {
            QString prog ("reboot");
            QStringList args;
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
        else {
            QString prog ("busybox");
            QStringList args;
            args << "reboot";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
    }
}
#endif // FUNCTIONS_H
