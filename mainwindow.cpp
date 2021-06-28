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
    ui->brightnessBtn->setProperty("type", "borderless");
    ui->homeBtn->setProperty("type", "borderless");

    ui->settingsBtn->setText("");
    ui->appsBtn->setText("");
    ui->pushButton->setText("");
    ui->quitBtn->setText("");
    ui->searchBtn->setText("");
    ui->brightnessBtn->setText("");
    ui->homeBtn->setText("");
    ui->quoteLabel->setText("");

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
    }
    else if(checkconfig_str_val == "n905\n") {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 26;
        brightnessIconHeight = sH / 26;
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
    }
    else if(checkconfig_str_val == "n613\n") {
        stdIconWidth = sW / 12.5;
        stdIconHeight = sH / 12.5;
        brightnessIconWidth = sW / 24.5;
        brightnessIconHeight = sH / 24.5;
        homeIconWidth = sW / 18.5;
        homeIconHeight = sW / 18.5;
    }
    else {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 26;
        brightnessIconHeight = sH / 26;
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
    }

    // Setting icons up
    ui->pushButton->setIcon(QIcon(":/resources/book.png"));
    ui->pushButton->setIconSize(QSize(stdIconWidth, stdIconHeight));
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

    // Checking if we have a Mini or Touch there
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        ui->batteryIcon->setStyleSheet("font-size: 5pt; padding-bottom: 0px; padding-top: 0px; padding-left: 1px; padding-right: 1px;");
    }
    else if(checkconfig_str_val == "n613\n") {
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

            // Second script
            QString rootfs_prog ("chroot");
            QStringList rootfs_args;
            rootfs_args << "/external_root" << "/tmp/rootfs.sh";
            // Removing script
            QFile::remove("/mnt/onboard/onboard/.inkbox/rootfs.sh");
            QProcess *rootfs_proc = new QProcess();
            rootfs_proc->start(rootfs_prog, rootfs_args);
            rootfs_proc->waitForFinished();

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
    usbmsPrompt->setInterval(2000);
    connect(usbmsPrompt, &QTimer::timeout, [&]() {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            if(global::usbms::showUsbmsDialog != true) {
                ;
            }
            else {
                string_checkconfig_ro("/sys/devices/platform/fsl-usb2-udc/gadget/suspended");
                if(checkconfig_str_val != "0\n") {
                    // Loop again...
                    ;
                }
                else {
                    // An USB cable is connected!
                    openUsbmsDialog();
                }
            }
        }
        else {
            // Do nothing, we're running along with Nickel & friends...
            ;
        }
    } );
    // **** FEATURE WARNING ****
    // Due to the fsl-usb2-udc/gadget/suspended file which really only changes after a real plug/unplug of the USB cable, further development on this feature is no longer possible and is abandoned unless a fix is found.
    // Problem encontered: it would prompt to connect after a fresh reboot even if no USB cable was plugged in.
    //
    // usbmsPrompt->start();
    //
    // **** FEATURE WARNING ****

    // We set the brightness level saved in the config file
    int brightness_value = brightness_checkconfig(".config/03-brightness/config");
    pre_set_brightness(brightness_value);

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

        // Book 1
        string_checkconfig(".config/08-recent_books/1");
        if(checkconfig_str_val == "") {
            ui->book1Btn->hide();
        }
        else {
            relative_path = checkconfig_str_val.split("/").last();
            relative_path.prepend("  ");
            relative_path.append("  ");
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
            relative_path.prepend("  ");
            relative_path.append("  ");
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
            relative_path.prepend("  ");
            relative_path.append("  ");
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
            relative_path.prepend("  ");
            relative_path.append("  ");
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

    // Write current running version to a file for utility purposes
    {
        string_checkconfig_ro("/external_root/opt/isa/version");
        std::string inkbox_version = checkconfig_str_val.toStdString();
        string_writeconfig("/opt/version", inkbox_version);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openUpdateDialog() {
    global::mainwindow::updateDialog = true;
    // Write to a temporary file to show a "Reset" prompt
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
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::settingsChooserWidgetSelected != true) {
        ui->settingsBtn->setStyleSheet("background: black");
        ui->settingsBtn->setIcon(QIcon(":/resources/settings-inverted.png"));

        // Create widget
        settingsChooserWindow = new settingsChooser();
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
    resetWindow(false);
    if(global::mainwindow::tabSwitcher::appsWidgetSelected != true) {
        ui->appsBtn->setStyleSheet("background: black");
        ui->appsBtn->setIcon(QIcon(":/resources/apps-inverted.png"));

        // Create widget
        appsWindow = new apps();
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
    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_searchBtn_clicked()
{
    // Hopefully this button will do something one day...
    /*
    global::text::textBrowserDialog = true;
    global::text::textBrowserContents = "Hullo";
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
    */
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

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book2Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 2;

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book3Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 3;

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book4Btn_clicked()
{
    global::reader::skipOpenDialog = true;
    global::reader::bookNumber = 4;

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
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

    global::mainwindow::tabSwitcher::appsWidgetCreated = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetCreated = false;
    global::mainwindow::tabSwitcher::appsWidgetSelected = false;
    global::mainwindow::tabSwitcher::settingsChooserWidgetSelected = false;

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
}

void MainWindow::setBatteryIcon() {
    // Battery
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n") {
        // Hide brightness controls; they won't be very useful there anyway (for anything but the Glo) ...
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

void MainWindow::pre_set_brightness(int brightnessValue) {
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
