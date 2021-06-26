#include "settings.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"
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
#include <QScreen>

using namespace std;

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    ui->setPasscodeBtn->setProperty("type", "borderless");
    ui->okBtn->setProperty("type", "borderless");
    ui->aboutBtn->setProperty("type", "borderless");
    ui->requestLeaseBtn->setProperty("type", "borderless");
    ui->usbmsBtn->setProperty("type", "borderless");
    ui->updateBtn->setProperty("type", "borderless");
    ui->resetBtn->setProperty("type", "borderless");
    ui->showSystemInfoBtn->setProperty("type", "borderless");
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->aboutBtn->setStyleSheet("font-size: 9pt");
    ui->requestLeaseBtn->setStyleSheet("font-size: 9pt");
    ui->usbmsBtn->setStyleSheet("font-size: 9pt");
    ui->updateBtn->setStyleSheet("font-size: 9pt");
    ui->resetBtn->setStyleSheet("font-size: 9pt");
    ui->showSystemInfoBtn->setStyleSheet("font-size: 9pt");
    ui->comboBox->setStyleSheet("font-size: 9pt");
    ui->setPasscodeBtn->setStyleSheet("font-size: 9pt");

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
    ui->updateBtn->hide();
    ui->updateLabel->hide();
    ui->enableLockscreenCheckBox->hide();
    ui->setPasscodeBtn->hide();
    ui->setPasscodeLabel->hide();
    ui->securityLabel->hide();
    ui->line_2->hide();
    ui->uiScaleNumberLabel->hide();
    ui->uiScalingSlider->hide();
    ui->uiScalingLabel->hide();

    // Variables
    defineDefaultPageSize();

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

    // Words number
    string_checkconfig(".config/07-words_number/config");
    if(checkconfig_str_val == "") {
        ;
    }
    else {
        int words_number = checkconfig_str_val.toInt();
        ui->wordsNumber->setValue(words_number);
    }

    // ePUB page size
    if(checkconfig(".config/13-epub_page_size/set") == true) {
        string_checkconfig_ro(".config/13-epub_page_size/width");
        if(checkconfig_str_val != "") {
            int pageWidth = checkconfig_str_val.toInt();
            ui->pageSizeWidthSpinBox->setValue(pageWidth);
        }
        else {
            // Failsafe: setting default
            ui->pageSizeWidthSpinBox->setValue(defaultEpubPageWidth);
        }
        string_checkconfig_ro(".config/13-epub_page_size/height");
        if(checkconfig_str_val != "") {
            int pageHeight = checkconfig_str_val.toInt();
            ui->pageSizeHeightSpinBox->setValue(pageHeight);
        }
        else {
            // Failsafe: setting default
            ui->pageSizeHeightSpinBox->setValue(defaultEpubPageHeight);
        }
    }
    else {
        ui->pageSizeWidthSpinBox->setValue(defaultEpubPageWidth);
        ui->pageSizeHeightSpinBox->setValue(defaultEpubPageHeight);
    }

    // Sticky menubar
    if(checkconfig(".config/11-menubar/sticky") == true) {
        ui->menuBarCheckBox->click();
    }

    // Scaling
    string_checkconfig(".config/09-dpi/config");
    if(checkconfig_str_val == "") {
        ;
    }
    else {
        int dpi_number = checkconfig_str_val.toInt();
        // Checking if it's a Mini, Touch or a Glo
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
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
        else if(checkconfig_str_val == "n905\n") {
            if(dpi_number == 160) {
                ui->uiScalingSlider->setValue(0);
            }
            if(dpi_number == 187) {
                ui->uiScalingSlider->setValue(1);
            }
            if(dpi_number == 200) {
                ui->uiScalingSlider->setValue(2);
            }
        }
        else if(checkconfig_str_val == "n613\n") {
            if(dpi_number == 195) {
                ui->uiScalingSlider->setValue(0);
            }
            if(dpi_number == 210) {
                ui->uiScalingSlider->setValue(1);
            }
            if(dpi_number == 225) {
                ui->uiScalingSlider->setValue(2);
            }
        }
        else {
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
    }

    // Refresh
    string_checkconfig(".config/04-book/refresh");
    if(checkconfig_str_val == "") {
        // Set default option, 3
        string_writeconfig(".config/04-book/refresh", "3");
        string_checkconfig(".config/04-book/refresh");
        ui->comboBox->setCurrentText("3 pages");
    }
    else {
        int refreshInt = checkconfig_str_val.toInt();
        if(refreshInt == -1) {
            ui->comboBox->setCurrentText("Never refresh");
        }
        if(refreshInt == 0) {
            ui->comboBox->setCurrentText("Every page");
        }
        if(refreshInt == 1) {
            ui->comboBox->setCurrentText("1 page");
        }
        if(refreshInt == 2) {
            ui->comboBox->setCurrentText("2 pages");
        }
        if(refreshInt == 3) {
            ui->comboBox->setCurrentText("3 pages");
        }
        if(refreshInt == 4) {
            ui->comboBox->setCurrentText("4 pages");
        }
        if(refreshInt == 5) {
            ui->comboBox->setCurrentText("5 pages");
        }
        if(refreshInt == 6) {
            ui->comboBox->setCurrentText("6 pages");
        }
        else {
            // Something bad (or not ;p) has happened
        }
    }

    // Lock screen
    if(checkconfig(".config/12-lockscreen/config") == true) {
        ui->enableLockscreenCheckBox->click();
    }

    // DPI checkbox
    string_checkconfig(".config/09-dpi/config");
    // Check if the string is a number; else, we don't check the check box
    if(checkconfig_str_val == "false") {
        string_writeconfig(".config/09-dpi/config-enabled", "false");
    }
    else {
        string_writeconfig(".config/09-dpi/config-enabled", "true");
    }
    if(checkconfig(".config/09-dpi/config-enabled") == true) {
        ui_not_user_change = true;
        ui->enableUiScalingCheckBox->click();
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
        ui->securityLabel->show();
        ui->line_2->show();
        ui->enableLockscreenCheckBox->show();
        ui->setPasscodeBtn->show();
        ui->setPasscodeLabel->show();
        ui->usbmsBtn->show();
        ui->label_5->show();
        ui->label_6->show();
        ui->line_7->show();
        ui->okBtn->setText("OK");
    }
    else {
        ui->label_8->setText("Reset InkBox");
    }


    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
}

settings::~settings()
{
    delete ui;
}

void settings::on_okBtn_clicked() {
    // Prevent potential unknown damage launching via shell script this could do
    if(launch_sh == true) {
        if(ui_enable_changed == true) {
            ui_enable_changed = false;
            global::settings::settingsRebootDialog = true;
            generalDialogWindow = new generalDialog();
            generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
            generalDialogWindow->show();
        }
        else {
            QProcess process;
            process.startDetached("inkbox.sh", QStringList());
            qApp->quit();
        }
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
        QString aboutmsg = "InkBox is an open-source, Qt-based eBook reader. It aims to bring you the latest Qt features while being also fast and responsive.";
        string_checkconfig_ro("/external_root/opt/isa/version");
        aboutmsg.append("\n\nInkBox ");
        aboutmsg.append(checkconfig_str_val);
        QMessageBox::information(this, tr("Information"), aboutmsg);
    }
    else {
        QMessageBox::information(this, tr("About"), tr("InkBox is an open-source Qt-based eBook reader. It aims to bring you the latest Qt features while being also fast and responsive."));
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
    usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    usbmsWindow->show();

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
            QString reboot_prog ("/sbin/reboot");
            QStringList reboot_args;
            reboot_args << "no_splash";
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

// Now I know that QStackedWidgets exist... ;p

void settings::on_previousBtn_clicked()
{
    settings_page = settings_page - 1;
    if(settings_page == 1) {
        ui->stackedWidget->setCurrentIndex(0);

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
        }
    }
    else {
        if(settings_page == 2) {
            ui->stackedWidget->setCurrentIndex(1);
            if(checkconfig("/opt/inkbox_genuine") == true) {
                if(checkconfig("/mnt/onboard/onboard/.inkbox/can_update") == true) {
                    ui->updateBtn->show();
                    ui->updateLabel->show();
                }
                else {
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
        ui->stackedWidget->setCurrentIndex(1);

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
    QString prog ("/sbin/reboot");
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
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            string_writeconfig(".config/09-dpi/config", "187");
        }
        if(checkconfig_str_val == "n905\n") {
            string_writeconfig(".config/09-dpi/config", "160");
        }
        if(checkconfig_str_val == "n613\n") {
            string_writeconfig(".config/09-dpi/config", "195");
        }
    }
    if(value == 1) {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            string_writeconfig(".config/09-dpi/config", "214");
        }
        if(checkconfig_str_val == "n905\n") {
            string_writeconfig(".config/09-dpi/config", "187");
        }
        if(checkconfig_str_val == "n613\n") {
            string_writeconfig(".config/09-dpi/config", "210");
        }
    }
    if(value == 2) {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            string_writeconfig(".config/09-dpi/config", "227");
        }
        if(checkconfig_str_val == "n905\n") {
            string_writeconfig(".config/09-dpi/config", "200");
        }
        if(checkconfig_str_val == "n613\n") {
            string_writeconfig(".config/09-dpi/config", "225");
        }
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

void settings::on_menuBarCheckBox_toggled(bool checked)
{
    if(checked == true) {
        checked_box = true;
        writeconfig(".config/11-menubar/sticky", "StickyMenuBar=");
    }
    else {
        checked_box = false;
        writeconfig(".config/11-menubar/sticky", "StickyMenuBar=");
    }
}

void settings::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Every page") {
        string_writeconfig(".config/04-book/refresh", "0");
    }
    if(arg1 == "1 page") {
        string_writeconfig(".config/04-book/refresh", "1");
    }
    if(arg1 == "2 pages") {
        string_writeconfig(".config/04-book/refresh", "2");
    }
    if(arg1 == "3 pages") {
        string_writeconfig(".config/04-book/refresh", "3");
    }
    if(arg1 == "4 pages") {
        string_writeconfig(".config/04-book/refresh", "4");
    }
    if(arg1 == "5 pages") {
        string_writeconfig(".config/04-book/refresh", "5");
    }
    if(arg1 == "6 pages") {
        string_writeconfig(".config/04-book/refresh", "6");
    }
    if(arg1 == "Never refresh") {
        string_writeconfig(".config/04-book/refresh", "-1");
    }
}

void settings::on_resetBtn_clicked()
{
    // We write to a temporary file to show a "Reset" prompt
    string_writeconfig("/inkbox/resetDialog", "true");

    // We show the dialog
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}

void settings::on_setPasscodeBtn_clicked()
{
    string_writeconfig("/tmp/setPasscode", "true");
    QProcess process;
    process.startDetached("lockscreen", QStringList());
    qApp->quit();
}

void settings::on_enableLockscreenCheckBox_toggled(bool checked)
{
    if(checked == true) {
        string_writeconfig(".config/12-lockscreen/config", "true");
    }
    else {
        string_writeconfig(".config/12-lockscreen/config", "false");
    }
}

void settings::on_enableUiScalingCheckBox_toggled(bool checked)
{
    if(checked == true) {
        // Writing default value depending on the device
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            string_writeconfig(".config/09-dpi/config", "187");
        }
        else if(checkconfig_str_val == "n905\n") {
            string_writeconfig(".config/09-dpi/config", "160");
        }
        else if(checkconfig_str_val == "n613\n") {
            string_writeconfig(".config/09-dpi/config", "195");
        }
        else {
            string_writeconfig(".config/09-dpi/config", "187");
        }
        string_writeconfig(".config/09-dpi/config-enabled", "true");
        ui->uiScaleNumberLabel->show();
        ui->uiScalingSlider->show();
        ui->uiScalingLabel->show();
        launch_sh = true;
        if(ui_not_user_change == true) {
            ui_enable_changed = false;
        }
        else {
            ui_enable_changed = true;
        }
    }
    else {
        string_writeconfig(".config/09-dpi/config", "false");
        string_writeconfig(".config/09-dpi/config-enabled", "false");
        ui->uiScaleNumberLabel->hide();
        ui->uiScalingSlider->hide();
        ui->uiScalingLabel->hide();
        launch_sh = true;
        ui_enable_changed = true;
    }
}

void settings::on_showSystemInfoBtn_clicked()
{
    getSystemInfo();
    global::text::textBrowserContents = global::systemInfoText;
    global::text::textBrowserDialog = true;

    // Show a system info dialog
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}

void settings::on_pageSizeWidthSpinBox_valueChanged(int arg1)
{
    std::string value = std::to_string(arg1);
    string_writeconfig(".config/13-epub_page_size/width", value);
    string_writeconfig(".config/13-epub_page_size/set", "true");
}

void settings::on_pageSizeHeightSpinBox_valueChanged(int arg1)
{
    std::string value = std::to_string(arg1);
    string_writeconfig(".config/13-epub_page_size/height", value);
    string_writeconfig(".config/13-epub_page_size/set", "true");
}
