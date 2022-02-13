#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
#include "apps.h"
#include "ui_apps.h"
#include "functions.h"
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

    ui->settingsBtn->setProperty("type", "borderless");
    ui->appsBtn->setProperty("type", "borderless");
    ui->quitBtn->setProperty("type", "borderless");
    ui->searchBtn->setProperty("type", "borderless");
    ui->pushButton->setProperty("type", "borderless");
    ui->libraryButton->setProperty("type", "borderless");
    ui->brightnessBtn->setProperty("type", "borderless");
    ui->homeBtn->setProperty("type", "borderless");
    ui->wifiBtn->setProperty("type", "borderless");

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

    ui->quotePictureLabel->setText("");

    ui->quoteHeadingLabel->setStyleSheet("padding: 30px");
    ui->homeBtn->setStyleSheet("padding: 5px");

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
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n") {
        stdIconWidth = sW / 12;
        stdIconHeight = sH / 12;
        brightnessIconWidth = sW / 24;
        brightnessIconHeight = sH / 24;
        homeIconWidth = sW / 18;
        homeIconHeight = sW / 18;
        wifiIconWidth = sW / 20.5;
        wifiIconHeight = sH / 20.5;
    }
    else if(checkconfig_str_val == "n905\n") {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 26;
        brightnessIconHeight = sH / 26;
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
        wifiIconWidth = sW / 22.5;
        wifiIconHeight = sH / 22.5;
    }
    else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "emu\n") {
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

    setWifiIcon();
    if(global::device::isWifiAble == true) {
        updateWifiIcon(0);
    }
    setBatteryIcon();

    int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont crimson(family);

    ui->book1Btn->setFont(QFont(crimson));
    ui->book2Btn->setFont(QFont(crimson));
    ui->book3Btn->setFont(QFont(crimson));
    ui->book4Btn->setFont(QFont(crimson));
    ui->book1Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book2Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book3Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book4Btn->setStyleSheet("font-size: 11pt; padding: 25px");

    ui->brightnessBtn->setStyleSheet("font-size: 9pt; padding-bottom: 5px; padding-top: 5px; padding-left: 8px; padding-right: 8px;");
    ui->wifiBtn->setStyleSheet("font-size: 9pt; padding-bottom: 0px; padding-top: 0px; padding-left: 8px; padding-right: 8px");

    // Checking if we have a Mini or Touch there
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 1px; padding-right: 1px;");
    }
    else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "emu\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 0px; padding-right: 0px;");
    }
    else if(checkconfig_str_val == "n873\n") {
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

    ui->book1Btn->hide();
    ui->book2Btn->hide();
    ui->book3Btn->hide();
    ui->book4Btn->hide();

    ui->recentBooksLabel->hide();

    // Deleting/Hiding "Library" button if device is not WiFi-able
    // NOTE: Using deleteLater() on these elements causes a segmentation fault and aborts the whole program when the Settings, Apps or Home button is pressed. No idea why.
    if(global::device::isWifiAble == false && readFile("/opt/inkbox_device") != "emu\n") {
        ui->libraryButton->hide();
        ui->line_10->hide();
    }

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Running rootfs changes script if it's there
    // Not a big security flaw anymore. InkBox OS verifies the digital signature of the update package and aborts the update if it's not genuine.
    if(checkconfig("/opt/inkbox_genuine") == true) {
        if(checkconfig("/external_root/opt/update/inkbox_updated") == true) {
            // Checking if we need to reboot after running the two scripts
            if(checkconfig("/mnt/onboard/onboard/.inkbox/reboot") == true) {
                reboot_after_update = true;
                QFile::remove("/mnt/onboard/onboard/.inkbox/reboot");
            }
            else {
                reboot_after_update = false;
            }

            QFile::copy("/mnt/onboard/onboard/.inkbox/rootfs.sh", "/external_root/tmp/rootfs.sh");
            QFile::copy("/mnt/onboard/onboard/.inkbox/rootfs-internal.sh", "/tmp/rootfs-internal.sh");

            // First script
            QString rootfs_internal_prog ("sh");
            QStringList rootfs_internal_args;
            rootfs_internal_args << "/tmp/rootfs-internal.sh";
            // Removing script
            QFile::remove("/mnt/onboard/onboard/.inkbox/rootfs-internal.sh");
            QProcess *rootfs_internal_proc = new QProcess();
            rootfs_internal_proc->start(rootfs_internal_prog, rootfs_internal_args);
            rootfs_internal_proc->waitForFinished();
            rootfs_internal_proc->deleteLater();

            // Second script
            QString rootfs_prog ("chroot");
            QStringList rootfs_args;
            rootfs_args << "/external_root" << "/tmp/rootfs.sh";
            // Removing script
            QFile::remove("/mnt/onboard/onboard/.inkbox/rootfs.sh");
            QProcess *rootfs_proc = new QProcess();
            rootfs_proc->start(rootfs_prog, rootfs_args);
            rootfs_proc->waitForFinished();
            rootfs_proc->deleteLater();

            // Removing update directory contents
            QDir dir("/mnt/onboard/onboard/.inkbox");
            dir.removeRecursively();
            // Re-creating update directory
            QString path("/mnt/onboard/onboard/.inkbox");
            dir.mkpath(path);

            // Rebooting if needed
            if(reboot_after_update == true) {
                reboot(false);
            }
            else {
                // Update process finished.
                ;
            }
        }
    }
    else {
        ;
    }

    // Custom settings
    // Reading from the config files and tweaking the program according to the options set

    // Safety mesure; /inkbox is a tmpfs
    string_writeconfig("/tmp/skip_opendialog", "true");
    global::reader::skipOpenDialog = false;

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
                string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
                if(checkconfig_str_val == "Charging\n") {
                    ;
                }
                else {
                    qDebug() << "Warning! Battery is at a critical charge level!";
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
                    qDebug() << "Warning! Battery is low!";
                    string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
                    if(checkconfig_str_val == "Charging\n") {
                        ;
                    }
                    else {
                        openLowBatteryDialog();
                    }
                }
            }
        }
    } );
    batteryWatchdog->start();

    // USB mass storage prompt
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

    // We set the brightness level saved in the config file
    QTimer::singleShot(2000, this, SLOT(setInitialBrightness()));

    // If new files are found in /mnt/onboard/onboard/encfs-dropbox, ask if user wants to encrypt them
    if(checkconfig(".config/18-encrypted_storage/status") == true) {
        QDir encfsDropboxDir("/mnt/onboard/onboard/encfs-dropbox");
        if(!encfsDropboxDir.isEmpty()) {
            QTimer::singleShot(1000, this, SLOT(openEncfsRepackDialog()));
        }
    }

    // Display quote if requested; otherwise, display recent books
    string_checkconfig(".config/05-quote/config");
    if(checkconfig_str_val == "") {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            checked_box = true;
            writeconfig(".config/05-quote/config", "DisableQuote=");
        }
        else {
            checked_box = false;
            writeconfig(".config/05-quote/config", "DisableQuote=");
        }
    }

    // Global reading settings
    string_checkconfig(".config/16-global_reading_settings/config");
    if(checkconfig_str_val == "") {
        checked_box = true;
        writeconfig(".config/16-global_reading_settings/config", "GlobalReadingSettings=");
    }

    if(checkconfig(".config/05-quote/config") == false) {
        int quote_value = display_quote();
        if(quote_value == 1) {
            QPixmap pm(":/resources/chesterton.jpg");
            ui->quotePictureLabel->setPixmap(pm);
            ui->quotePictureLabel->setScaledContents(true);
            ui->quoteLabel->setText("“A good novel tells us the truth about its hero; but a bad novel tells us the truth about its author.”\n― G.K. Chesterton");
        }
        if(quote_value == 2) {
            QPixmap pm(":/resources/alcott.jpg");
            ui->quotePictureLabel->setPixmap(pm);
            ui->quotePictureLabel->setScaledContents(true);
            ui->quoteLabel->setText("“I've got the key to my castle in the air, but whether I can unlock the door remains to be seen.”\n― Louisa May Alcott");
        }
        if(quote_value == 3) {
            QPixmap pm(":/resources/king.jpg");
            ui->quotePictureLabel->setPixmap(pm);
            ui->quotePictureLabel->setScaledContents(true);
            ui->quoteLabel->setText("“Quiet people have the loudest minds”\n― Stephen King");
        }
        if(quote_value == 4) {
            QPixmap pm(":/resources/davies.jpg");
            ui->quotePictureLabel->setPixmap(pm);
            ui->quotePictureLabel->setScaledContents(true);
            ui->quoteLabel->setText("“Authors like cats because they are such quiet, lovable, wise creatures, and cats like authors for the same reasons.”\n― Robertson Davies");
        }
        if(quote_value == 5) {
            QPixmap pm(":/resources/christie.png");
            ui->quotePictureLabel->setPixmap(pm);
            ui->quotePictureLabel->setScaledContents(true);
            ui->quoteLabel->setText("“One of the saddest things in life, is the things one remembers.”\n― Agatha Christie");
        }
    }
    else {
        ui->gridLayout_4->deleteLater();
        ui->quotePictureLabel->deleteLater();
        ui->quoteLabel->deleteLater();
        ui->quoteHeadingLabel->setText("Books");
        ui->book1Btn->show();
        ui->book2Btn->show();
        ui->book3Btn->show();
        ui->book4Btn->show();

        setRecentBooksLabelsTruncateTreshold();
        // Book 1
        string_checkconfig(".config/08-recent_books/1");
        if(checkconfig_str_val == "") {
            ui->book1Btn->hide();
        }
        else {
            relative_path = checkconfig_str_val.split("/").last();
            if(relative_path.length() > truncateTreshold) {
                relative_path.truncate(truncateTreshold);
                relative_path.append(" ...");
            }
            ui->book1Btn->setText(relative_path);
            existing_recent_books = true;
        }
        // Book 2
        string_checkconfig(".config/08-recent_books/2");
        if(checkconfig_str_val == "") {
            ui->book2Btn->hide();
        }
        else {
            relative_path = checkconfig_str_val.split("/").last();
            if(relative_path.length() > truncateTreshold) {
                relative_path.truncate(truncateTreshold);
                relative_path.append(" ...");
            }
            ui->book2Btn->setText(relative_path);
            existing_recent_books = true;
        }
        // Book 3
        string_checkconfig(".config/08-recent_books/3");
        if(checkconfig_str_val == "") {
            ui->book3Btn->hide();
        }
        else {
            relative_path = checkconfig_str_val.split("/").last();
            if(relative_path.length() > truncateTreshold) {
                relative_path.truncate(truncateTreshold);
                relative_path.append(" ...");
            }
            ui->book3Btn->setText(relative_path);
            existing_recent_books = true;
        }
        // Book 4
        string_checkconfig(".config/08-recent_books/4");
        if(checkconfig_str_val == "") {
            ui->book4Btn->hide();
        }
        else {
            relative_path = checkconfig_str_val.split("/").last();
            if(relative_path.length() > truncateTreshold) {
                relative_path.truncate(truncateTreshold);
                relative_path.append(" ...");
            }
            ui->book4Btn->setText(relative_path);
            existing_recent_books = true;
        }

        // Preventing from having the same label two times
        if(ui->book2Btn->text() == ui->book1Btn->text()) {
            ui->book2Btn->hide();
        }
        else if(ui->book3Btn->text() == ui->book1Btn->text()) {
            ui->book3Btn->hide();
        }
        else if(ui->book4Btn->text() == ui->book1Btn->text()) {
            ui->book4Btn->hide();
        }
        else if(existing_recent_books == false) {
            ui->recentBooksLabel->show();
        }
        else {
            ui->recentBooksLabel->hide();
        }
    }

    // Check for an update and ask if the user wants to install it
    checkForUpdate();

    // Check if it's the first boot since an update and confirm that it installed successfully
    if(checkconfig("/opt/inkbox_genuine") == true) {
        if(checkconfig("/external_root/opt/update/inkbox_updated") == true) {
            string_checkconfig_ro("/external_root/opt/isa/version");
            QString updatemsg = "InkBox update to v";
            updatemsg = updatemsg.append(checkconfig_str_val);
            updatemsg = updatemsg.remove(QRegExp("[\n]"));
            updatemsg = updatemsg.append(" completed successfully.\n\nChangelog:\n");
            string_checkconfig_ro("/external_root/opt/isa/changelog");
            updatemsg = updatemsg.append(checkconfig_str_val);
            QMessageBox::information(this, tr("Information"), updatemsg);
            string_writeconfig("/external_root/opt/update/inkbox_updated", "false");
        }
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
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openUpdateDialog() {
    global::mainwindow::updateDialog = true;
    // Write to a temporary file to show an "Update" prompt
    string_writeconfig("/inkbox/updateDialog", "true");

    // Show the dialog
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
    QApplication::processEvents();
}

void MainWindow::openLowBatteryDialog() {
    global::mainwindow::lowBatteryDialog = true;
    global::battery::batteryAlertLock = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
    QApplication::processEvents();
}

void MainWindow::openUsbmsDialog() {
    global::usbms::showUsbmsDialog = false;
    global::usbms::usbmsDialog = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
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
    resetFullWindowException = true;
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::appsWidgetSelected != true) {
        ui->appsBtn->setStyleSheet("background: black");
        ui->appsBtn->setIcon(QIcon(":/resources/apps-inverted.png"));

        // Create widget
        appsWindow = new apps();
        connect(appsWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
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
    openReaderFramework();
}

void MainWindow::on_searchBtn_clicked()
{
    global::forbidOpenSearchDialog = false;
    setupSearchDialog();
}

void MainWindow::on_quitBtn_clicked()
{
    quitWindow = new quit();
    quitWindow->setAttribute(Qt::WA_DeleteOnClose);
    quitWindow->showFullScreen();
}

void MainWindow::on_book1Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 1;

    openReaderFramework();
}

void MainWindow::on_book2Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 2;

    openReaderFramework();
}

void MainWindow::on_book3Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 3;

    openReaderFramework();
}

void MainWindow::on_book4Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 4;

    openReaderFramework();
}

void MainWindow::on_brightnessBtn_clicked()
{
    brightnessDialogWindow = new brightnessDialog();
    brightnessDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    brightnessDialogWindow->show();
}

void MainWindow::on_homeBtn_clicked()
{
    global::mainwindow::tabSwitcher::repaint = true;
    resetFullWindowException = true;
    resetWindow(true);
}

void MainWindow::resetWindow(bool resetStackedWidget) {
    // Reset layout
    if(resetStackedWidget == true) {
        ui->stackedWidget->setCurrentIndex(0);
    }

    // Destroy widgets
    if(global::mainwindow::tabSwitcher::appsWidgetCreated == true) {
        appsWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::settingsChooserWidgetCreated == true) {
        settingsChooserWindow->deleteLater();
    }
    if(global::mainwindow::tabSwitcher::libraryWidgetCreated == true) {
        libraryWidgetWindow->deleteLater();
    }

    global::mainwindow::tabSwitcher::appsWidgetCreated = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetCreated = false;
    global::mainwindow::tabSwitcher::appsWidgetSelected = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetSelected = false;
    global::mainwindow::tabSwitcher::libraryWidgetCreated = false;
    global::mainwindow::tabSwitcher::libraryWidgetSelected = false;

    resetIcons();
    setBatteryIcon();
    if(global::mainwindow::tabSwitcher::repaint == true) {
        this->repaint();
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
}

void MainWindow::setBatteryIcon() {
    // Battery
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n873\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n") {
        // Hide brightness controls; they won't be very useful there anyway (for anything but the Glo (HD)/Libra/Aura 2) ...
        if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
            ui->brightnessBtn->hide();
            ui->line_7->hide();
        }

        // Setting icons up
        stdIconWidth = sW / 16;
        stdIconHeight = sH / 16;

        QPixmap chargingPixmap(":/resources/battery_charging.png");
        QPixmap scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap fullPixmap(":/resources/battery_full.png");
        QPixmap scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap halfPixmap(":/resources/battery_half.png");
        QPixmap scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap emptyPixmap(":/resources/battery_empty.png");
        QPixmap scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);

        // Checking battery level and status, then displaying the relevant icon on batteryIconLabel
        string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
        if(checkconfig_str_val == "Charging\n") {
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
        QPixmap scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap fullPixmap(":/resources/battery_full.png");
        QPixmap scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap halfPixmap(":/resources/battery_half.png");
        QPixmap scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap emptyPixmap(":/resources/battery_empty.png");
        QPixmap scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);

        // Checking battery level and status, then displaying the relevant icon on batteryIconLabel
        string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
        if(checkconfig_str_val == "Charging\n") {
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
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n873\n") {
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
    if(checkconfig("/tmp/oobe-inkbox_completed") == true or checkconfig("/tmp/inkbox-cinematicBrightness_ran") == true) {
        // Coming from OOBE setup; not doing that fancy stuff again ;p
        QFile::remove("/tmp/oobe-inkbox_completed");
        pre_set_brightness(brightness_value);
    }
    else {
        // Fancy brightness fade-in
        string_writeconfig("/tmp/inkbox-cinematicBrightness_ran", "true");
        cinematicBrightness(brightness_value, 0);
    }
}

void MainWindow::refreshScreen() {
    this->repaint();
}

void MainWindow::setupSearchDialog() {
    if(global::forbidOpenSearchDialog == false) {
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

int MainWindow::testPing() {
    QString pingProg = "ping";
    QStringList pingArgs;
    pingArgs << "-c" << "1" << "1.1.1.1";
    QProcess *pingProcess = new QProcess();
    pingProcess->start(pingProg, pingArgs);
    pingProcess->waitForFinished();
    int exitCode = pingProcess->exitCode();
    pingProcess->deleteLater();
    return exitCode;
}

void MainWindow::updateWifiIcon(int mode) {
    /* Usage:
     * mode 0: auto
     * mode 1: off
     * mode 2: standby
     * mode 3: connected
    */
    if(mode == 0) {
        QTimer *wifiIconTimer = new QTimer(this);
        wifiIconTimer->setInterval(60000);
        connect(wifiIconTimer, SIGNAL(timeout()), this, SLOT(setWifiIcon()));
        wifiIconTimer->start();
    }
    if(mode == 1) {
        ui->wifiBtn->setIcon(QIcon(":/resources/wifi-off.png"));
        ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
    }
    if(mode == 2) {
        ui->wifiBtn->setIcon(QIcon(":/resources/wifi-standby.png"));
        ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
    }
    if(mode == 3) {
        ui->wifiBtn->setIcon(QIcon(":/resources/wifi-connected.png"));
        ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
    }
}

bool MainWindow::checkWifiState() {
    /* Return value:
     * true: interface UP
     * false: interface DOWN
    */
    if(readFile("/opt/inkbox_device") == "n437\n") {
        string_checkconfig_ro("/sys/class/net/wlan0/operstate");
    }
    else {
        string_checkconfig_ro("/sys/class/net/eth0/operstate");
    }

    if(checkconfig_str_val == "up\n") {
        return true;
    }
    else {
        return false;
    }
}

void MainWindow::setWifiIcon() {
    if(checkconfig("/run/wifi_able") == true) {
        global::device::isWifiAble = true;
        if(checkWifiState() == true) {
            if(testPing() == 0) {
                ui->wifiBtn->setIcon(QIcon(":/resources/wifi-connected.png"));
                ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
            }
            else {
                ui->wifiBtn->setIcon(QIcon(":/resources/wifi-standby.png"));
                ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
            }
        }
        else {
            ui->wifiBtn->setIcon(QIcon(":/resources/wifi-off.png"));
            ui->wifiBtn->setIconSize(QSize(wifiIconWidth, wifiIconHeight));
        }
    }
    else {
        global::device::isWifiAble = false;
        ui->wifiBtn->hide();
        ui->line_9->hide();
    }
}

void MainWindow::openWifiDialog() {
    global::toast::wifiToast = true;
    showToast("Searching for networks");
}

void MainWindow::on_wifiBtn_clicked()
{
    openWifiDialog();
}

void MainWindow::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(toastWindow, SIGNAL(updateWifiIconSig(int)), SLOT(updateWifiIcon(int)));
    connect(toastWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    connect(toastWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
    connect(toastWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
    toastWindow->show();

    if(messageToDisplay == "Connection successful") {
        // Give the toast some time to vanish away, then launch OTA updater
        QTimer::singleShot(5000, this, SLOT(launchOtaUpdater()));
    }
}

void MainWindow::hello(int testNumber) {
    qDebug() << "Hello" << testNumber;
}

void MainWindow::closeIndefiniteToast() {
    // Warning: use with caution
    toastWindow->close();
}

void MainWindow::openUpdateDialogOTA(bool open) {
    if(open == true) {
        global::otaUpdate::isUpdateOta = true;
        openUpdateDialog();
    }
    else {
        ;
    }
}

void MainWindow::launchOtaUpdater() {
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
    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(readerWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
    readerWindow->showFullScreen();
}

void MainWindow::checkForUpdate() {
    if(checkconfig("/mnt/onboard/onboard/.inkbox/can_update") == true) {
        if(checkconfig("/tmp/cancelUpdateDialog") == false) {
            // I'm sorry.
            qDebug() << "An update is available.";
            QTimer::singleShot(2000, this, SLOT(openUpdateDialog()));
        }
        else {
            qDebug() << "Not showing update dialog, user dismissed it...";
        }
    }
}

void MainWindow::openEncfsRepackDialog() {
    global::encfs::repackDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}

void MainWindow::on_libraryButton_clicked()
{
    resetFullWindowException = false;
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
    else {
        ;
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

void MainWindow::setRecentBooksLabelsTruncateTreshold() {
    if(readFile("/opt/inkbox_device") == "n705\n" or readFile("/opt/inkbox_device") == "n905b\n" or readFile("/opt/inkbox_device") == "n905c\n") {
        truncateTreshold = 12;
    }
    else if(readFile("/opt/inkbox_device") == "n613\n" or readFile("/opt/inkbox_device") == "n873\n" or readFile("/opt/inkbox_device") == "n236\n" or readFile("/opt/inkbox_device") == "n437\n"){
        truncateTreshold = 20;
    }
    else {
        truncateTreshold = 12;
    }
}
