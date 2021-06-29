#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <fstream>
#include <QDir>
#include <QProcess>
#include <regex>
#include <QThread>

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// WoW, global variables and namespaces are awesome
namespace global {
    namespace battery {
        inline bool showLowBatteryDialog;
        inline bool showCriticalBatteryAlert;
        inline bool batteryAlertLock;
    }
    namespace reader {
        inline int pageNumber;
        inline int bookNumber;
        inline bool skipOpenDialog;
        inline bool startBatteryWatchdog;
        inline bool startUsbmsPrompt;
        inline bool bookIsEpub;
    }
    namespace kobox {
        inline bool showKoboxSplash;
        inline bool koboxSettingsRebootDialog;
        inline bool resetKoboxUserDataBool;
        inline bool resetKoboxDialog;
    }
    namespace mainwindow {
        namespace tabSwitcher {
            inline bool repaint;
            inline bool appsWidgetCreated;
            inline bool appsWidgetSelected;
            inline bool settingsChooserWidgetCreated;
            inline bool settingsChooserWidgetSelected;
        }
        inline bool updateDialog;
        inline bool lowBatteryDialog;
    }
    namespace usbms {
        inline bool usbmsDialog;
        inline bool showUsbmsDialog;
        inline bool launchUsbms;
        inline bool koboxExportExtensions;
    }
    namespace settings {
        inline bool settingsRebootDialog;
    }
    namespace text {
        inline bool textBrowserDialog;
        inline QString textBrowserContents;
    }
    inline QString systemInfoText;
    inline bool isN705;
    inline bool isN905C;
    inline bool isN613;
}

// https://stackoverflow.com/questions/6080853/c-multiple-definition-error-for-global-functions-in-the-header-file/20679534#20679534
namespace {
    QString checkconfig_str_val;
    QString deviceUID;
    QString device;
    QString batt_level;
    QString kernelVersion;
    int batt_level_int;
    int defaultEpubPageWidth;
    int defaultEpubPageHeight;
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
    void setDefaultWorkDir() {
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");
    }
    int brightness_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        int content_int = content.toInt();
        return content_int;
        config.close();
        return 0;
    }
    void set_brightness(int value) {
        std::ofstream fhandler;
        fhandler.open("/var/run/brightness");
        fhandler << value;
        fhandler.close();
    }
    void set_brightness_ntxio(int value) {
        // Thanks to Kevin Short for this (GloLight)
        int light;
        if((light = open("/dev/ntx_io", O_RDWR)) == -1) {
                fprintf(stderr, "Error opening ntx_io device\n");
        }
        ioctl(light, 241, value);
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
        checkconfig_str_val = "";
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void string_checkconfig_ro(QString file) {
        checkconfig_str_val = "";
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
    int get_brightness() {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n613") {
            string_checkconfig_ro(".config/03-brightness/config");
            int brightness;
            if(checkconfig_str_val == "") {
                brightness = 0;
            }
            else {
                brightness = checkconfig_str_val.toInt();
            }
            return brightness;
        }
        else {
            QFile brightness("/var/run/brightness");
            brightness.open(QIODevice::ReadOnly);
            QString valuestr = brightness.readAll();
            int value = valuestr.toInt();
            brightness.close();
            return value;
        }
        return 0;
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
    void zeroBrightness() {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val != "n613\n") {
            set_brightness(0);
        }
        else {
            set_brightness_ntxio(0);
        }
    }
    void poweroff(bool splash) {
        if(splash == true) {
            QString prog ("/sbin/poweroff");
            QStringList args;
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
        else {
            QString prog ("/sbin/poweroff");
            QStringList args;
            args << "no_splash";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
    }
    void reboot(bool splash) {
        if(splash == true) {
            QString prog ("/sbin/reboot");
            QStringList args;
            if(global::kobox::resetKoboxUserDataBool == true) {
                args << "splash" << "reset_kobox";
            }
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
        else {
            QString prog ("/sbin/reboot");
            QStringList args;
            if(global::kobox::resetKoboxUserDataBool == true) {
                args << "no_splash" << "reset_kobox";
            }
            else {
                args << "no_splash";
            }
            QProcess *proc = new QProcess();
            proc->start(prog, args);
        }
    }
    void getUID() {
        QString prog ("dd");
        QStringList args;
        args << "if=/dev/mmcblk0" << "bs=512" << "skip=1" << "count=1" << "status=none";
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        deviceUID = proc->readAllStandardOutput();
        deviceUID = deviceUID.left(256);
    }
    void getKernelVersion() {
        QString prog ("uname");
        QStringList args;
        args << "-r";
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        kernelVersion = proc->readAllStandardOutput();
        kernelVersion = kernelVersion.trimmed();

        setDefaultWorkDir();
        QString fifo_prog("sh");
        QStringList fifo_args;
        fifo_args << "get_kernel_build_id.sh";
        QProcess *fifo_proc = new QProcess();
        fifo_proc->start(fifo_prog, fifo_args);
        fifo_proc->waitForFinished();
        QThread::msleep(100);

        string_checkconfig_ro("/external_root/run/build_id");
        QString kernelBuildID = checkconfig_str_val.trimmed();
        kernelVersion.append(", build ");
        kernelVersion.append(kernelBuildID);
    }
    void getSystemInfo() {
        getUID();
        getKernelVersion();
        global::systemInfoText = "<b>InkBox OS version ";
        string_checkconfig_ro("/external_root/opt/isa/version");
        global::systemInfoText.append(checkconfig_str_val);
        global::systemInfoText.append("</b>");
        global::systemInfoText.append("<br><b>Device UID:</b> ");
        global::systemInfoText.append(deviceUID);
        global::systemInfoText.append("<br><b>Kernel version:</b> ");
        global::systemInfoText.append(kernelVersion);
        global::systemInfoText.append("<br><b>Device:</b> ");
        string_checkconfig_ro("/opt/inkbox_device");
        QString device = checkconfig_str_val.trimmed();
        global::systemInfoText.append(device);
    }
    void resetKoboxUserData() {
        global::kobox::resetKoboxUserDataBool = true;
        reboot(true);
    }
    QString findEpubMetadata(QString book_file, QString metadata) {
        setDefaultWorkDir();
        QString prog ("sh");
        QStringList args;
        args << "find_epub_metadata.sh" << book_file << metadata;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        QString returnedMetadata = proc->readAllStandardOutput();
        return returnedMetadata;
    }
    void defineDefaultPageSize() {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            defaultEpubPageWidth = 365;
            defaultEpubPageHeight = 365;
        }
        if(checkconfig_str_val == "n905\n") {
            defaultEpubPageHeight = 425;
            defaultEpubPageWidth = 425;
        }
        if(checkconfig_str_val == "n613\n") {
            defaultEpubPageHeight = 450;
            defaultEpubPageWidth = 450;
        }
    }
    void pre_set_brightness(int brightnessValue) {
        string_checkconfig_ro("/opt/inkbox_device");

        if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
            set_brightness(brightnessValue);
        }
        else if(checkconfig_str_val == "n613\n") {
            set_brightness_ntxio(brightnessValue);
        }
        else {
            set_brightness(brightnessValue);
        }
    }
    void cinematicBrightness(int value, int mode) {
        /* mode can be 0 or 1, respectively
         * 0: Bring UP brightness
         * 1: Bring DOWN brightness
        */
        if(mode == 0) {
            int brightness = 0;
            while(brightness != value) {
                brightness = brightness + 1;
                pre_set_brightness(brightness);
                QThread::msleep(30);
            }
        }
        else {
            int brightness = get_brightness();
            while(brightness != 0) {
                brightness = brightness - 1;
                pre_set_brightness(brightness);
                QThread::msleep(30);
            }
        }
    }
}
#endif // FUNCTIONS_H
