#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
#include "apps.h"
#include "ui_apps.h"
#include "functions.h"
#include "audiodialog.h"

#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <string>
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QPixmap>
#include <QScreen>
#include <QFont>
#include <QFontDatabase>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Needed, the screen is shifted?
    // ~Szybet
    if(global::deviceID == "n306\n") {
        ui->centralwidget->layout()->setContentsMargins(4, 2, 8, 2);
    }

    ui->inkboxLabel->setFont(QFont("u001"));

    ui->settingsBtn->setProperty("type", "borderless");
    ui->appsBtn->setProperty("type", "borderless");
    ui->quitBtn->setProperty("type", "borderless");
    ui->searchBtn->setProperty("type", "borderless");
    ui->pushButton->setProperty("type", "borderless");
    ui->libraryButton->setProperty("type", "borderless");
    ui->brightnessBtn->setProperty("type", "borderless");
    ui->homeBtn->setProperty("type", "borderless");
    ui->wifiBtn->setProperty("type", "borderless");
    ui->audioBtn->setProperty("type", "borderless");

    ui->settingsBtn->setText("");
    ui->appsBtn->setText("");
    ui->pushButton->setText("\t\tReader");
    ui->libraryButton->setText("\t\tLibrary");
    ui->quitBtn->setText("");
    ui->searchBtn->setText("");
    ui->brightnessBtn->setText("");
    ui->homeBtn->setText("");
    ui->quoteLabel->setText("");
    ui->wifiBtn->setText("");
    ui->audioBtn->setText("");

    ui->quotePictureLabel->setText("");

    ui->quoteHeadingLabel->setStyleSheet("padding: 30px");
    ui->inkboxLabel->setStyleSheet("font-size: 10.5pt");
    ui->homeBtn->setStyleSheet("padding: 5px");

    if(global::audio::enabled == false) {
        ui->audioBtn->hide();
        ui->audioLine->hide();
    }

    // Initializing some variables
    global::battery::showLowBatteryDialog = true;
    global::battery::showCriticalBatteryAlert = true;
    global::usbms::showUsbmsDialog = true;
    global::usbms::launchUsbms = false;
    global::usbms::koboxExportExtensions = false;
    global::mainwindow::tabSwitcher::repaint = true;
    resetFullWindowException = false;

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    if(global::deviceID == "n705\n") {
        stdIconWidth = sW / 12;
        stdIconHeight = sH / 12;
        brightnessIconWidth = sW / 24;
        brightnessIconHeight = sH / 24;
        homeIconWidth = sW / 18;
        homeIconHeight = sW / 18;
        wifiIconWidth = sW / 20.5;
        wifiIconHeight = sH / 20.5;
    }
    else if(global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 26;
        brightnessIconHeight = sH / 26;
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
        wifiIconWidth = sW / 22.5;
        wifiIconHeight = sH / 22.5;
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
        stdIconWidth = sW / 12.5;
        stdIconHeight = sH / 12.5;
        brightnessIconWidth = sW / 24.5;
        brightnessIconHeight = sH / 24.5;
        homeIconWidth = sW / 18.5;
        homeIconHeight = sW / 18.5;
        wifiIconWidth = sW / 21;
        wifiIconHeight = sH / 21;
    }
    else {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 26;
        brightnessIconHeight = sH / 26;
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
        wifiIconWidth = sW / 22.5;
        wifiIconHeight = sH / 22.5;
    }

    // Setting icons up
    ui->pushButton->setIcon(QIcon(":/resources/book.png"));
    ui->pushButton->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->libraryButton->setIcon(QIcon(":/resources/online-library.png"));
    ui->libraryButton->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->searchBtn->setIcon(QIcon(":/resources/search.png"));
    ui->searchBtn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->settingsBtn->setIcon(QIcon(":/resources/settings.png"));
    ui->settingsBtn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->appsBtn->setIcon(QIcon(":/resources/apps.png"));
    ui->appsBtn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->quitBtn->setIcon(QIcon(":/resources/power.png"));
    ui->quitBtn->setIconSize(QSize(stdIconWidth, stdIconHeight));

    ui->homeBtn->setIcon(QIcon(":/resources/home.png"));
    ui->homeBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));

    ui->brightnessBtn->setIcon(QIcon(":/resources/frontlight.png"));
    ui->brightnessBtn->setIconSize(QSize(brightnessIconWidth, brightnessIconHeight));

    ui->audioBtn->setIcon(QIcon(":/resources/music-note.png"));
    ui->audioBtn->setIconSize(QSize(stdIconWidth, stdIconHeight));

    updateWifiAble();
    if(global::device::isWifiAble == true) {
        // Start Wi-Fi icon updater
        QTimer *wifiIconTimer = new QTimer(this);
        wifiIconTimer->setInterval(2500);
        connect(wifiIconTimer, SIGNAL(timeout()), this, SLOT(updateWifiIcon()));
        wifiIconTimer->start();
    }
    setBatteryIcon();

    ui->brightnessBtn->setStyleSheet("font-size: 9pt; padding-bottom: 5px; padding-top: 5px; padding-left: 8px; padding-right: 8px;");
    ui->wifiBtn->setStyleSheet("font-size: 9pt; padding-bottom: 0px; padding-top: 0px; padding-left: 8px; padding-right: 8px");

    // Checking if we have a Mini or Touch there
    if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 1px; padding-right: 1px;");
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 0px; padding-right: 0px;");
    }
    else if(global::deviceID == "n873\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 0px; padding-right: 0px;");
    }
    else {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 8px; padding-right: 8px;");
    }

    ui->line_7->setStyleSheet("padding: 0px");
    ui->batteryIcon->setText("");
    ui->batteryLabel->setText("");
    ui->timeLabel->setText("");
    ui->batteryLabel->setStyleSheet("padding-top: 0px; padding-bottom: 0px; padding-left: 0px; padding-right: 0px");

    // Deleting/Hiding "Library" button if device is not WiFi-able
    // NOTE: Using deleteLater() on these elements causes a segmentation fault and aborts the whole program when the Settings, Apps or Home button is pressed. No idea why.
    if(global::device::isWifiAble == false && global::deviceID != "emu\n") {
        ui->libraryButton->hide();
        ui->line_10->hide();
    }

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Custom settings
    // Reading from the config files and tweaking the program according to the options set
    // Demo setting, changes "Welcome to InkBox" label to "InkBox"
    if(checkconfig(".config/01-demo/config") == true) {
        ui->inkboxLabel->setText("InkBox");
    }

    // Dark mode; write to the Kobo Nightmode FIFO
    if(checkconfig(".config/10-dark_mode/config") == true) {
        string_writeconfig("/tmp/invertScreen", "y");
    }
    else {
        string_writeconfig("/tmp/invertScreen", "n");
    }

    // Clock setting to show seconds
    if(checkconfig(".config/02-clock/config") == true) {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm:ss");
           get_battery_level();
           ui->timeLabel->setText(time);
           ui->batteryLabel->setText(batt_level);
        } );
        t->start();
    }
    else {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm");
           get_battery_level();
           ui->timeLabel->setText(time);
           ui->batteryLabel->setText(batt_level);
        } );
        t->start();
    }

    // Battery watchdog
    QTimer *batteryWatchdog = new QTimer(this);
    batteryWatchdog->setInterval(2000);
    connect(batteryWatchdog, &QTimer::timeout, [&]() {
        // Checking if battery level is low
        if(global::battery::showCriticalBatteryAlert != true) {
            ;
        }
        else {
            if(isBatteryCritical() == true) {
                if(isUsbPluggedIn() == true) {
                    ;
                }
                else {
                    log("Warning! Battery is at a critical charge level!", className);
                    openCriticalBatteryAlertWindow();
                }
            }
        }

        if(global::battery::showLowBatteryDialog != true) {
            // Do nothing, since a dialog should already have been displayed and (probably) dismissed
            ;
        }
        else {
            if(isBatteryLow() == true) {
                if(global::battery::batteryAlertLock == true) {
                    ;
                }
                else {
                    if(isUsbPluggedIn() == true) {
                        ;
                    }
                    else {
                        log("Warning! Battery is low!", className);
                        openLowBatteryDialog();
                    }
                }
            }
        }
    } );
    batteryWatchdog->start();

    // We set the brightness level saved in the config file
    QTimer::singleShot(2000, this, SLOT(setInitialBrightness()));

    // If new files are found in /mnt/onboard/onboard/encfs-dropbox, ask if user wants to encrypt them
    if(checkconfig(".config/18-encrypted_storage/status") == true) {
        QDir encfsDropboxDir("/mnt/onboard/onboard/encfs-dropbox");
        if(!encfsDropboxDir.isEmpty()) {
            QTimer::singleShot(1000, this, SLOT(openEncfsRepackDialog()));
        }
    }

    // Global reading settings
    string_checkconfig(".config/16-global_reading_settings/config");
    if(checkconfig_str_val == "") {
        checked_box = true;
        writeconfig(".config/16-global_reading_settings/config", "GlobalReadingSettings=");
    }

    if(checkconfig(".config/05-quote/config") == false) {
        stdIconWidth = sW / 2;
        stdIconHeight = sH / 2;
        int quote_value = display_quote();
        if(quote_value == 1) {
            QPixmap pixmap(":/resources/chesterton.jpg");
            QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            ui->quotePictureLabel->setPixmap(scaledPixmap);
            ui->quoteLabel->setText("“A good novel tells us the truth about its hero; but a bad novel tells us the truth about its author.”\n― G.K. Chesterton");
        }
        if(quote_value == 2) {
            QPixmap pixmap(":/resources/alcott.jpg");
            QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            ui->quotePictureLabel->setPixmap(scaledPixmap);
            ui->quoteLabel->setText("“I've got the key to my castle in the air, but whether I can unlock the door remains to be seen.”\n― Louisa May Alcott");
        }
        if(quote_value == 3) {
            QPixmap pixmap(":/resources/king.jpg");
            QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            ui->quotePictureLabel->setPixmap(scaledPixmap);
            ui->quoteLabel->setText("“Quiet people have the loudest minds”\n― Stephen King");
        }
        if(quote_value == 4) {
            QPixmap pixmap(":/resources/davies.jpg");
            QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            ui->quotePictureLabel->setPixmap(scaledPixmap);
            ui->quoteLabel->setText("“Authors like cats because they are such quiet, lovable, wise creatures, and cats like authors for the same reasons.”\n― Robertson Davies");
        }
        if(quote_value == 5) {
            QPixmap pixmap(":/resources/christie.png");
            QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            ui->quotePictureLabel->setPixmap(scaledPixmap);
            ui->quoteLabel->setText("“One of the saddest things in life, is the things one remembers.”\n― Agatha Christie");
        }
        ui->homeStackedWidget->setCurrentIndex(0);
    }
    else {
        setupHomePageWidget();
    }

    // Check if it's the first boot since an update and confirm that it installed successfully
    if(checkconfig("/opt/inkbox_genuine") == true) {
        if(checkconfig("/external_root/opt/update/inkbox_updated") == true) {
            string_checkconfig_ro("/external_root/opt/isa/version");
            QString updatemsg = "<font face='u001'>InkBox update to version ";
            updatemsg = updatemsg.append(checkconfig_str_val);
            updatemsg = updatemsg.remove(QRegExp("[\n]"));
            updatemsg = updatemsg.append(" completed successfully.<br><br>Changelog:<br>");
            string_checkconfig_ro("/external_root/opt/isa/changelog");
            updatemsg = updatemsg.append(checkconfig_str_val);
            updatemsg = updatemsg.append("</font>");
            log("Showing update changelog", className);
            QMessageBox::information(this, tr("Information"), updatemsg);
            string_writeconfig("/external_root/opt/update/inkbox_updated", "false");

            // Trigger Gutenberg re-sync, because we deleted the .inkbox folder ;)
            if(QFile::exists("/external_root/opt/storage/gutenberg/last_sync")) {
                QFile::remove("/external_root/opt/storage/gutenberg/last_sync");
            }
        }
    }

    // OTA update
    if(!QFile(updatesCheckConfigPath).exists()) {
        writeFile(updatesCheckConfigPath, "true");
    }

    if(checkconfig(updatesCheckConfigPath) == true) {
        // Check for an update and ask if the user wants to install it
        checkForUpdate();
        // Check for an OTA update
        QTimer::singleShot(1000, this, SLOT(checkForOtaUpdate()));
    }

    // USB mass storage prompt
    if(!QFile(usbConfigPath).exists()) {
        writeFile(usbConfigPath, "true");
    }

    if(checkconfig(usbConfigPath) == true) {
        QTimer *usbmsPrompt = new QTimer(this);
        usbmsPrompt->setInterval(500);
        connect(usbmsPrompt, &QTimer::timeout, [&]() {
            if(checkconfig("/opt/inkbox_genuine") == true) {
                if(global::usbms::showUsbmsDialog != true) {
                    if(isUsbPluggedIn() != usbmsStatus) {
                        global::usbms::showUsbmsDialog = true;
                    }
                }
                else {
                    usbmsStatus = isUsbPluggedIn();
                    if(usbmsStatus == false) {
                        // Loop again...
                        ;
                    }
                    else {
                        // An USB cable is connected!
                        setBatteryIcon();
                        openUsbmsDialog();
                    }
                }
            }
            else {
                // Do nothing, we're running along with Nickel & friends...
                ;
            }
        } );
        usbmsPrompt->start();
    }

    // If the DEVKEY file is present, install a developer key
    if(QFile::exists("/mnt/onboard/onboard/.inkbox/DEVKEY") == true && QFile::exists("/mnt/onboard/onboard/.inkbox/DEVKEY.dgst") == true) {
        string_checkconfig_ro("/mnt/onboard/onboard/.inkbox/DEVKEY");
        QString developerKey = checkconfig_str_val.left(256);

        setDefaultWorkDir();
        QString prog ("sh");
        QStringList args;
        args << "install_developer-key.sh" << developerKey << "/mnt/onboard/onboard/.inkbox/DEVKEY.dgst";
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();
        proc->deleteLater();

        QFile::remove("/mnt/onboard/onboard/.inkbox/DEVKEY");
        QFile::remove("/mnt/onboard/onboard/.inkbox/DEVKEY.dgst");

        reboot(true);
        qApp->quit();
    }

    // Write version control info to file in tmpfs
    if(checkconfig("/opt/inkbox_genuine") == true) {
        string_writeconfig("/external_root/run/inkbox_gui_git_commit", GIT_COMMIT);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openUpdateDialog() {
    log("Showing Update dialog", className);
    global::mainwindow::updateDialog = true;
    // Write to a temporary file to show an "Update" prompt
    string_writeconfig("/inkbox/updateDialog", "true");

    // Setup the dialog
    generalDialogWindow = new generalDialog(this);
    connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
    connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QApplication::processEvents();
}

void MainWindow::openLowBatteryDialog() {
    log("Showing Low Battery Dialog", className);
    global::mainwindow::lowBatteryDialog = true;
    global::battery::batteryAlertLock = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QApplication::processEvents();
}

void MainWindow::openUsbmsDialog() {
    log("Showing USB Mass Storage dialog", className);
    global::usbms::showUsbmsDialog = false;
    global::usbms::usbmsDialog = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QApplication::processEvents();
}

void MainWindow::openCriticalBatteryAlertWindow() {
    global::battery::showCriticalBatteryAlert = true;
    global::battery::showLowBatteryDialog = false;

    alertWindow = new alert();
    alertWindow->setAttribute(Qt::WA_DeleteOnClose);
    alertWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    alertWindow->show();
}

void MainWindow::on_settingsBtn_clicked()
{
    log("Opening Settings Chooser widget", className);
    resetFullWindowException = true;
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::settingsChooserWidgetSelected != true) {
        ui->settingsBtn->setStyleSheet("background: black");
        ui->settingsBtn->setIcon(QIcon(":/resources/settings-inverted.png"));

        // Create widget and make necessary connections
        settingsChooserWindow = new settingsChooser();
        connect(settingsChooserWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
        connect(settingsChooserWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
        ui->stackedWidget->insertWidget(2, settingsChooserWindow);
        global::mainwindow::tabSwitcher::settingsChooserWidgetCreated = true;

        // Switch tab
        ui->stackedWidget->setCurrentIndex(2);
        global::mainwindow::tabSwitcher::settingsChooserWidgetSelected = true;

        // Repaint
        this->repaint();
    }
    else {
        ;
    }
}

void MainWindow::on_appsBtn_clicked()
{
    log("Opening Apps widget", className);
    resetFullWindowException = true;
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::appsWidgetSelected != true) {
        ui->appsBtn->setStyleSheet("background: black");
        ui->appsBtn->setIcon(QIcon(":/resources/apps-inverted.png"));

        // Create widget
        appsWindow = new apps();
        connect(appsWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
        connect(appsWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
        ui->stackedWidget->insertWidget(1, appsWindow);
        global::mainwindow::tabSwitcher::appsWidgetCreated = true;

        // Switch tab
        ui->stackedWidget->setCurrentIndex(1);
        global::mainwindow::tabSwitcher::appsWidgetSelected = true;

        // Repaint
        this->repaint();
    }
    else {
        ;
    }
}

void MainWindow::on_pushButton_clicked()
{
    resetFullWindowException = true;
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::localLibraryWidgetSelected != true) {
        ui->pushButton->setStyleSheet("background: black; color: white");
        ui->pushButton->setIcon(QIcon(":/resources/book_inverted.png"));

        // Create widget
        setupLocalLibraryWidget();
        global::mainwindow::tabSwitcher::localLibraryWidgetCreated = true;

        // Switch tab
        ui->stackedWidget->setCurrentIndex(0);
        global::mainwindow::tabSwitcher::localLibraryWidgetSelected = true;

        // Repaint
        this->repaint();
    }
}

void MainWindow::on_searchBtn_clicked()
{
    global::forbidOpenSearchDialog = false;
    setupSearchDialog();
}

void MainWindow::on_quitBtn_clicked()
{
    log("Opening Quit widget", className);
    quitWindow = new quit();
    quitWindow->setAttribute(Qt::WA_DeleteOnClose);
    quitWindow->showFullScreen();
}

void MainWindow::on_brightnessBtn_clicked()
{
    log("Showing Brightness Dialog", className);
    brightnessDialogWindow = new brightnessDialog();
    brightnessDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    brightnessDialogWindow->show();
}

void MainWindow::on_homeBtn_clicked()
{
    log("Showing home screen", className);
    if(global::localLibrary::bookOptionsDialog::bookPinAction == true) {
        global::localLibrary::bookOptionsDialog::bookPinAction = false;
        global::mainwindow::tabSwitcher::repaint = false;
    }
    else {
        global::mainwindow::tabSwitcher::repaint = true;
    }
    resetFullWindowException = true;
    resetWindow(true);
}

void MainWindow::resetWindow(bool resetStackedWidget) {
    // Reset layout
    if(resetStackedWidget == true) {
        ui->homeStackedWidget->setCurrentIndex(2);
        ui->stackedWidget->setCurrentIndex(0);
    }

    // Destroy widgets
    if(global::mainwindow::tabSwitcher::homePageWidgetCreated == true) {
        homePageWidgetWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::appsWidgetCreated == true) {
        appsWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::settingsChooserWidgetCreated == true) {
        settingsChooserWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::libraryWidgetCreated == true) {
        libraryWidgetWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::localLibraryWidgetCreated == true) {
        localLibraryWidgetWindow->deleteLater();
    }

    global::mainwindow::tabSwitcher::homePageWidgetCreated = false;
    global::mainwindow::tabSwitcher::appsWidgetCreated = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetCreated = false;
    global::mainwindow::tabSwitcher::appsWidgetSelected = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetSelected = false;
    global::mainwindow::tabSwitcher::libraryWidgetCreated = false;
    global::mainwindow::tabSwitcher::libraryWidgetSelected = false;
    global::mainwindow::tabSwitcher::localLibraryWidgetCreated = false;
    global::mainwindow::tabSwitcher::localLibraryWidgetSelected = false;

    resetIcons();
    setBatteryIcon();
    if(global::mainwindow::tabSwitcher::repaint == true) {
        this->repaint();
    }
    if(resetStackedWidget == true) {
        setupHomePageWidget();
    }
}

void MainWindow::resetIcons() {
    // Reset icons
    ui->appsBtn->setStyleSheet("background: white");
    ui->appsBtn->setIcon(QIcon(":/resources/apps.png"));
    ui->settingsBtn->setStyleSheet("background: white");
    ui->settingsBtn->setIcon(QIcon(":/resources/settings.png"));
    ui->libraryButton->setStyleSheet("background: white");
    ui->libraryButton->setIcon(QIcon(":/resources/online-library.png"));
    ui->pushButton->setStyleSheet("background: white");
    ui->pushButton->setIcon(QIcon(":/resources/book.png"));
}

void MainWindow::setBatteryIcon() {
    // Battery
    if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "n613\n" or global::deviceID == "n873\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "kt\n") {
        // Hide brightness controls; they won't be very useful there anyway (for anything but the Glo (HD)/Libra/Aura 2) ...
        if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
            ui->brightnessBtn->hide();
            ui->line_7->hide();
        }

        // Setting icons up
        stdIconWidth = sW / 16;
        stdIconHeight = sH / 16;

        QPixmap chargingPixmap(":/resources/battery_charging.png");
        QPixmap scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap fullPixmap(":/resources/battery_full.png");
        QPixmap scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap halfPixmap(":/resources/battery_half.png");
        QPixmap scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap emptyPixmap(":/resources/battery_empty.png");
        QPixmap scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Checking battery level and status, then displaying the relevant icon on batteryIcon
        if(isUsbPluggedIn() == true) {
            ui->batteryIcon->setPixmap(scaledChargingPixmap);
        }
        else {
            get_battery_level();
            if(batt_level_int >= 75 && batt_level_int <= 100) {
                ui->batteryIcon->setPixmap(scaledFullPixmap);
            }
            else if(batt_level_int >= 25 && batt_level_int <= 74) {
                ui->batteryIcon->setPixmap(scaledHalfPixmap);
            }
            else if(batt_level_int >= 0 && batt_level_int <= 24) {
                ui->batteryIcon->setPixmap(scaledEmptyPixmap);
            }
        }
    }
    else {
        // Setting icons up
        stdIconWidth = sW / 19;
        stdIconHeight = sH / 19;
        QPixmap chargingPixmap(":/resources/battery_charging.png");
        QPixmap scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap fullPixmap(":/resources/battery_full.png");
        QPixmap scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap halfPixmap(":/resources/battery_half.png");
        QPixmap scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap emptyPixmap(":/resources/battery_empty.png");
        QPixmap scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Checking battery level and status, then displaying the relevant icon on batteryIcon
        if(isUsbPluggedIn() == true) {
            ui->batteryIcon->setPixmap(scaledChargingPixmap);
        }
        else {
            get_battery_level();
            if(batt_level_int >= 75 && batt_level_int <= 100) {
                ui->batteryIcon->setPixmap(scaledFullPixmap);
            }
            else if(batt_level_int >= 25 && batt_level_int <= 74) {
                ui->batteryIcon->setPixmap(scaledHalfPixmap);
            }
            else if(batt_level_int >= 0 && batt_level_int <= 24) {
                ui->batteryIcon->setPixmap(scaledEmptyPixmap);
            }
        }
    }
}

void MainWindow::setInitialBrightness() {
    if(global::deviceID == "n873\n") {
        int warmth;
        string_checkconfig_ro(".config/03-brightness/config-warmth");
        if(checkconfig_str_val == "") {
            warmth = 0;
        }
        else {
            warmth = checkconfig_str_val.toInt();
        }
        set_warmth(warmth);
    }
    int brightness_value = brightness_checkconfig(".config/03-brightness/config");
    if(global::deviceID != "n705\n" and global::deviceID != "n905\n" and global::deviceID != "kt\n") {
        log("Setting initial brightness to " + QString::number(brightness_value), className);
    }
    if(checkconfig("/tmp/oobe-inkbox_completed") == true) {
        // Coming from OOBE setup; not doing that fancy stuff again ;p
        QFile::remove("/tmp/oobe-inkbox_completed");
        pre_set_brightness(brightness_value);
        log("Ignoring cinematic brightness call because it has already been done", className);
    }
    else {
        // Fancy brightness fade-in
        if(checkconfig("/tmp/inkbox-cinematicBrightness_auto") == true) {
            QFile::remove("/tmp/inkbox-cinematicBrightness_auto");
            cinematicBrightness(brightness_value, 2);
        }
        else {
            if(checkconfig("/tmp/inkbox-cinematicBrightness_ran") == false) {
                string_writeconfig("/tmp/inkbox-cinematicBrightness_ran", "true");
                cinematicBrightness(brightness_value, 0);
            }
            else {
                log("Ignoring cinematic brightness call because it has already been done", className);
            }
        }
    }
}

void MainWindow::refreshScreen() {
    this->repaint();
}

void MainWindow::setupSearchDialog() {
    if(global::forbidOpenSearchDialog == false) {
        log("Launching Search dialog", className);
        global::keyboard::keyboardDialog = true;
        global::keyboard::searchDialog = true;
        global::keyboard::keyboardText = "";
        generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupSearchDialog()));
        connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
        connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
        connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
        connect(generalDialogWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
        generalDialogWindow->show();
    }
    else {
        ;
    }
}

void MainWindow::updateWifiIcon() {
    /* Usage:
     * Mode 0 (looping it) is handled in MainWindow
    */

    global::wifi::wifiState currentWifiState = checkWifiState();

    // It's executing only in enabled mode, which is a mode between connected and disabled, so don't worry about performance
    if(isConnecting == false and isReconnecting == false) {
        if(currentWifiState == global::wifi::wifiState::enabled) {
            if(checkProcessName("connection_manager.sh") == true) {
                isConnecting = true;
            }
            else if(checkProcessName("connect_to_network.sh") == true){
                isConnecting = true;
                isReconnecting = true;
            }
        }
    }

    // ms can make a difference, so:
    currentWifiState = checkWifiState();

    if(lastWifiState != currentWifiState) {
        if(currentWifiState == global::wifi::wifiState::disabled) {
            if(isConnecting == true) {
                if(checkconfig("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/stopped") == false) {
                    if(isReconnecting == true) {
                        showToast("Reconnection failed");
                        isReconnecting = false;
                    }
                    else {
                        showToast("Connection failed");
                    }
                    isConnecting = false;
                    QFile("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/essid").remove();
                    QFile("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/passphrase").remove();
                }
                else {
                    QFile("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/stopped").remove();
                }
            }
            lastWifiState = global::wifi::wifiState::disabled;
            ui->wifiBtn->setIcon(QIcon(":/resources/wifi-off.png"));
            ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
        }
        if(currentWifiState == global::wifi::wifiState::enabled) {
            lastWifiState = global::wifi::wifiState::enabled;
            ui->wifiBtn->setIcon(QIcon(":/resources/wifi-standby.png"));
            ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
        }
        if(currentWifiState == global::wifi::wifiState::configured) {
            if(isConnecting == true) {
                setDefaultWorkDir();
                    if(isReconnecting == true) {
                        showToast("Reconnection successful");
                        isReconnecting = false;
                    }
                    else {
                        showToast("Connection successful");
                    }
                    isConnecting = false;
                    QFile("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/stopped").remove();
            }
            lastWifiState = global::wifi::wifiState::configured;
            ui->wifiBtn->setIcon(QIcon(":/resources/wifi-100.png"));
            ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
        }
    }
}

void MainWindow::updateWifiAble() {
    if(global::device::isWifiAble == false) {
        ui->wifiBtn->hide();
        ui->line_9->hide();
    }
}

void MainWindow::on_wifiBtn_clicked()
{
    wifiDialog* newWifiDialog = new wifiDialog();
    QObject::connect(newWifiDialog, &wifiDialog::showToast, this, &MainWindow::showToast);
    newWifiDialog->exec();
}

void MainWindow::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(toastWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    connect(toastWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
    connect(toastWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
    toastWindow->show();

    // I will soon manage the update thing in a more proper way somewhere else... ~ Szybet
    if(messageToDisplay.contains("onnected successfully") == true) {
        // Give the toast some time to vanish away, then launch OTA updater
        QTimer::singleShot(5000, this, SLOT(launchOtaUpdater()));
    }
}

void MainWindow::hello(int testNumber) {
    log("Hello" + QString::number(testNumber), className);
}

void MainWindow::closeIndefiniteToast() {
    // Warning: use with caution
    toastWindow->close();
}

void MainWindow::openUpdateDialogOTA(bool open) {
    if(open == true) {
        QString function = __func__; log(function + ": Showing update dialog (OTA)", className);
        global::otaUpdate::isUpdateOta = true;
        openUpdateDialog();
    }
    else {
        ;
    }
}

void MainWindow::launchOtaUpdater() {
    log("Launching OTA updater", className);
    otaManagerWindow = new otaManager(this);
    connect(otaManagerWindow, SIGNAL(canOtaUpdate(bool)), SLOT(openUpdateDialogOTA(bool)));
    otaManagerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::openBookFile(QString book, bool relativePath) {
    if(relativePath == true) {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            book.prepend("/mnt/onboard/onboard/");
        }
        else {
            book.prepend("/mnt/onboard/");
        }
    }

    global::reader::skipOpenDialog = true;
    global::reader::bookFile = book;
    openReaderFramework();
}

void MainWindow::openReaderFramework() {
    log("Launching Reader Framework", className);
    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(readerWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
    readerWindow->showFullScreen();
}

void MainWindow::checkForUpdate() {
    log("Checking for available updates", className);
    if(checkconfig("/mnt/onboard/onboard/.inkbox/can_update") == true) {
        if(checkconfig("/tmp/cancelUpdateDialog") == false) {
            // I'm sorry.
            QString function = __func__; log(function + ": An update is available.", className);
            QTimer::singleShot(2000, this, SLOT(openUpdateDialog()));
        }
        else {
            QString function = __func__; log(function + ": Not showing update dialog, user dismissed it", className);
        }
    }
    else {
        QString function = __func__; log(function + ": No update available.", className);
    }
}

void MainWindow::openEncfsRepackDialog() {
    log("Showing encrypted storage repack dialog", className);
    global::encfs::repackDialog = true;
    global::usbms::showUsbmsDialog = false;
    global::usbms::usbmsDialog = false;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_libraryButton_clicked()
{
    log("Launching Online Library", className);
    if(testPing() == 0 or global::deviceID == "emu\n") {
        resetFullWindowException = true;
        resetWindow(false);
        if(global::mainwindow::tabSwitcher::libraryWidgetSelected != true) {
            ui->libraryButton->setStyleSheet("background: black; color: white");
            ui->libraryButton->setIcon(QIcon(":/resources/online-library-inverted.png"));

            // Create widget
            libraryWidgetWindow = new libraryWidget();
            connect(libraryWidgetWindow, SIGNAL(destroyed(QObject*)), SLOT(resetFullWindow()));
            libraryWidgetWindow->setAttribute(Qt::WA_DeleteOnClose);
            ui->stackedWidget->insertWidget(3, libraryWidgetWindow);
            global::mainwindow::tabSwitcher::libraryWidgetCreated = true;

            // Switch tab
            ui->stackedWidget->setCurrentIndex(3);
            global::mainwindow::tabSwitcher::libraryWidgetSelected = true;

            // Repaint
            this->repaint();
        }
    }
    else {
        showToast("Wi-Fi connection error");
    }
}

void MainWindow::resetFullWindow() {
    if(resetFullWindowException == false) {
        resetWindow(true);
    }
    else {
        resetFullWindowException = false;
    }
}

void MainWindow::checkForOtaUpdate() {
    if(global::wifi::isConnected == true) {
        string_checkconfig_ro("/external_root/opt/storage/update/last_sync");
        if(!checkconfig_str_val.isEmpty()) {
            unsigned long currentEpoch = QDateTime::currentSecsSinceEpoch();
            unsigned long syncEpoch = checkconfig_str_val.toULong();
            unsigned long allowSyncEpoch = syncEpoch + 86400;
            if(currentEpoch > allowSyncEpoch) {
                launchOtaUpdater();
            }
        }
        else {
            launchOtaUpdater();
        }
    }
}

void MainWindow::setupLocalLibraryWidget() {
    localLibraryWidgetWindow = new localLibraryWidget(this);
    QObject::connect(localLibraryWidgetWindow, &localLibraryWidget::openBookSignal, this, &MainWindow::openBookFile);
    QObject::connect(localLibraryWidgetWindow, &localLibraryWidget::refreshScreen, this, &MainWindow::refreshScreen);
    localLibraryWidgetWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->homeStackedWidget->insertWidget(1, localLibraryWidgetWindow);
    ui->homeStackedWidget->setCurrentIndex(1);
}

void MainWindow::setupHomePageWidget() {
    homePageWidgetWindow = new homePageWidget(this);
    QObject::connect(homePageWidgetWindow, &homePageWidget::openBookSignal, this, &MainWindow::openBookFile);
    QObject::connect(homePageWidgetWindow, &homePageWidget::refreshScreen, this, &MainWindow::refreshScreen);
    QObject::connect(homePageWidgetWindow, &homePageWidget::relaunchHomePageWidget, this, &MainWindow::on_homeBtn_clicked);
    homePageWidgetWindow->setAttribute(Qt::WA_DeleteOnClose);
    ui->homeStackedWidget->insertWidget(2, homePageWidgetWindow);
    ui->homeStackedWidget->setCurrentIndex(2);
    global::mainwindow::tabSwitcher::homePageWidgetCreated = true;
}

void MainWindow::on_audioBtn_clicked()
{
    QDialog* newAudioDialog = new audioDialog(this);
    newAudioDialog->exec();
}
