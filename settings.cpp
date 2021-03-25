#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QWidget>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <QProcess>
#include <QTimer>
#include <QIcon>

using namespace std;

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    ui->okBtn->setProperty("type", "borderless");
    ui->aboutBtn->setProperty("type", "borderless");
    ui->requestLeaseBtn->setProperty("type", "borderless");
    ui->usbmsBtn->setProperty("type", "borderless");
    ui->updateBtn->setProperty("type", "borderless");
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->requestLeaseBtn->setStyleSheet("font-size: 9pt");
    ui->usbmsBtn->setStyleSheet("font-size: 9pt");
    ui->updateBtn->setStyleSheet("font-size: 9pt");

    ui->previousBtn->setText("");
    ui->previousBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextBtn->setText("");
    ui->nextBtn->setIcon(QIcon(":/resources/chevron-right.png"));

    ui->requestLeaseBtn->hide();
    ui->usbmsBtn->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->line_3->hide();
    ui->line_7->hide();
    ui->line_9->hide();
    ui->softwareLabel->hide();
    ui->updateBtn->hide();
    ui->updateLabel->hide();
    ui->darkModeCheckBox->hide();
    ui->uiScalingLabel->hide();
    ui->uiScalingSlider->hide();
    ui->uiScaleNumberLabel->hide();

    // Settings tweaking + enabling specific features whether it's running on the provided integrated OS or Kobo firmware
    if(checkconfig(".config/01-demo/config") == true) {
        ui->demoCheckBox->click();
    }
    if(checkconfig(".config/02-clock/config") == true) {
        ui->clockCheckBox->click();
    }
    if(checkconfig(".config/05-quote/config") == true) {
        ui->quoteCheckBox->click();
    }
    if(checkconfig(".config/10-dark_mode/config") == true) {
        ui->darkModeCheckBox->click();
    }
    string_checkconfig(".config/07-words_number/config");
    if(checkconfig_str_val == "") {
        ;
    }
    else {
        int words_number = checkconfig_str_val.toInt();
        ui->wordsNumber->setValue(words_number);
    }
    // Scaling
    string_checkconfig(".config/09-dpi/config");
    if(checkconfig_str_val == "") {
        ;
    }
    else {
        int dpi_number = checkconfig_str_val.toInt();
        if(dpi_number == 187) {
            ui->uiScalingSlider->setValue(0);
        }
        if(dpi_number == 214) {
            ui->uiScalingSlider->setValue(1);
        }
        if(dpi_number == 227) {
            ui->uiScalingSlider->setValue(2);
        }
    }

    if(checkconfig("/opt/inkbox_genuine") == true) {
        // Enforcing security policy if the user has not rooted the device
        if(checkconfig("/external_root/opt/root/rooted") == true) {
            ui->requestLeaseBtn->show();
            ui->label_4->show();
            ui->label_3->show();
            ui->line_3->show();
        }
        else {
            ui->requestLeaseBtn->hide();
            ui->label_4->hide();
            ui->label_3->hide();
            ui->line_3->hide();
        }
        ui->usbmsBtn->show();
        ui->label_5->show();
        ui->label_6->show();
        ui->line_7->show();
        ui->okBtn->setText("OK");
    }


    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    QObject::connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(exitSlot()));
}

void settings::exitSlot() {
    settings::close();
}

settings::~settings()
{
    delete ui;
}

void settings::on_okBtn_clicked() {
    // Prevent potential unknown damage launching via shell script this could do
    if(launch_sh == true) {
        QProcess process;
        process.startDetached("inkbox.sh", QStringList());
        qApp->quit();
    }
    else {
        QProcess process;
        process.startDetached("inkbox", QStringList());
        qApp->quit();
    }
}

void settings::on_aboutBtn_clicked()
{
    if(checkconfig("/opt/inkbox_genuine") == true) {
        QString aboutmsg = "InkBox is an open-source Qt-based eBook reader. It brings you the latest Qt features while being also fast and responsive.";
        string_checkconfig_ro("/external_root/opt/isa/version");
        aboutmsg.append("\n\nInkBox v");
        aboutmsg.append(checkconfig_str_val);
        QMessageBox::information(this, tr("Information"), aboutmsg);
    }
    else {
        QMessageBox::information(this, tr("About"), tr("InkBox is an open-source Qt-based eBook reader. It brings you the latest Qt features while being also fast and responsive."));
    }
}

void settings::on_demoCheckBox_toggled(bool checked)
{
    // Write to config file
    if(checked == true) {
        checked_box = true;
        writeconfig(".config/01-demo/config", "InkboxChangeLabel=");
    }
    else {
        checked_box = false;
        writeconfig(".config/01-demo/config", "InkboxChangeLabel=");
    }
}

void settings::on_clockCheckBox_toggled(bool checked)
{
    // Write to config file
    if(checked == true) {
        checked_box = true;
        writeconfig(".config/02-clock/config", "ClockShowSeconds=");
    }

    else {
        checked_box = false;
        writeconfig(".config/02-clock/config", "ClockShowSeconds=");
    }
}

void settings::on_quoteCheckBox_toggled(bool checked)
{
    if(checked == true) {
        checked_box = true;
        writeconfig(".config/05-quote/config", "DisableQuote=");
    }
    else {
        checked_box = false;
        writeconfig(".config/05-quote/config", "DisableQuote=");
    }
}

void settings::on_requestLeaseBtn_clicked()
{
    QString prog ("chroot");
    QStringList args;
    args << "/external_root" << "/usr/sbin/dhclient";
    QProcess *proc = new QProcess();
    proc->start(prog, args);
    proc->waitForFinished();
}

void settings::on_usbmsBtn_clicked()
{
    QString umount_prog ("umount");
    QStringList umount_args;
    umount_args << "/dev/loop0";
    QProcess *umount_proc = new QProcess();
    umount_proc->start(umount_prog, umount_args);
    umount_proc->waitForFinished();

    QString rmmod ("rmmod");
    QStringList rmmod_args;
    rmmod_args << "g_ether.ko";
    QProcess *rmmod_proc = new QProcess();
    rmmod_proc->start(rmmod, rmmod_args);
    rmmod_proc->waitForFinished();

    QString prog ("insmod");
    QStringList args;
    args << "/external_root/modules/arcotg_udc.ko";
    QProcess *proc = new QProcess();
    proc->start(prog, args);
    proc->waitForFinished();

    QString prog_1 ("insmod");
    QStringList args_1;
    args_1 << "/external_root/modules/g_mass_storage.ko" << "file=/external_root/opt/storage/onboard" << "removable=y" << "stall=0";
    QProcess *proc_1 = new QProcess();
    proc_1->start(prog_1, args_1);
    proc_1->waitForFinished();

    usbmsWindow = new usbms_splash();
    usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
    usbmsWindow->showFullScreen();

    QTimer *usbms_t = new QTimer(this);
    usbms_t->setInterval(1000);
    connect(usbms_t, &QTimer::timeout, [&]() {
        QString prog ("mass_storage.sh");
        QStringList args;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();

        QFile modules("/tmp/usbevent");
        modules.open(QIODevice::ReadWrite);
        QTextStream in (&modules);
        const QString content = in.readAll();
        string contentstr = content.toStdString();
        modules.close();
        if(contentstr.find("1") != std::string::npos) {
            QString reboot_prog ("busybox");
            QStringList reboot_args;
            reboot_args << "reboot";
            QProcess *reboot_proc = new QProcess();
            reboot_proc->start(reboot_prog, reboot_args);
            reboot_proc->waitForFinished();
        }
        else {
            ;
        }
    } );
    usbms_t->start();
}

// Please forgive me, I didn't know that QStackedWidget existed back then...

void settings::on_previousBtn_clicked()
{
    settings_page = settings_page - 1;
    if(settings_page == 1) {
        ui->line_6->show();
        ui->label_2->show();
        ui->demoCheckBox->show();
        ui->quoteCheckBox->show();
        ui->line_8->show();
        ui->label_7->show();
        ui->wordsNumber->show();
        ui->wordsNumberLabel->show();
        ui->clockCheckBox->show();

        ui->line_9->hide();
        ui->softwareLabel->hide();
        ui->updateBtn->hide();
        ui->updateLabel->hide();
        ui->darkModeCheckBox->hide();
        ui->uiScalingLabel->hide();
        ui->uiScalingSlider->hide();
        ui->uiScaleNumberLabel->hide();

        if(checkconfig("/opt/inkbox_genuine") == true) {
            // Enforcing security policy if the user has not rooted the device
            if(checkconfig("/external_root/opt/root/rooted") == true) {
                ui->requestLeaseBtn->show();
                ui->label_4->show();
                ui->label_3->show();
                ui->line_3->show();
            }
            else {
                ui->requestLeaseBtn->hide();
                ui->label_4->hide();
                ui->label_3->hide();
                ui->line_3->hide();
            }
            ui->label_5->show();
            ui->label_6->show();
            ui->line_7->show();
            ui->usbmsBtn->show();
        }
    }
    else {
        if(settings_page == 2) {
            ui->line_6->hide();
            ui->label_2->hide();
            ui->label_7->hide();
            ui->line_8->hide();
            ui->clockCheckBox->hide();
            ui->demoCheckBox->hide();
            ui->quoteCheckBox->hide();
            if(checkconfig("/opt/inkbox_genuine") == true) {
                ui->label_3->hide();
                ui->label_4->hide();
                ui->label_5->hide();
                ui->label_6->hide();
                ui->line_7->hide();
                ui->line_3->hide();

                ui->line_9->show();
                ui->softwareLabel->show();
                ui->darkModeCheckBox->show();

                if(checkconfig("/mnt/onboard/onboard/.inkbox/can_update") == true) {
                    ui->updateBtn->show();
                    ui->updateLabel->show();
                }
            }
        }
        if(settings_page <= 0) {
            // Prevent unwanted accesses
            settings_page = settings_page + 1;
        }
    }
}

void settings::on_nextBtn_clicked()
{
    settings_page = settings_page + 1;
    if(settings_page == 2) {
        ui->line_6->hide();
        ui->label_2->hide();
        ui->label_7->hide();
        ui->line_8->hide();
        ui->demoCheckBox->hide();
        ui->quoteCheckBox->hide();
        ui->clockCheckBox->hide();
        ui->wordsNumber->hide();
        ui->wordsNumberLabel->hide();

        ui->line_9->show();
        ui->softwareLabel->show();
        ui->darkModeCheckBox->show();
        ui->uiScalingLabel->show();
        ui->uiScalingSlider->show();
        ui->uiScaleNumberLabel->show();

        if(checkconfig("/opt/inkbox_genuine") == true) {
            // Enforcing security policy if the user has not rooted the device
            if(checkconfig("/external_root/opt/root/rooted") == true) {
                ui->requestLeaseBtn->show();
                ui->label_4->show();
                ui->label_3->show();
                ui->line_3->show();
            }
            else {
                ui->requestLeaseBtn->hide();
                ui->label_4->hide();
                ui->label_3->hide();
                ui->line_3->hide();
            }
            ui->label_3->hide();
            ui->label_4->hide();
            ui->label_5->hide();
            ui->label_6->hide();
            ui->line_7->hide();
            ui->line_3->hide();
            ui->requestLeaseBtn->hide();
            ui->usbmsBtn->hide();

            if(checkconfig("/mnt/onboard/onboard/.inkbox/can_update") == true) {
                ui->updateBtn->show();
                ui->updateLabel->show();
            }
        }
    }
    if(settings_page >= 3) {
        settings_page = settings_page - 1;
    }
}

void settings::on_wordsNumber_valueChanged(int arg1)
{
    QString number = QString::number(arg1);
    string number_str = number.toStdString();
    string_writeconfig(".config/07-words_number/config", number_str);
}

void settings::on_updateBtn_clicked()
{
    string_writeconfig("/mnt/onboard/onboard/.inkbox/can_really_update", "true");
    string_writeconfig("/external_root/opt/update/will_update", "true");
    string_writeconfig("/external_root/boot/flags/WILL_UPDATE", "true");
    QString prog ("reboot");
    QStringList args;
    QProcess *proc = new QProcess();
    proc->start(prog, args);
    proc->waitForFinished();
}

void settings::on_darkModeCheckBox_toggled(bool checked)
{
    if(checked == true) {
        string_writeconfig(".config/10-dark_mode/config", "true");
        string_writeconfig("/tmp/invertScreen", "y");
    }
    else {
        string_writeconfig(".config/10-dark_mode/config", "false");
        string_writeconfig("/tmp/invertScreen", "n");
    }
}

void settings::on_uiScalingSlider_valueChanged(int value)
{
    if(value == 0) {
        string_writeconfig(".config/09-dpi/config", "187");
    }
    if(value == 1) {
        string_writeconfig(".config/09-dpi/config", "214");
    }
    if(value == 2) {
        string_writeconfig(".config/09-dpi/config", "227");
    }

    // Making sense for the user
    value = value + 1;
    QString value_qstr = QString::number(value);
    ui->uiScaleNumberLabel->setText(value_qstr);

    if(not_user_change == true) {
        launch_sh = false;
        // If value is changed, it will launch inkbox.sh detached when "OK" is pressed.
        not_user_change = false;
    }
    else {
        // This is supposed to happen when the user clicks the slider, and not the software. Refer to setValue() methods for uiScalingSlider in main().
        launch_sh = true;
    }
}
