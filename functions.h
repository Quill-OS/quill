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
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>
#include <QDateTime>

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

// WoW, global variables and namespaces are awesome
namespace global {
    namespace battery {
        inline bool showLowBatteryDialog;
        inline bool showCriticalBatteryAlert;
        inline bool batteryAlertLock;
    }
    namespace reader {
        inline QString bookFile;
        inline int pageNumber;
        inline int bookNumber;
        inline bool skipOpenDialog;
        inline bool startBatteryWatchdog;
        inline bool startUsbmsPrompt;
        inline bool bookIsEpub;
        inline bool globalReadingSettings;
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
            inline bool libraryWidgetCreated;
            inline bool libraryWidgetSelected;
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
    namespace keyboard {
        inline bool keyboardDialog;
        inline bool keypadDialog;
        inline bool searchDialog;
        inline bool encfsDialog;
        inline bool vncDialog;
        inline bool wifiPassphraseDialog;
        inline QString keyboardText;
        inline QString keypadText;
    }
    namespace toast {
        inline QString message;
        inline bool wifiToast;
        inline bool modalToast;
        inline bool indefiniteToast;
        inline int delay;
    }
    namespace device {
        inline bool isWifiAble;
    }
    namespace network {
        inline bool isConnected;
    }
    namespace otaUpdate {
        inline bool isUpdateOta;
        inline bool downloadOta;
    }
    namespace encfs {
        inline QString passphrase;
        inline QString unlockTime;
        inline bool cancelSetup;
        inline bool cancelUnlock;
        inline bool lockdown;
        inline bool enableStorageEncryptionDialog;
        inline bool disableStorageEncryptionDialog;
        inline bool errorNoBooksInDropboxDialog;
        inline bool repackDialog;
    }
    namespace library {
        inline unsigned long bookId;
        inline bool isLatestBook;
        inline int latestBookNumber;
        inline QString bookTitle;
        inline bool librarySearchDialog;
        inline bool libraryResults;
    }
    namespace localStorage {
        inline QStringList searchResultsPaths;
    }
    namespace logger {
        inline bool status;
    }
    inline QString systemInfoText;
    inline bool forbidOpenSearchDialog;
    inline bool isN705;
    inline bool isN905C;
    inline bool isN613;
    inline bool isN873;
    inline bool isN236;
    inline bool isN437;
    inline bool isN306;
    inline bool runningInstanceIsReaderOnly;
    inline QString deviceID;
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
    int defaultPdfPageWidth;
    int defaultPdfPageHeight;
    bool checked_box = false;
    void log(QString message, QString className) {
        if(global::logger::status == true) {
            QDebug logger = qDebug();
            logger.noquote();
            logger << QDateTime::currentDateTime().toString("dd/MM/yyyy @ hh:mm:ss") << "|" << className + ":" << message;
        }
    }
    bool checkconfig(QString file) {
        if(QFile::exists(file)) {
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
        }
        else {
            return false;
        }
        return 0;
    };
    bool checkconfig_rw(QString file) {
        if(QFile::exists(file)) {
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
        }
        else {
            return false;
        }
        return 0;
    };
    void setDefaultWorkDir() {
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");
    }
    int brightness_checkconfig(QString file) {
        if(QFile::exists(file)) {
            QFile config(file);
            config.open(QIODevice::ReadWrite);
            QTextStream in (&config);
            const QString content = in.readAll();
            int content_int = content.toInt();
            return content_int;
            config.close();
        }
        else {
           return EXIT_FAILURE;
        }
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
        close(light);
    }
    int int_checkconfig(QString file) {
        if(QFile::exists(file)) {
            QFile int_config(file);
            int_config.open(QIODevice::ReadOnly);
            QString valuestr = int_config.readAll();
            int value = valuestr.toInt();
            int_config.close();
            return value;
        }
        else {
            return EXIT_FAILURE;
        }
        return 0;
    }
    int display_quote() {
        int quoteNumber = QRandomGenerator::global()->bounded(1, 6);
        return quoteNumber;
        return 0;
    }
    void string_writeconfig(std::string file, std::string config_option) {
        std::ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    void string_checkconfig(QString file) {
        if(QFile::exists(file)) {
            checkconfig_str_val = "";
            QFile config(file);
            config.open(QIODevice::ReadWrite);
            QTextStream in (&config);
            checkconfig_str_val = in.readAll();
            config.close();
        }
        else {
            checkconfig_str_val = "";
        }
    }
    void string_checkconfig_ro(QString file) {
        if(QFile::exists(file)) {
            checkconfig_str_val = "";
            QFile config(file);
            config.open(QIODevice::ReadOnly);
            QTextStream in (&config);
            checkconfig_str_val = in.readAll();
            config.close();
        }
        else {
            checkconfig_str_val = "";
        }
    }
    QString readFile(QString file) {
        if(QFile::exists(file)) {
            QFile fileToRead(file);
            fileToRead.open(QIODevice::ReadOnly);
            QTextStream in (&fileToRead);
            QString content = in.readAll();
            return content;
        }
        else {
            return NULL;
        }
    }
    void brightness_writeconfig(int value) {
        std::ofstream fhandler;
        fhandler.open(".config/03-brightness/config");
        fhandler << value;
        fhandler.close();
    }
    void warmth_writeconfig(int value) {
        std::ofstream fhandler;
        fhandler.open(".config/03-brightness/config-warmth");
        fhandler << value;
        fhandler.close();
    }
    int get_brightness() {
        if(global::deviceID == "n613\n") {
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
        if(batt_level_file.exists()) {
            batt_level_file.open(QIODevice::ReadOnly);
            batt_level = batt_level_file.readAll();
            batt_level = batt_level.trimmed();
            batt_level_int = batt_level.toInt();
            batt_level = batt_level.append("%");
            batt_level_file.close();
        }
        else {
            batt_level_int = 100;
            batt_level = "100%";
        }
    }
    void writeconfig(std::string file, std::string config) {
        std::ofstream fhandler;
        fhandler.open(file);
        fhandler << config << std::boolalpha << checked_box << std::endl;
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
        if(global::deviceID != "n613\n") {
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
            proc->waitForFinished();
            proc->deleteLater();
        }
        else {
            QString prog ("/sbin/poweroff");
            QStringList args;
            args << "no_splash";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
            proc->waitForFinished();
            proc->deleteLater();
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
            proc->waitForFinished();
            proc->deleteLater();
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
            proc->waitForFinished();
            proc->deleteLater();
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

        proc->deleteLater();
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

        proc->deleteLater();

        setDefaultWorkDir();
        string_writeconfig("/external_root/run/initrd-fifo", "get_kernel_build_id\n");
        QThread::msleep(100);
        string_writeconfig("/external_root/run/initrd-fifo", "get_kernel_commit\n");
        QThread::msleep(100);

        string_checkconfig_ro("/external_root/run/kernel_build_id");
        QString kernelBuildID = checkconfig_str_val.trimmed();
        kernelVersion.append(", build ");
        kernelVersion.append(kernelBuildID);

        string_checkconfig_ro("/external_root/run/kernel_commit");
        QString kernelCommit = checkconfig_str_val.trimmed();
        kernelVersion.append(", commit ");
        kernelVersion.append(kernelCommit);
    }
    QString getConnectionInformation() {
        QString getIpProg ("sh");
        QStringList getIpArgs;
        if(global::deviceID != "n437\n") {
            getIpArgs << "-c" << "/sbin/ifconfig eth0 | grep 'inet addr' | cut -d: -f2 | awk '{print $1}'";
        }
        else {
            getIpArgs << "-c" << "/sbin/ifconfig wlan0 | grep 'inet addr' | cut -d: -f2 | awk '{print $1}'";
        }
        QProcess *getIpProc = new QProcess();
        getIpProc->start(getIpProg, getIpArgs);
        getIpProc->waitForFinished();

        QString ipAddress = getIpProc->readAllStandardOutput();
        if(ipAddress == "") {
            ipAddress = "Not available";
        }

        getIpProc->deleteLater();
        return ipAddress;
    }
    void getSystemInfo() {
        getUID();
        getKernelVersion();
        global::systemInfoText = "<b>InkBox OS version ";
        string_checkconfig_ro("/external_root/opt/isa/version");
        global::systemInfoText.append(checkconfig_str_val);
        global::systemInfoText.append("</b>");
        global::systemInfoText.append("<br><b>Git:</b> ");
        global::systemInfoText.append(GIT_VERSION);
        global::systemInfoText.append("<br><b>Device UID:</b> ");
        global::systemInfoText.append(deviceUID);
        global::systemInfoText.append("<br><b>Kernel version:</b> ");
        global::systemInfoText.append(kernelVersion);
        global::systemInfoText.append("<br><b>Device:</b> ");
        QString device = global::deviceID.trimmed();
        global::systemInfoText.append(device);
        QString ipAddress = getConnectionInformation();
        global::systemInfoText.append("<br><b>IP address: </b>");
        global::systemInfoText.append(ipAddress);

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
    void defineDefaultPageSize(int fileType) {
        /* fileType can be:
         * 0: ePUB
         * 1: PDF
        */
        if(fileType == 0) {
            if(global::deviceID == "n705\n") {
                defaultEpubPageHeight = 365;
                defaultEpubPageWidth = 365;
            }
            else if(global::deviceID == "n905\n") {
                defaultEpubPageHeight = 425;
                defaultEpubPageWidth = 425;
            }
            else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
                defaultEpubPageHeight = 450;
                defaultEpubPageWidth = 450;
            }
            else if(global::deviceID == "n873\n") {
                defaultEpubPageHeight = 525;
                defaultEpubPageWidth = 525;
            }
        }
        else if(fileType == 1) {
            if(global::deviceID == "n705\n" or global::deviceID == "n905\n") {
                defaultPdfPageHeight = 750;
                defaultPdfPageWidth = 550;
            }
            else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
                defaultPdfPageHeight = 974;
                defaultPdfPageWidth = 708;
            }
            else if(global::deviceID == "n437\n") {
                defaultPdfPageHeight = 1398;
                defaultPdfPageWidth = 1022;
            }
            else if(global::deviceID == "n873\n") {
                defaultPdfPageHeight = 1630;
                defaultPdfPageWidth = 1214;
            }
        }
    }
    void pre_set_brightness(int brightnessValue) {
        if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "n873\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n") {
            set_brightness(brightnessValue);
        }
        else if(global::deviceID == "n613\n") {
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
    bool connectToNetwork(QString essid, QString passphrase) {
        std::string essid_str = essid.toStdString();
        std::string passphrase_str = passphrase.toStdString();
        string_writeconfig("/run/wifi_network_essid", essid_str);
        string_writeconfig("/run/wifi_network_passphrase", passphrase_str);
        string_writeconfig("/opt/ibxd", "connect_to_wifi_network\n");

        int connectionSuccessful = 0;

        while(connectionSuccessful == 0) {
            if(QFile::exists("/run/wifi_connected_successfully")) {
                if(checkconfig("/run/wifi_connected_successfully") == true) {
                    QFile::remove("/run/wifi_connected_successfully");
                    connectionSuccessful = 1;
                    global::network::isConnected = true;
                    setDefaultWorkDir();
                    string_writeconfig(".config/17-wifi_connection_information/essid", essid_str);
                    string_writeconfig(".config/17-wifi_connection_information/passphrase", passphrase_str);
                    return true;
                }
                else {
                    QFile::remove("/run/wifi_connected_successfully");
                    connectionSuccessful = 0;
                    global::network::isConnected = false;
                    return false;
                }
            }
            else {
                QThread::msleep(100);
            }
        }
    }
    int get_warmth() {
        QString sysfsWarmthPath;
        if(global::deviceID == "n873\n") {
            sysfsWarmthPath = "/sys/class/backlight/lm3630a_led/color";
        }
        string_checkconfig_ro(sysfsWarmthPath);
        int warmthValue = checkconfig_str_val.toInt();
        warmthValue = 10 - warmthValue;
        return warmthValue;
    }
    void set_warmth(int warmthValue) {
        // Value 0 gives a warmer lighting than value 10
        warmthValue = 10 - warmthValue;
        std::string warmthValueStr = std::to_string(warmthValue);
        std::string sysfsWarmthPath;
        if(global::deviceID == "n873\n") {
            sysfsWarmthPath = "/sys/class/backlight/lm3630a_led/color";
        }
        string_writeconfig(sysfsWarmthPath, warmthValueStr);
    }
    void installUpdate() {
        string_writeconfig("/mnt/onboard/onboard/.inkbox/can_really_update", "true");
        string_writeconfig("/external_root/opt/update/will_update", "true");
        string_writeconfig("/external_root/boot/flags/WILL_UPDATE", "true");
        reboot(true);
    }
    bool getEncFSStatus() {
        return checkconfig("/external_root/run/encfs_mounted");
    }
    bool isUsbPluggedIn() {
        // Thanks to https://github.com/koreader/KoboUSBMS/blob/2efdf9d920c68752b2933f21c664dc1afb28fc2e/usbms.c#L148-L158
        int ntxfd;
        if((ntxfd = open("/dev/ntx_io", O_RDWR)) == -1) {
                fprintf(stderr, "Error opening ntx_io device\n");
        }
        unsigned long ptr = 0U;
        ioctl(ntxfd, 108, &ptr);
        close(ntxfd);
        return !!ptr;
    }
    int testPing() {
        QString pingProg = "ping";
        QStringList pingArgs;
        pingArgs << "-c" << "1" << "1.1.1.1";
        QProcess *pingProcess = new QProcess();
        pingProcess->start(pingProg, pingArgs);
        pingProcess->waitForFinished();
        int exitCode = pingProcess->exitCode();
        pingProcess->deleteLater();
        if(exitCode == 0) {
            global::network::isConnected = true;
        }
        return exitCode;
    }
}

#endif // FUNCTIONS_H
