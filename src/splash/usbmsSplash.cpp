#include "usbmsSplash.h"
#include "ui_usbmsSplash.h"

#include <QPixmap>
#include <QScreen>
#include <QTimer>

#include "functions.h"

usbmsSplash::usbmsSplash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usbmsSplash)
{
    ui->setupUi(this);
    usbmsSplash::setFont(QFont("u001"));
    ui->label->setFont(QFont("Inter"));

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    if(global::kobox::showKoboxSplash == true) {
        float stdIconWidth = sW / 1.30;
        float stdIconHeight = sH / 1.30;

        // Stylesheet
        QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
        stylesheetFile.open(QFile::ReadOnly);
        this->setStyleSheet(stylesheetFile.readAll());
        stylesheetFile.close();

        ui->label->setFont(QFont("Inter"));
        ui->label->setText("Launching KoBox subsystem");
        ui->label->setStyleSheet("font-size: 14pt; font-weight: bold");
        ui->label_3->setText("Please wait, this could take a while.");
        if(global::deviceID == "n905\n" or global::deviceID == "kt\n") {
            ui->label_3->setStyleSheet("font-size: 11pt");
        }
        else {
            ui->label_3->setStyleSheet("font-size: 10pt");
        }

        QPixmap pixmap(":/resources/kobox-icon.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_2->setPixmap(scaledPixmap);
    }
    else if(global::reader::showKoreaderSplash == true) {
        float stdIconWidth = sW / 1.50;
        float stdIconHeight = sH / 1.50;

        // Stylesheet
        QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
        stylesheetFile.open(QFile::ReadOnly);
        this->setStyleSheet(stylesheetFile.readAll());
        stylesheetFile.close();

        ui->label->setFont(QFont("Inter"));
        ui->label->setText("Launching KOReader subsystem");
        ui->label->setStyleSheet("font-size: 14pt; font-weight: bold");
        ui->label_3->setText("Please wait, this could take a while.");
        if(global::deviceID == "n905\n" or global::deviceID == "kt\n") {
            ui->label_3->setStyleSheet("font-size: 11pt");
        }
        else {
            ui->label_3->setStyleSheet("font-size: 10pt");
        }

        QPixmap pixmap(":/resources/koreader.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_2->setPixmap(scaledPixmap);
    }
    else {
        float stdIconWidth = sW / 1.15;
        float stdIconHeight = sH / 1.15;

        this->setStyleSheet("background-color:black;");
        ui->label->setStyleSheet("QLabel { background-color : black; color : white; font-size: 15pt; font-weight: bold }");
        ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 10pt }");

        QPixmap pixmap(":/resources/usbms.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_2->setPixmap(scaledPixmap);
    }

    if(global::usbms::launchUsbms == true) {
        global::usbms::launchUsbms = false;
        usbmsLaunch();
    }
}

void usbmsSplash::usbmsLaunch()
{
    log("Entering USBMS session", className);
    writeFile("/tmp/in_usbms", "true");
    QTimer::singleShot(1500, this, SLOT(brightnessDown()));
    QTimer::singleShot(1500, this, SLOT(warmthDown()));

    if(global::usbms::koboxExportExtensions == true) {
        writeFile("/opt/ibxd", "kobox_extensions_storage_unmount\n");
    }
    if(checkconfig("/external_root/run/encfs_mounted") == true) {
        writeFile("/external_root/run/encfs_stop_cleanup", "true");
        writeFile("/opt/ibxd", "encfs_stop\n");
        while(true) {
            if(QFile::exists("/tmp/encfs_stopped")) {
                QFile::remove("/tmp/encfs_stopped");
                break;
            }
            QThread::msleep(500);
        }
    }

    writeFile("/opt/ibxd", "gui_apps_stop\n");
    while(true) {
        if(QFile::exists("/tmp/gui_apps_stopped")) {
            QFile::remove("/tmp/gui_apps_stopped");
            break;
        }
        QThread::msleep(500);
    }

    writeFile("/opt/ibxd", "onboard_unmount\n");
    while(true) {
        if(QFile::exists("/tmp/onboard_unmounted")) {
            QFile::remove("/tmp/onboard_unmounted");
            break;
        }
        QThread::msleep(500);
    }

    writeFile("/opt/ibxd", "usbnet_stop\n");
    while(true) {
        if(QFile::exists("/tmp/usbnet_stopped")) {
            QFile::remove("/tmp/usbnet_stopped");
            break;
        }
        QThread::msleep(500);
    }

    if(global::realDeviceID == "n306\n" or global::realDeviceID == "n249\n" or global::realDeviceID == "n873\n") {
        QProcess::execute("insmod", QStringList() << "/external_root/lib/modules/fs/configfs/configfs.ko");
        QProcess::execute("insmod", QStringList() << "/external_root/lib/modules/drivers/usb/gadget/libcomposite.ko");
        QProcess::execute("insmod", QStringList() << "/external_root/lib/modules/drivers/usb/gadget/function/usb_f_mass_storage.ko");
    }
    else if(global::realDeviceID == "kt\n") {
        QProcess::execute("insmod", QStringList() << "/external_root/lib/modules/2.6.35-inkbox/kernel/drivers/usb/gadget/arcotg_udc.ko");
    }

    QString prog_1 ("insmod");
    QStringList args_1;

    if(global::realDeviceID == "kt\n") {
        massStorageModule = "/external_root/lib/modules/2.6.35-inkbox/kernel/drivers/usb/gadget/g_file_storage.ko";
    }
    else if(global::realDeviceID == "n418\n") {
        massStorageModule = "/external_root/lib/modules/4.1.15-inkbox/kernel/g_mass_storage.ko";
    }
    else {
        massStorageModule = "/external_root/lib/modules/g_mass_storage.ko";
    }

    if(global::usbms::koboxExportExtensions == true) {
        args_1 << massStorageModule << "file=/external_root/opt/storage/X11/extensions-user.img" << "removable=y" << "stall=0";
    }
    else {
        args_1 << massStorageModule << "file=/external_root/opt/storage/onboard" << "removable=y" << "stall=0";
    }

    QProcess *proc_1 = new QProcess();
    proc_1->start(prog_1, args_1);
    proc_1->waitForFinished();
    proc_1->deleteLater();

    bool exitUsbMsDone = false;
    QTimer *usbms_t = new QTimer(this);
    usbms_t->setInterval(1000);
    connect(usbms_t, &QTimer::timeout, [&]() {
        if(exitUsbMsDone == false) {
            if(isUsbPluggedIn() == 0) {
                if(global::usbms::koboxExportExtensions == true) {
                    reboot(false);
                    qApp->quit();
                }
                else {
                    log("Exiting USBMS session", className);
                    // '<font/>' bit: because nothing else works ...
                    ui->label->setText("<font face='Inter'>Processing content</font>");
                    ui->label->setFont(QFont("Inter"));
                    ui->label_3->setText("Please wait");
                    ui->label_3->setFont(QFont("u001"));
                    ui->label->setStyleSheet("QLabel { background-color : black; color : white; font-size: 15pt; font-weight: bold }");
                    ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 11pt }");
                    ui->label->setFont(QFont("u001"));

                    float stdIconWidth = sW / 2;
                    float stdIconHeight = sH / 2;

                    QPixmap pixmap(":/resources/clock-inverted.png");
                    QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    ui->label_2->setPixmap(scaledPixmap);

                    this->repaint();

                    QString prog("rmmod");
                    QStringList args;
                    args << massStorageModule;
                    QProcess * proc = new QProcess();
                    proc->start(prog, args);
                    proc->waitForFinished();
                    proc->deleteLater();

                    QTimer::singleShot(500, this, SLOT(restartServices()));
                }
                exitUsbMsDone = true;
            }
            else {
                ;
            }
        }
    } );
    usbms_t->start();
}

usbmsSplash::~usbmsSplash()
{
    delete ui;
}

void usbmsSplash::brightnessDown() {
    cinematicBrightness(0, 1);
    writeFile("/tmp/inkbox-cinematicBrightness_ran", "false");
}

void usbmsSplash::warmthDown() {
    if(global::deviceID == "n249\n" or global::deviceID == "n873\n") {
        cinematicWarmth(0);
    }
}

void usbmsSplash::quit_restart() {
    // If existing, cleaning bookconfig_mount mountpoint
    writeFile("/opt/ibxd", "bookconfig_unmount\n");

    // Restarting InkBox
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void usbmsSplash::restartServices() {
    // Restarting USBNet
    // NOTE: USBNet is only started if required conditions are met (see https://github.com/Kobo-InkBox/rootfs/blob/master/etc/init.d/usbnet)
    writeFile("/opt/ibxd", "usbnet_start\n");
    while(true) {
        if(QFile::exists("/tmp/usbnet_started")) {
            QFile::remove("/tmp/usbnet_started");
            break;
        }
        QThread::msleep(500);
    }

    // Mounting onboard storage
    writeFile("/opt/ibxd", "onboard_mount\n");
    while(true) {
        if(QFile::exists("/tmp/onboard_mounted")) {
            QFile::remove("/tmp/onboard_mounted");
            break;
        }
        QThread::msleep(500);
    }

    // Checking for updates
    writeFile("/opt/ibxd", "update_inkbox_restart\n");
    while(true) {
        if(QFile::exists("/tmp/update_inkbox_restarted")) {
            QFile::remove("/tmp/update_inkbox_restarted");
            break;
        }
        QThread::msleep(500);
    }

    QFile::remove("/tmp/in_usbms");
    // GUI apps: update main JSON file
    writeFile("/opt/ibxd", "gui_apps_start\n");
    while(true) {
        if(QFile::exists("/tmp/gui_apps_started")) {
            if(checkconfig("/tmp/gui_apps_started") == true) {
                QFile::remove("/tmp/gui_apps_started");
                updateUserAppsMainJsonFile();
                break;
            }
            else {
                log("GUI apps service failed to start", className);
                QFile::remove("/tmp/gui_apps_started");
                break;
            }
        }
        QThread::msleep(500);
    }
    // Remove macOS dotfiles
    {
        QString prog("busybox-initrd");
        QStringList args;
        args << "find" << "/mnt/onboard/onboard" << "-type" << "f" << "-name" << "._*" << "-delete";
        QProcess * proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();
        proc->deleteLater();
    }
    // Re-generate local library on next launch
    QFile::remove(global::localLibrary::databasePath);

    quit_restart();
}
