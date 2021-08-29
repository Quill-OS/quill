#include "usbms_splash.h"
#include "ui_usbms_splash.h"

#include <QPixmap>
#include <QScreen>
#include <QTimer>

#include "functions.h"

usbms_splash::usbms_splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usbms_splash)
{
    ui->setupUi(this);

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    if(global::kobox::showKoboxSplash == true) {
        float stdIconWidth = sW / 1.30;
        float stdIconHeight = sH / 1.30;

        // Stylesheet
        QFile stylesheetFile(":/resources/eink.qss");
        stylesheetFile.open(QFile::ReadOnly);
        this->setStyleSheet(stylesheetFile.readAll());
        stylesheetFile.close();

        ui->label->setText("Launching KoBox subsystem");
        ui->label->setStyleSheet("font-size: 14pt");
        ui->label_3->setText("Please wait, this could take a while.");
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n905\n") {
            ui->label_3->setStyleSheet("font-size: 11pt");
        }
        else {
            ui->label_3->setStyleSheet("font-size: 10pt");
        }

        QPixmap pixmap(":/resources/kobox-icon.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->label_2->setPixmap(scaledPixmap);
    }
    else {
        float stdIconWidth = sW / 1.15;
        float stdIconHeight = sH / 1.15;

        this->setStyleSheet("background-color:black;");
        ui->label->setStyleSheet("QLabel { background-color : black; color : white; }");
        ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 9pt}");

        QPixmap pixmap(":/resources/usbms.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->label_2->setPixmap(scaledPixmap);
    }

    if(global::usbms::launchUsbms == true) {
        global::usbms::launchUsbms = false;
        usbms_launch();
    }
}

void usbms_splash::usbms_launch()
{
    string_writeconfig("/tmp/in_usbms", "true");
    QTimer::singleShot(1500, this, SLOT(brightnessDown()));

    QString umount_prog ("umount");
    QStringList umount_args;
    if(global::usbms::koboxExportExtensions == true) {
        umount_args << "/dev/loop10";
    }
    else {
        umount_args << "/dev/loop0";
    }
    QProcess *umount_proc = new QProcess();
    umount_proc->start(umount_prog, umount_args);
    umount_proc->waitForFinished();
    umount_proc->deleteLater();

    QString rmmod ("rmmod");
    QStringList rmmod_args;
    rmmod_args << "g_ether.ko";
    QProcess *rmmod_proc = new QProcess();
    rmmod_proc->start(rmmod, rmmod_args);
    rmmod_proc->waitForFinished();
    rmmod_proc->deleteLater();

    QString prog ("insmod");
    QStringList args;
    args << "/external_root/modules/arcotg_udc.ko";
    QProcess *proc = new QProcess();
    proc->start(prog, args);
    proc->waitForFinished();
    proc->deleteLater();

    QString prog_1 ("insmod");
    QStringList args_1;

    if(global::usbms::koboxExportExtensions == true) {
        args_1 << "/external_root/modules/g_mass_storage.ko" << "file=/external_root/opt/storage/X11/extensions-user.img" << "removable=y" << "stall=0";
    }
    else {
        args_1 << "/external_root/modules/g_mass_storage.ko" << "file=/external_root/opt/storage/onboard" << "removable=y" << "stall=0";
    }

    QProcess *proc_1 = new QProcess();
    proc_1->start(prog_1, args_1);
    proc_1->waitForFinished();
    proc_1->deleteLater();

    QTimer *usbms_t = new QTimer(this);
    usbms_t->setInterval(1000);
    connect(usbms_t, &QTimer::timeout, [&]() {
        QString prog ("mass_storage.sh");
        QStringList args;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();
        proc->deleteLater();

        QFile modules("/tmp/usbevent");
        modules.open(QIODevice::ReadWrite);
        QTextStream in (&modules);
        const QString content = in.readAll();
        std::string contentstr = content.toStdString();
        modules.close();
        if(contentstr.find("1") != std::string::npos) {
            QString reboot_prog ("/sbin/reboot");
            QStringList reboot_args;
            reboot_args << "no_splash";
            QProcess *reboot_proc = new QProcess();
            reboot_proc->start(reboot_prog, reboot_args);
            reboot_proc->waitForFinished();
            reboot_proc->deleteLater();
        }
        else {
            ;
        }
    } );
    usbms_t->start();
}

usbms_splash::~usbms_splash()
{
    delete ui;
}

void usbms_splash::brightnessDown() {
    cinematicBrightness(0, 1);
}
