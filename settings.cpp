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
    settings::setFont(QFont("u001"));
    ui->comboBox->setFont(QFont("u001"));
    ui->sleepTimeoutComboBox->setFont(QFont("u001"));
    ui->aboutBtn->setFont(QFont("u001"));
    ui->okBtn->setFont(QFont("Inter"));

    ui->setPasscodeBtn->setProperty("type", "borderless");
    ui->okBtn->setProperty("type", "borderless");
    ui->aboutBtn->setProperty("type", "borderless");
    ui->requestLeaseBtn->setProperty("type", "borderless");
    ui->usbmsBtn->setProperty("type", "borderless");
    ui->updateBtn->setProperty("type", "borderless");
    ui->resetBtn->setProperty("type", "borderless");
    ui->showSystemInfoBtn->setProperty("type", "borderless");
    ui->generateSystemReportBtn->setProperty("type", "borderless");
    ui->checkOtaUpdateBtn->setProperty("type", "borderless");
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->repackBtn->setProperty("type", "borderless");
    ui->label->setStyleSheet("font-size: 10.5pt; font-weight: bold");
    ui->okBtn->setStyleSheet("font-weight: bold");
    ui->aboutBtn->setStyleSheet("font-size: 9pt");
    ui->requestLeaseBtn->setStyleSheet("font-size: 9pt");
    ui->usbmsBtn->setStyleSheet("font-size: 9pt");
    ui->updateBtn->setStyleSheet("font-size: 9pt");
    ui->resetBtn->setStyleSheet("font-size: 9pt");
    ui->showSystemInfoBtn->setStyleSheet("font-size: 9pt");
    ui->generateSystemReportBtn->setStyleSheet("font-size: 9pt");
    ui->checkOtaUpdateBtn->setStyleSheet("font-size: 9pt");
    ui->comboBox->setStyleSheet("font-size: 9pt");
    ui->sleepTimeoutComboBox->setStyleSheet("font-size: 9pt");
    ui->setPasscodeBtn->setStyleSheet("font-size: 9pt");
    ui->repackBtn->setStyleSheet("font-size: 9pt");

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
    defineDefaultPageSize(0);
    defineDefaultPageSize(1);

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

    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "n306\n") {
        if(checkconfig(".config/10-dark_mode/config") == true) {
            ui->darkModeCheckBox->click();
        }
    }
    else {
        ui->darkModeCheckBox->hide();
        ui->darkModeCheckBox->deleteLater();
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

    // QTextEdit scroll bar in Reader framework
    if(checkconfig(".config/14-reader_scrollbar/config") == true) {
        ui->readerScrollBarCheckBox->click();
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
        else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n306\n") {
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
        else if(checkconfig_str_val == "n437\n") {
            if(dpi_number == 275) {
                ui->uiScalingSlider->setValue(0);
            }
            if(dpi_number == 290) {
                ui->uiScalingSlider->setValue(1);
            }
            if(dpi_number == 305) {
                ui->uiScalingSlider->setValue(2);
            }
        }
        else if(checkconfig_str_val == "n873\n") {
            if(dpi_number == 285) {
                ui->uiScalingSlider->setValue(0);
            }
            if(dpi_number == 300) {
                ui->uiScalingSlider->setValue(1);
            }
            if(dpi_number == 315) {
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
    string_checkconfig_ro(".config/04-book/refresh");
    if(checkconfig_str_val == "") {
        // Set default option, 3
        string_writeconfig(".config/04-book/refresh", "3");
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
    }

    // Sleep mode timeout
    string_checkconfig_ro(".config/15-sleep_timeout/config");
    if(checkconfig_str_val == "") {
        // Set default option, '15 minutes'
        string_writeconfig(".config/15-sleep_timeout/config", "15");
        ui->sleepTimeoutComboBox->setCurrentText("15 minutes");
    }
    else {
        int sleepTimeoutInt = checkconfig_str_val.toInt();
        if(sleepTimeoutInt == -1) {
            ui->sleepTimeoutComboBox->setCurrentText("Never");
        }
        if(sleepTimeoutInt == 2) {
            ui->sleepTimeoutComboBox->setCurrentText("2 minutes");
        }
        if(sleepTimeoutInt == 5) {
            ui->sleepTimeoutComboBox->setCurrentText("5 minutes");
        }
        if(sleepTimeoutInt == 10) {
            ui->sleepTimeoutComboBox->setCurrentText("10 minutes");
        }
        if(sleepTimeoutInt == 15) {
            ui->sleepTimeoutComboBox->setCurrentText("15 minutes");
        }
        if(sleepTimeoutInt == 30) {
            ui->sleepTimeoutComboBox->setCurrentText("30 minutes");
        }
        if(sleepTimeoutInt == 60) {
            ui->sleepTimeoutComboBox->setCurrentText("60 minutes");
        }
    }

    // Lock screen
    if(checkconfig(".config/12-lockscreen/config") == true) {
        ui->enableLockscreenCheckBox->click();
    }

    // Global reading settings
    if(checkconfig(".config/16-global_reading_settings/config") == true) {
        ui->globalReadingSettingsCheckBox->click();
    }

    // Encrypted storage
    if(checkconfig(".config/18-encrypted_storage/status") == true) {
        ui->enableEncryptedStorageCheckBox->click();
    }
    else {
        // Next interaction will be by the user
        enableEncryptedStorageUserChange = true;
    }
    if(getEncFSStatus() == false) {
        ui->repackLabel->hide();
        ui->repackBtn->hide();
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

    if(global::device::isWifiAble == false) {
        ui->checkOtaUpdateLabel->hide();
        ui->checkOtaUpdateBtn->hide();
        ui->checkOtaUpdateLabel->deleteLater();
        ui->checkOtaUpdateBtn->deleteLater();
        ui->checkOtaUpdateGridLayout->deleteLater();
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
        aboutmsg.prepend("<font face='u001'>");
        string_checkconfig_ro("/external_root/opt/isa/version");
        aboutmsg.append("<br><br>InkBox ");
        aboutmsg.append(checkconfig_str_val);
        aboutmsg.append("</font>");
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
    proc->deleteLater();
}

void settings::on_usbmsBtn_clicked()
{
    usbms_launch();
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
    proc->deleteLater();
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
        if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n306\n") {
            string_writeconfig(".config/09-dpi/config", "195");
        }
        if(checkconfig_str_val == "n437\n") {
            string_writeconfig(".config/09-dpi/config", "275");
        }
        if(checkconfig_str_val == "n873\n") {
            string_writeconfig(".config/09-dpi/config", "285");
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
        if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n306\n") {
            string_writeconfig(".config/09-dpi/config", "210");
        }
        if(checkconfig_str_val == "n437\n") {
            string_writeconfig(".config/09-dpi/config", "290");
        }
        if(checkconfig_str_val == "n873\n") {
            string_writeconfig(".config/09-dpi/config", "300");
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
        if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n306\n") {
            string_writeconfig(".config/09-dpi/config", "225");
        }
        if(checkconfig_str_val == "n437\n") {
            string_writeconfig(".config/09-dpi/config", "305");
        }
        if(checkconfig_str_val == "n873\n") {
            string_writeconfig(".config/09-dpi/config", "315");
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

    // We setup the dialog
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
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
        else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n306\n") {
            string_writeconfig(".config/09-dpi/config", "195");
        }
        else if(checkconfig_str_val == "n437\n") {
            string_writeconfig(".config/09-dpi/config", "275");
        }
        else if(checkconfig_str_val == "n873\n") {
            string_writeconfig(".config/09-dpi/config", "285");
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
    global::usbms::usbmsDialog = false;
    global::text::textBrowserContents = global::systemInfoText;
    global::text::textBrowserDialog = true;

    // Show a system info dialog
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
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

void settings::on_readerScrollBarCheckBox_toggled(bool checked)
{
    if(checked == true) {
        string_writeconfig(".config/14-reader_scrollbar/config", "true");
    }
    else {
        string_writeconfig(".config/14-reader_scrollbar/config", "false");
    }
}

void settings::on_sleepTimeoutComboBox_currentIndexChanged(const QString &arg1)
{
    setDefaultWorkDir();
    if(arg1 == "Never") {
        string_writeconfig(".config/15-sleep_timeout/config", "-1");
    }
    if(arg1 == "2 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "2");
    }
    if(arg1 == "5 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "5");
    }
    if(arg1 == "10 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "10");
    }
    if(arg1 == "15 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "15");
    }
    if(arg1 == "30 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "30");
    }
    if(arg1 == "60 minutes") {
        string_writeconfig(".config/15-sleep_timeout/config", "60");
    }
}

void settings::brightnessDown() {
    cinematicBrightness(0, 1);
}

void settings::on_globalReadingSettingsCheckBox_toggled(bool checked)
{
    if(checked == true) {
        checked_box = true;
        writeconfig(".config/16-global_reading_settings/config", "GlobalReadingSettings=");
    }
    else {
        checked_box = false;
        writeconfig(".config/16-global_reading_settings/config", "GlobalReadingSettings=");
    }
}

void settings::on_checkOtaUpdateBtn_clicked()
{
    launchOtaUpdater();
}

void settings::openUpdateDialog() {
    global::mainwindow::updateDialog = true;
    // Write to a temporary file to show an "Update" prompt
    string_writeconfig("/inkbox/updateDialog", "true");

    // Show the dialog
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
    connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToastNative()));
    QApplication::processEvents();
}

void settings::launchOtaUpdater() {
    global::toast::modalToast = true;
    global::toast::indefiniteToast = true;
    emit showToast("Checking for updates");
    otaManagerWindow = new otaManager(this);
    connect(otaManagerWindow, SIGNAL(canOtaUpdate(bool)), SLOT(openUpdateDialogOTA(bool)));
    otaManagerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void settings::openUpdateDialogOTA(bool open) {
    emit closeIndefiniteToast();
    if(open == true) {
        global::otaUpdate::isUpdateOta = true;
        openUpdateDialog();
    }
    else {
        emit showToast("No update available");
    }
}

void settings::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

void settings::closeIndefiniteToastNative() {
    emit closeIndefiniteToast();
}

void settings::usbms_launch()
{
    global::usbms::launchUsbms = true;

    usbmsWindow = new usbms_splash();
    usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
    usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    usbmsWindow->show();
}

void settings::quit_restart() {
    // If existing, cleaning bookconfig_mount mountpoint
    string_writeconfig("/opt/ibxd", "bookconfig_unmount\n");

    // Restarting InkBox
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void settings::on_enableEncryptedStorageCheckBox_toggled(bool checked)
{
    if(checked == true) {
        if(enableEncryptedStorageUserChange == true) {
            setDefaultWorkDir();
            string_writeconfig(".config/18-encrypted_storage/initial_setup_done", "false");
            string_writeconfig(".config/18-encrypted_storage/status", "true");
            if(QFile::exists(".config/18-encrypted_storage/storage_list")) {
                QFile::remove(".config/18-encrypted_storage/storage_list");
            }
            global::settings::settingsRebootDialog = true;
            global::encfs::enableStorageEncryptionDialog = true;
            generalDialogWindow = new generalDialog(this);
            generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        }
        else {
            enableEncryptedStorageUserChange = true;
        }
    }
    else {
        global::encfs::disableStorageEncryptionDialog = true;
        generalDialogWindow = new generalDialog(this);
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(generalDialogWindow, SIGNAL(cancelDisableStorageEncryption()), SLOT(cancelDisableStorageEncryption()));
        connect(generalDialogWindow, SIGNAL(disableStorageEncryption()), SLOT(disableStorageEncryption()));
    }
}

void settings::disableStorageEncryption() {
    setDefaultWorkDir();
    string_writeconfig("/external_root/run/encfs_stop_cleanup", "true");
    string_writeconfig("/opt/ibxd", "encfs_stop\n");
    QThread::msleep(5000);

    string_writeconfig(".config/18-encrypted_storage/status", "false");
    QFile::remove(".config/18-encrypted_storage/initial_setup_done");
    QFile::remove(".config/18-encrypted_storage/storage_list");
    QFile::remove("/mnt/onboard/onboard/data.encfs");

    QDir dumpDir("/mnt/onboard/onboard/encfs-dropbox");
    dumpDir.removeRecursively();
    QDir decDir("/mnt/onboard/onboard/encfs-decrypted");
    decDir.removeRecursively();

    global::settings::settingsRebootDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void settings::cancelDisableStorageEncryption() {
    enableEncryptedStorageUserChange = false;
    ui->enableEncryptedStorageCheckBox->click();
}

void settings::on_repackBtn_clicked()
{
    QDir dir("/mnt/onboard/onboard/encfs-dropbox");
    if(dir.isEmpty()) {
        global::encfs::errorNoBooksInDropboxDialog = true;
        generalDialogWindow = new generalDialog(this);
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    }
    else {
        string_writeconfig("/external_root/run/encfs_repack", "true");
        quit_restart();
    }
}

void settings::on_generateSystemReportBtn_clicked()
{
    string_writeconfig("/opt/ibxd", "generate_system_report\n");
    while(true) {
        if(QFile::exists("/inkbox/systemReportDone")) {
            if(checkconfig("/inkbox/systemReportDone") == true) {
                emit showToast("System report generated successfully");
            }
            else {
                emit showToast("Error in generating system report");
            }
            QFile::remove("/inkbox/systemReportDone");
            break;
        }
    }
}
