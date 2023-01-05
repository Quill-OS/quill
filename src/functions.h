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
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QMutex>

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
        inline int pdfOrientation;
        inline bool highlightAlreadyDone;
        inline int textAlignment;
        inline int lineSpacing;
        inline QString font;
        static inline int initialFontSize = 6;
        inline int fontSize;
        inline int margins;
        inline QString currentViewportText;
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
            inline bool homePageWidgetCreated;
            inline bool appsWidgetCreated;
            inline bool appsWidgetSelected;
            inline bool settingsChooserWidgetCreated;
            inline bool settingsChooserWidgetSelected;
            inline bool libraryWidgetCreated;
            inline bool libraryWidgetSelected;
            inline bool localLibraryWidgetCreated;
            inline bool localLibraryWidgetSelected;
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
        inline QString textBrowserTitle = ""; // At default empty, so "Information" is displayed
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
        inline bool embed = true;
    }
    namespace toast {
        inline QString message;
        inline bool modalToast;
        inline bool indefiniteToast;
        inline int delay;
    }
    namespace device {
        inline bool isWifiAble;
    }
    namespace otaUpdate {
        inline bool isUpdateOta;
        inline bool downloadOta;
    }
    namespace encfs {
        inline QString passphrase;
        inline QString unlockTime;
        inline QString lockdownMessage;
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
    namespace bookInfoDialog {
        inline bool localInfoDialog;
    }
    namespace localLibrary {
        static inline QString rawDatabasePath = "/inkbox/LocalLibrary.db.raw";
        static inline QString databaseDirectoryPath = "/mnt/onboard/onboard/.database/";
        static inline QString databasePath = databaseDirectoryPath + "LocalLibrary.db";
        static inline QString recentBooksDatabasePath = databaseDirectoryPath + "RecentBooks.db";
        static inline QString pinnedBooksDatabasePath = databaseDirectoryPath + "PinnedBooks.db";
        static inline QString highlightsDatabasePath = databaseDirectoryPath + "Highlights.db";
        static inline QString todoDatabasePath = databaseDirectoryPath + "ToDo.db";
        // Maximum signed integer value for 32-bit systems
        static inline int folderID = 2147483647;
        inline bool headless;
        namespace bookOptionsDialog {
            inline int bookID;
            inline bool deleteOption = true;
            inline bool bookDeleted;
            inline bool bookPinAction;
            inline QString folderPath;
            inline bool isFolder = false;
        }
    }
    namespace localStorage {
        inline QStringList searchResultsPaths;
    }
    namespace logger {
        inline bool status;
    }
    namespace userApps {
        inline bool appCompatibilityDialog;
        inline QString appCompatibilityText;
        inline bool appCompatibilityLastContinueStatus = true; // This is for RequiredFeatures to show only one dialog if 'Cancel' is clicked.
        inline bool appInfoDialog;
        inline bool launchApp;
    }
    namespace homePageWidget {
        static inline int recentBooksNumber = 8;
        static inline int recentBooksNumberPerRow = 4;
        static inline int recentBooksRowNumber = global::homePageWidget::recentBooksNumber / global::homePageWidget::recentBooksNumberPerRow;
        static inline int pinnedBooksNumber = 4;
        static inline int pinnedBooksNumberPerRow = 4;
        static inline int pinnedBooksRowNumber = global::homePageWidget::pinnedBooksNumber / global::homePageWidget::pinnedBooksNumberPerRow;
    }
    namespace highlightsListDialog {
        inline QString bookPath;
    }
    namespace wifi {
        enum class wifiState {
            configured,
            enabled,
            disabled,
            unknown, // To not confuse lastWifiState
        };
        inline bool isConnected;
        class wifiNetworkData {
        public:
            QString mac;
            QString name;
            bool encryption;
            int signal;
        };
    }
    namespace audio {
        inline bool enabled = false;
        struct musicFile {
            QString path;
            QString name; // Cutted path for easier use in names
            int lengths;
            QString length; // In minutes:seconds
        };
        enum class Action { // Function will be called with this enum
            Play,
            Next,
            Previous,
            Pause,
            Continue,
            Stop, // Sets paused to false, isSomethingCurrentlyPlaying to false, and itemCurrentlyPLaying to -1, also stops playing
            None,
            SetVolume,
        };
        inline Action currentAction = Action::None;
        inline bool actionDone = false; // bool to await for answer
        inline QVector<musicFile> queue;
        inline QVector<musicFile> fileList;
        inline int itemCurrentlyPLaying = 0; // Also indicates in the queue menu which a grey color which is playing
        inline QMutex audioMutex; // These variables will be shared between threads, so here its to protect it
        inline int progressSeconds = 0;
        inline bool paused = false;
        inline bool isSomethingCurrentlyPlaying = false;
        inline bool firstScan = true;
        inline int volumeLevel = 100;
    }
    inline QString systemInfoText;
    inline bool forbidOpenSearchDialog;
    inline bool isN705 = false;
    inline bool isN905C = false;
    inline bool isN613 = false;
    inline bool isN873 = false;
    inline bool isN236 = false;
    inline bool isN437 = false;
    inline bool isN306 = false;
    inline bool isKT = false;
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
    QFile logFile("/external_root/var/log/inkbox-gui.log");
    void log(QString message, QString className = "undefined", bool applicationStart = false) {
        if(global::logger::status == true) {
            QString initialTime;
            if(applicationStart == true) {
                initialTime = QDateTime::currentDateTime().toString("dd/MM/yyyy @ hh:mm:ss");
            }
            QDebug logger = qDebug();
            logger.noquote();

            QStringList logStringList;
            logStringList << QDateTime::currentDateTime().toString("dd/MM/yyyy @ hh:mm:ss") << "|" << className + ":" << message.trimmed();
            QString logString = logStringList.join(" ");

            logger << logString;
            if(!logFile.isOpen()) {
                logFile.open(QIODevice::Append|QIODevice::Text);
            }
            QTextStream logFileOut(&logFile);
            if(applicationStart == true) {
                logFileOut << "========== InkBox binary start at " << initialTime << " ==========" << Qt::endl;
            }
            logFileOut << logString << Qt::endl;
            logFile.close();
        }
    }
    void logEnabled(QString configOption, QString className) {
        log("Enabling " + configOption + " setting", className);
    }
    void logDisabled(QString configOption, QString className) {
        log("Disabling " + configOption + " setting", className);
    }
    // Rewrited this function
    // 1 if it returns it doesnt closes the file
    // 2 just use qt functions
    // ~Szybet
    bool checkconfig(QString file) {
        if(QFile::exists(file)) {
            QFile config(file);
            config.open(QIODevice::ReadOnly);
            QTextStream in (&config);
            QString content = in.readAll();
            config.close();
            if(content.contains("true") == true) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            log("WARNING: File " + file + " doesn't exist, returning false", "functions");
            return false;
        }
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
        if(QFile::exists("/var/run/brightness")) {
            std::ofstream fhandler;
            fhandler.open("/var/run/brightness");
            fhandler << value;
            fhandler.close();
        }
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
    bool writeFile(QString filename, QString content) {
        QFile file(filename);
        if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << content;
            return true;
        }
        else {
            QString function = __func__; log(function + ": Failed to write string '" + content + "' to file '" + filename + "'", "functions");
            return false;
        }
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
            if(QFile::exists("/var/run/brightness")) {
                QFile brightness("/var/run/brightness");
                brightness.open(QIODevice::ReadOnly);
                QString valuestr = brightness.readAll();
                int value = valuestr.toInt();
                brightness.close();
                return value;
            }
            else {
                return 0;
            }
        }
        return 0;
    }
    void get_battery_level() {
        QString batteryLevelFileQstr;
        if(global::deviceID == "kt\n") {
            QFile batt_level_file("/sys/devices/system/yoshi_battery/yoshi_battery0/battery_capacity");
            if(batt_level_file.exists()) {
                batt_level_file.open(QIODevice::ReadOnly);
                batt_level = batt_level_file.readAll();
                batt_level = batt_level.trimmed();
                batt_level_int = batt_level.toInt();
                batt_level = batt_level.append("%");
                batt_level_file.close();
            }
        }
        else {
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
            QString function = __func__; log(function + ": Battery is at a critical charge level!", "functions");
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
        log("Powering off", "functions");
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
        log("Rebooting", "functions");
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
        global::systemInfoText.append("</b><br>Copyright <font face='Inter'>©</font> 2021-2022 Nicolas Mailloux and contributors");
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
        log("Resetting KoBox user data", "functions");
        global::kobox::resetKoboxUserDataBool = true;
        reboot(true);
    }
    QString findEpubMetadata(QString book_file, QString metadata) {
        log("Finding ePUB metadata, query: " + metadata, "functions");
        setDefaultWorkDir();
        QString prog ("sh");
        QStringList args;
        args << "find_epub_metadata.sh" << book_file << metadata;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        QString returnedMetadata = proc->readAllStandardOutput();
        QString function = __func__; log(function + ": ePUB metadata is: " + returnedMetadata, "functions");
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
            else if(global::deviceID == "n905\n" or global::deviceID == "kt\n") {
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
            QString function = __func__;
            log(function + ": Defined default ePUB page height to " + QString::number(defaultEpubPageHeight), "functions");
            log(function + ": Defined default ePUB page width to " + QString::number(defaultEpubPageWidth), "functions");
        }
        else if(fileType == 1) {
            if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
                if(global::reader::pdfOrientation == 0) {
                    defaultPdfPageHeight = 750;
                    defaultPdfPageWidth = 550;
                }
                else {
                    defaultPdfPageHeight = 550;
                    defaultPdfPageWidth = 750;
                }
            }
            else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
                if(global::reader::pdfOrientation == 0) {
                    defaultPdfPageHeight = 974;
                    defaultPdfPageWidth = 708;
                }
                else {
                    defaultPdfPageHeight = 708;
                    defaultPdfPageWidth = 974;
                }
            }
            else if(global::deviceID == "n437\n") {
                if(global::reader::pdfOrientation == 0) {
                    defaultPdfPageHeight = 1398;
                    defaultPdfPageWidth = 1022;
                }
                else {
                    defaultPdfPageHeight = 1022;
                    defaultPdfPageWidth = 1398;
                }
            }
            else if(global::deviceID == "n873\n") {
                if(global::reader::pdfOrientation == 0) {
                    defaultPdfPageHeight = 1630;
                    defaultPdfPageWidth = 1214;
                }
                else {
                    defaultPdfPageHeight = 1214;
                    defaultPdfPageWidth = 1630;
                }
            }
            QString function = __func__;
            log(function + "Defined default PDF page height to " + QString::number(defaultPdfPageHeight), "functions");
            log(function + "Defined default PDF page width to " + QString::number(defaultPdfPageWidth), "functions");
        }
    }
    void pre_set_brightness(int brightnessValue) {
        if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "n873\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "kt\n") {
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
        /* mode can be 0, 1, or 2, respectively:
         * 0: Bring UP brightness
         * 1: Bring DOWN brightness
         * 2: Auto; smooth brightness transition between two brightness levels
        */
        if(global::deviceID != "n705\n" && global::deviceID != "n905\n" && global::deviceID != "kt\n") {
            QString function = __func__; log(function + ": Setting brightness to " + QString::number(value), "functions");
        }
        if(mode == 0) {
            int brightness = 0;
            while(brightness != value) {
                brightness = brightness + 1;
                pre_set_brightness(brightness);
                QThread::msleep(30);
            }
        }
        else if(mode == 1) {
            int brightness = get_brightness();
            while(brightness != 0) {
                brightness = brightness - 1;
                pre_set_brightness(brightness);
                QThread::msleep(30);
            }
        }
        else if(mode == 2) {
            int brightness = get_brightness();
            if(brightness <= value) {
                while(brightness != value) {
                    brightness = brightness + 1;
                    pre_set_brightness(brightness);
                    QThread::msleep(30);
                }
            }
            else if(brightness >= value) {
                while(brightness != value) {
                    brightness = brightness - 1;
                    pre_set_brightness(brightness);
                    QThread::msleep(30);
                }
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
        log("Installing update package", "functions");
        writeFile("/mnt/onboard/onboard/.inkbox/can_really_update", "true\n");
        writeFile("/external_root/opt/update/will_update", "true\n");
	writeFile("/external_root/boot/flags/WILL_UPDATE", "true\n");
        reboot(true);
    }
    bool getEncFSStatus() {
        return checkconfig("/external_root/run/encfs_mounted");
    }
    bool isUsbPluggedIn() {
        if(global::deviceID == "kt\n") {
            if(readFile("/sys/devices/system/yoshi_battery/yoshi_battery0/battery_status") == "1\n") {
                return 1;
            }
            else {
                return 0;
            }
        }
        else {
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
    }
    void updateUserAppsMainJsonFile() {
        QDirIterator appsDir("/mnt/onboard/onboard/.apps", QDirIterator::NoIteratorFlags);
        QFile newJsonFile = QFile{"/mnt/onboard/onboard/.apps/apps.json"};
        QJsonDocument newJsonDocument;
        QJsonArray array;

        while (appsDir.hasNext())
        {
            QDir dir(appsDir.next());
            if(dir.exists() == true) {
                if(dir.path().split("/").last().contains(".") == false) {
                    QFile jsonSmall = QFile{dir.path() + "/app.json"};
                    if(jsonSmall.exists() == true) {
                        jsonSmall.open(QIODevice::ReadOnly | QIODevice::Text);
                        QString jsonString = jsonSmall.readAll();
                        jsonSmall.close();

                        QJsonDocument jsonSmallDoc = QJsonDocument::fromJson(jsonString.toUtf8());
                        if(jsonSmallDoc["app"].isObject() == true) {
                            QJsonObject jsonSmallMainObj = jsonSmallDoc["app"].toObject();
                            array.append(jsonSmallMainObj);

                        }
                        else {
                            log("Error: User application '" + appsDir.path() + "''s JSON file descriptor is missing main object 'app'", "main");
                        }

                    }
                    else {
                        QString message = "User application '" + appsDir.path() + "' does not contain any 'app.json' file: ";
                        message.append(jsonSmall.fileName());
                        log(message, "main");
                    }
                }
            }
        }
        // https://forum.qt.io/topic/104791/how-i-can-create-json-format-in-qt/5
        QJsonObject root;
        root["list"] = array;
        newJsonDocument.setObject(root);

        newJsonFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        newJsonFile.write(newJsonDocument.toJson());
        newJsonFile.flush();
        newJsonFile.close();
    }
    void updateUserAppsSmallJsonFiles() {
        QFile jsonFile = QFile{"/mnt/onboard/onboard/.apps/apps.json"};

        jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString fileRead = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

        if(jsonDocument["list"].isArray() == true) {
            QJsonArray jsonArray = jsonDocument["list"].toArray();
            for(QJsonValueRef refJsonObject: jsonArray) {
                QJsonObject jsonMainObject = refJsonObject.toObject();
                QString appName = jsonMainObject["Name"].toString();

                // This needs to be here and not at the beggining of this function because it is an iterator
                QDirIterator appsDir("/mnt/onboard/onboard/.apps", QDirIterator::NoIteratorFlags);
                while (appsDir.hasNext()) {
                    QDir dir(appsDir.next());
                    if(dir.exists() == true) {
                        if(dir.path().split("/").last().toLower().contains(appName.toLower()) == true) {
                            QJsonObject root;
                            root["app"] = refJsonObject.toObject();;
                            QJsonDocument newJsonDocument;
                            newJsonDocument.setObject(root);

                            QFile newSmallJson = QFile{dir.path() + "/" + "app.json"};

                            newSmallJson.open(QIODevice::ReadWrite);
                            QTextStream stream(&newSmallJson);
                            stream << newJsonDocument.toJson() << Qt::endl;
                            newSmallJson.flush();
                            newSmallJson.close();
                        }
                    }
                }
            }
        }
    }
    QString fileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm) {
        QFile f(fileName);
        if (f.open(QFile::ReadOnly)) {
            QCryptographicHash hash(hashAlgorithm);
            if (hash.addData(&f)) {
                return hash.result().toHex();
            }
        }
    }
    QJsonObject getBookMetadata(int bookID) {
        // Read library database from file
        QFile database(global::localLibrary::databasePath);
        QByteArray data;
        if(database.open(QIODevice::ReadOnly)) {
            data = database.readAll();
            database.close();
        }
        else {
            QString function = __func__; log(function + ": Failed to open local library database file for reading at '" + database.fileName() + "'", "functions");
        }

        // Parse JSON data
        QJsonObject jsonObject = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data))).object();
        QJsonArray jsonArrayList = jsonObject["database"].toArray();
        return jsonArrayList.at(bookID - 1).toObject();
    }
    QJsonObject readHighlightsDatabase() {
        // Read highlights database from file
        QFile database(global::localLibrary::highlightsDatabasePath);
        QByteArray data;
        if(database.open(QIODevice::ReadOnly)) {
            data = database.readAll();
            database.close();
        }
        else {
            QString function = __func__; log(function + ": Failed to open highlights database file for reading at '" + database.fileName() + "'", "functions");
        }

        // Parse JSON data
        return QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data))).object();
    }
    void writeHighlightsDatabase(QJsonObject jsonObject) {
        QFile::remove(global::localLibrary::highlightsDatabasePath);
        writeFile(global::localLibrary::highlightsDatabasePath, qCompress(QJsonDocument(jsonObject).toJson()).toBase64());
    }
    QJsonDocument readTodoDatabase() {
        // Read To-Do database from file
        QFile database(global::localLibrary::todoDatabasePath);
        QByteArray data;
        if(database.open(QIODevice::ReadOnly)) {
            data = database.readAll();
            database.close();
        }
        else {
            QString function = __func__; log(function + ": Failed to open To-Do database file for reading at '" + database.fileName() + "'", "functions");
        }

        // Parse JSON data
        return QJsonDocument::fromJson(data);
    }
    void writeTodoDatabase(QJsonDocument jsonDocument) {
        QFile::remove(global::localLibrary::todoDatabasePath);
        writeFile(global::localLibrary::todoDatabasePath, jsonDocument.toJson());
    }
    void highlightBookText(QString text, QString bookPath, bool remove) {
        if(remove == false) {
            if(!QFile::exists(global::localLibrary::highlightsDatabasePath)) {
                QJsonObject mainJsonObject;
                QJsonObject firstJsonObject;
                firstJsonObject.insert("BookPath", QJsonValue(bookPath));
                firstJsonObject.insert("T-" + QUuid::createUuid().toString(QUuid::WithoutBraces), QJsonValue(text));
                mainJsonObject["Book1"] = firstJsonObject;
                writeHighlightsDatabase(mainJsonObject);
            }
            else {
                QJsonObject jsonObject = readHighlightsDatabase();
                bool highlightWrote = false;
                int length = jsonObject.length();
                for(int i = 1; i <= length; i++) {
                    if(jsonObject["Book" + QString::number(i)].toObject().value("BookPath").toString() == bookPath) {
                        log("highlightBookText: Found existing book in database with path '" + bookPath + "'", "functions");

                        // Insert highlight
                        QJsonObject highlightJsonObject = jsonObject["Book" + QString::number(i)].toObject();
                        // Finding available slot for highlight in case the one we are looking for is already occupied
                        QString uuid = "T-"+ QUuid::createUuid().toString(QUuid::WithoutBraces);
                        if(highlightJsonObject.contains(uuid)) {
                            while(true) {
                                if(highlightJsonObject.contains(uuid)) {
                                    uuid = "T-" + QUuid::createUuid().toString(QUuid::WithoutBraces);
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        highlightJsonObject.insert(uuid, text);
                        jsonObject["Book" + QString::number(i)] = highlightJsonObject;

                        writeHighlightsDatabase(jsonObject);
                        highlightWrote = true;
                    }
                }

                if(highlightWrote == false) {
                    // This block of code is called when the book is referenced in the database, but no highlights are currently indexed
                    QJsonObject bookJsonObject;
                    bookJsonObject.insert("BookPath", QJsonValue(bookPath));
                    bookJsonObject.insert("T-" + QUuid::createUuid().toString(QUuid::WithoutBraces), QJsonValue(text));
                    jsonObject["Book" + QString::number(length + 1)] = bookJsonObject;

                    writeHighlightsDatabase(jsonObject);
                    highlightWrote = true;
                }
            }
        }
        else {
            QJsonObject jsonObject = readHighlightsDatabase();
            int length = jsonObject.length();
            for(int i = 1; i <= length; i++) {
                if(jsonObject["Book" + QString::number(i)].toObject().value("BookPath").toString() == bookPath) {
                    QJsonObject bookJsonObject = jsonObject["Book" + QString::number(i)].toObject();
                    foreach(const QString& key, bookJsonObject.keys()) {
                        if(bookJsonObject.value(key).toString() == text) {
                            log("Found matching highlight to remove with text '" + text + "'", "functions.h");
                            bookJsonObject.remove(key);
                        }
                    }
                    jsonObject["Book" + QString::number(i)] = bookJsonObject;
                    writeHighlightsDatabase(jsonObject);
                }
            }
        }
    }
    QJsonObject getHighlightsForBook(QString bookPath) {
        QJsonObject jsonObject = readHighlightsDatabase();
        int length = jsonObject.length();
        for(int i = 1; i <= length; i++) {
            if(jsonObject["Book" + QString::number(i)].toObject().value("BookPath").toString() == bookPath) {
                return jsonObject["Book" + QString::number(i)].toObject();
                break;
            }
        }
        return QJsonObject();
    }
    float determineYIncrease() {
        if(global::deviceID == "n705\n" or global::deviceID == "n905\n") {
            return 2;
        }
        else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
            return 2.6;
        }
        else if(global::deviceID == "n437\n" or global::deviceID == "n873\n") {
            return 3;
        }
        else {
            return 2;
        }
    }
    global::wifi::wifiState checkWifiState() {
        QProcess *wifiStateProcess = new QProcess();
        QString path = "/external_root/usr/local/bin/wifi/wifi_status.sh";
        QStringList args;
        wifiStateProcess->start(path, args);
        wifiStateProcess->waitForFinished();
        wifiStateProcess->deleteLater();

        QString currentWifiState;
        if(QFile("/run/wifi_status").exists() == true) {
            currentWifiState = readFile("/run/wifi_status");
        } else {
            log("/run/wifi_status doesn't exist", "functions");
        }
        if (currentWifiState.contains("configured") == true) {
            global::wifi::isConnected = true;
            return global::wifi::wifiState::configured;
        }
        else if (currentWifiState.contains("enabled") == true) {
            global::wifi::isConnected = false;
            return global::wifi::wifiState::enabled;
        }
        else if (currentWifiState.contains("disabled") == true) {
            global::wifi::isConnected = false;
            return global::wifi::wifiState::disabled;
        } else {
            global::wifi::isConnected = false;
            QString function = __func__; log(function + ": Critical error", "functions");
            return global::wifi::wifiState::unknown;
        }
    }
    int testPing() {
        // For some reason, implementing a non-blocking version of this functions triggers a "terminate called without an active exception" error with a platform plugin compiled with a newer GCC 11 toolchain. The problem has been solved by transplanting this function into the related area which uses it.
        QProcess *pingProcess = new QProcess();
        QString pingProg = "ping";
        QStringList pingArgs;
        pingArgs << "-c" << "1" << "1.1.1.1";
        pingProcess->start(pingProg, pingArgs);
        pingProcess->waitForFinished();
        int exitCode = pingProcess->exitCode();
        pingProcess->deleteLater();
        if(exitCode == 0) {
            global::wifi::isConnected = true;
        }
        else {
            global::wifi::isConnected = false;
        }
        return exitCode;
    }
    bool checkProcessName(QString name) {
        QDirIterator appsDir("/proc", QDirIterator::NoIteratorFlags);
        while (appsDir.hasNext()) {
            QDir dir(appsDir.next());
            QFile process = QFile(dir.path() + "/cmdline");
            if(process.exists() == true) {
                process.open(QIODevice::ReadOnly);
                QTextStream stream(&process);
                if(stream.readLine().contains(name) == true) {
                    process.close();
                    return true;
                }
                process.close();
            }
        }
        return false;
    }
    QString purgeHtml(QString text) {
        // https://stackoverflow.com/questions/2799379/is-there-an-easy-way-to-strip-html-from-a-qstring-in-qt
        // This can cause problems if someone names their directory with HTML tags, so stop here. Anki, which is a big project, also doesn't care about this
        return text.remove(QRegExp("<[^>]*>"));
    }
    // Some audio wrappers
    void waitForAudioThread() {
        bool tempBool = false;
        while(tempBool == false) {
          QThread::msleep(50);
          global::audio::audioMutex.lock();
          if(global::audio::actionDone == true) {
              global::audio::actionDone = false;
              tempBool = true;
          }
          global::audio::audioMutex.unlock();
        }
    }
    void AudioThreadAction(global::audio::Action actionToPerform) {
        global::audio::audioMutex.lock();
        global::audio::currentAction = actionToPerform;
        global::audio::audioMutex.unlock();
        waitForAudioThread();
    }
    // I'm sick of poor code
    void bool_writeconfig(QString file, bool option) {
        QString str;
        if(option == true) {
            str = "true";
        }
        else {
            str = "false";
        }
        std::ofstream fhandler;
        fhandler.open(file.toStdString());
        fhandler << str.toStdString();
        fhandler.close();
    }
}

#endif // FUNCTIONS_H
