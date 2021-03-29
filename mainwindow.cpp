#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
#include "apps.h"
#include "ui_apps.h"
#include <QTime>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <string>
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QPixmap>
#include <QScreen>

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

    ui->settingsBtn->setText("");
    ui->appsBtn->setText("");
    ui->pushButton->setText("");
    ui->quitBtn->setText("");
    ui->searchBtn->setText("");
    ui->brightnessBtn->setText("");

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the default icon size will be

    float stdIconWidth;
    float stdIconHeight;
    float brightnessIconWidth;
    float brightnessIconHeight;

    string_checkconfig("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n") {
        stdIconWidth = sW / 12;
        stdIconHeight = sH / 12;
        brightnessIconWidth = sW / 30;
        brightnessIconHeight = sH / 30;
    }
    else {
        stdIconWidth = sW / 14;
        stdIconHeight = sH / 14;
        brightnessIconWidth = sW / 28;
        brightnessIconHeight = sH / 28;
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

    ui->brightnessBtn->setIcon(QIcon(":/resources/frontlight.png"));
    ui->brightnessBtn->setIconSize(QSize(brightnessIconWidth, brightnessIconHeight));

    ui->book1Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book2Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book3Btn->setStyleSheet("font-size: 11pt; padding: 25px");
    ui->book4Btn->setStyleSheet("font-size: 11pt; padding: 25px");

    ui->brightnessBtn->setStyleSheet("font-size: 9pt; padding-bottom: 5px; padding-top: 5px; padding-left: 10px; padding-right: 10px;");
    ui->batteryIcon->setStyleSheet("font-size: 5pt");
    ui->batteryIcon->setText("");

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
    // This is a BIG security flaw. I'll try to find something that could replace it but in a safer way.
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
            QString rootfs_prog ("chroot");
            QStringList rootfs_args;
            rootfs_args << "/external_root" << "/tmp/rootfs.sh";
            // Removing script
            QFile::remove("/mnt/onboard/onboard/.inkbox/rootfs.sh");
            QProcess *rootfs_proc = new QProcess();
            rootfs_proc->start(rootfs_prog, rootfs_args);
            rootfs_proc->waitForFinished();

            // Second script
            QString rootfs_internal_prog ("sh");
            QStringList rootfs_internal_args;
            rootfs_internal_args << "/tmp/rootfs-internal.sh";
            // Removing script
            QFile::remove("/mnt/onboard/onboard/.inkbox/rootfs-internal.sh");
            QProcess *rootfs_internal_proc = new QProcess();
            rootfs_internal_proc->start(rootfs_internal_prog, rootfs_internal_args);
            rootfs_internal_proc->waitForFinished();

            // Rebooting if needed
            if(reboot_after_update == true) {
                QString rebootProg("busybox");
                QStringList rebootArgs;
                rebootArgs << "reboot";
                QProcess *rebootProc = new QProcess();
                rebootProc->start(rebootProg, rebootArgs);
                rebootProc->waitForFinished();
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
    checked_box = false;
    writeconfig("/inkbox/skip_opendialog", "SkipOpenDialog=");

    // Demo setting, changes "Welcome to InkBox" label to "InkBox"
    if(checkconfig(".config/01-demo/config") == true) {
        ui->inkboxLabel->setText("InkBox");
    }

    // Dark mode: specfic to InkBox OS, but could be ported to Kobos running Nickel in the future
    if(checkconfig("/opt/inkbox_genuine") == true) {
        if(checkconfig(".config/10-dark_mode/config") == true) {
            string_writeconfig("/tmp/invertScreen", "y");
        }
        else {
            string_writeconfig("/tmp/invertScreen", "n");
        }
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

    // We set the brightness level saved in the config file
    int brightness_value = brightness_checkconfig(".config/03-brightness/config");
    set_brightness(brightness_value);

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
        ui->quoteLabel->hide();
        ui->quotePictureLabel->hide();
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
        if(ui->book3Btn->text() == ui->book1Btn->text()) {
            ui->book3Btn->hide();
        }
        if(ui->book4Btn->text() == ui->book1Btn->text()) {
            ui->book4Btn->hide();
        }

        if(existing_recent_books == false) {
            ui->recentBooksLabel->show();
        }
        else {
            ui->recentBooksLabel->hide();
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_settingsBtn_clicked()
{
    settingsWindow = new settings();
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->showFullScreen();
}

void MainWindow::on_appsBtn_clicked()
{
    appsWindow = new apps();
    appsWindow->setAttribute(Qt::WA_DeleteOnClose);
    appsWindow->showFullScreen();
}

void MainWindow::on_pushButton_clicked()
{
    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_searchBtn_clicked()
{
    // Testing
    /*alertWindow = new alert();
    alertWindow->setAttribute(Qt::WA_DeleteOnClose);
    alertWindow->showFullScreen();*/
}

void MainWindow::on_quitBtn_clicked()
{
    quitWindow = new quit();
    quitWindow->setAttribute(Qt::WA_DeleteOnClose);
    quitWindow->showFullScreen();
}

void MainWindow::on_book1Btn_clicked()
{
    checked_box = true;
    writeconfig("/inkbox/skip_opendialog", "SkipOpenDialog=");

    string_writeconfig("/inkbox/book_number", "1");

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book2Btn_clicked()
{
    checked_box = true;
    writeconfig("/inkbox/skip_opendialog", "SkipOpenDialog=");

    string_writeconfig("/inkbox/book_number", "2");

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book3Btn_clicked()
{
    checked_box = true;
    writeconfig("/inkbox/skip_opendialog", "SkipOpenDialog=");

    string_writeconfig("/inkbox/book_number", "3");

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}

void MainWindow::on_book4Btn_clicked()
{
    checked_box = true;
    writeconfig("/inkbox/skip_opendialog", "SkipOpenDialog=");

    string_writeconfig("/inkbox/book_number", "4");

    readerWindow = new reader();
    readerWindow->setAttribute(Qt::WA_DeleteOnClose);
    readerWindow->showFullScreen();
}
