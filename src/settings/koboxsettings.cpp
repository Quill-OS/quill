#include "koboxsettings.h"
#include "ui_koboxsettings.h"
#include "functions.h"

#include <QFile>
#include <QScreen>

koboxSettings::koboxSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::koboxSettings)
{
    ui->setupUi(this);
    koboxSettings::setFont(QFont("u001"));
    ui->okBtn->setFont(QFont("Inter"));

    // UI tweaks
    ui->okBtn->setProperty("type", "borderless");
    ui->pushButton->setProperty("type", "borderless");
    ui->resetKoboxBtn->setProperty("type", "borderless");
    ui->pushButton->setStyleSheet("font-weight: bold");
    ui->resetKoboxBtn->setStyleSheet("font-weight: bold");
    ui->okBtn->setStyleSheet("font-weight: bold");

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    if(checkconfig("/external_root/boot/flags/X11_START") == true) {
        ui->koboxStatusLabel->setText("KoBox is <b>enabled</b>");
        not_user_change = true;
        ui->checkBox->click();
        ui->runtimeSettingsWidget->setVisible(true);
    }
    else {
        ui->koboxStatusLabel->setText("KoBox is <b>disabled</b>");
        ui->runtimeSettingsWidget->setVisible(false);
    }

    // DPI setting
    QString dpiSetting;
    string_checkconfig(".config/00-kobox/dpiSetting");
    if(checkconfig_str_val == "") {
        if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
            dpiSetting = "125";
        }
        else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
            dpiSetting = "175";
        }
        else if(global::deviceID == "n437\n") {
            dpiSetting = "225";
        }
        else if(global::deviceID == "n873\n") {
            dpiSetting = "250";
        }
        else {
            dpiSetting = "125";
        }
        std::string dpiSetting_str = dpiSetting.toStdString();
        string_writeconfig(".config/00-kobox/dpiSetting", dpiSetting_str);
    }

    string_checkconfig(".config/00-kobox/dpiSetting");
    int dpi_setting = checkconfig_str_val.toInt();
    ui->spinBox->setValue(dpi_setting);
}

koboxSettings::~koboxSettings()
{
    delete ui;
}

void koboxSettings::on_okBtn_clicked()
{
    koboxSettings::close();
}

void koboxSettings::on_checkBox_toggled(bool checked)
{
    if(checked == true) {
        if(not_user_change != true) {
            log("Enabling KoBox subsystem", className);
            string_writeconfig("/external_root/boot/flags/X11_START", "true");
            openSettingsRebootDialog();
        }
        else {
            not_user_change = false;
        }
    }
    else {
        log("Disabling KoBox subsystem", className);
        string_writeconfig("/external_root/boot/flags/X11_START", "false");
        openSettingsRebootDialog();
    }
}

void koboxSettings::openSettingsRebootDialog() {
    log("Showing reboot dialog", className);
    global::settings::settingsRebootDialog = true;
    global::kobox::koboxSettingsRebootDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
}
void koboxSettings::openResetKoboxDialog() {
    log("Showing KoBox reset dialog", className);
    global::kobox::resetKoboxDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void koboxSettings::on_spinBox_valueChanged(int arg1)
{
    QString number = QString::number(arg1);
    string number_str = number.toStdString();
    string_writeconfig(".config/00-kobox/dpiSetting", number_str);
    log("X11 DPI set to " + number, className);
}

void koboxSettings::on_pushButton_clicked()
{
    // Export Extensions disk image over USB with g_mass_storage/g_file_storage
    global::usbms::usbmsDialog = false;
    global::usbms::launchUsbms = true;
    global::usbms::koboxExportExtensions = true;

    log("Exporting KoBox extensions onboard storage via USB", className);
    log("Showing USBMS splash", className);
    usbmsWindow = new usbms_splash();
    usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
    usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    usbmsWindow->show();
}

void koboxSettings::on_resetKoboxBtn_clicked()
{
    openResetKoboxDialog();
}
