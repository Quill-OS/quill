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

    // UI tweaks
    ui->okBtn->setProperty("type", "borderless");
    ui->pushButton->setProperty("type", "borderless");
    ui->resetKoboxBtn->setProperty("type", "borderless");

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
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
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
            dpiSetting = "125";
        }
        else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n") {
            dpiSetting = "175";
        }
        else if(checkconfig_str_val == "n873\n") {
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
            string_writeconfig("/external_root/boot/flags/X11_START", "true");
            openSettingsRebootDialog();
        }
        else {
            not_user_change = false;
        }
    }
    else {
        string_writeconfig("/external_root/boot/flags/X11_START", "false");
        openSettingsRebootDialog();
    }
}

void koboxSettings::openSettingsRebootDialog() {
    global::settings::settingsRebootDialog = true;
    global::kobox::koboxSettingsRebootDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}
void koboxSettings::openResetKoboxDialog() {
    global::kobox::resetKoboxDialog = true;
    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}

void koboxSettings::on_spinBox_valueChanged(int arg1)
{
    QString number = QString::number(arg1);
    string number_str = number.toStdString();
    string_writeconfig(".config/00-kobox/dpiSetting", number_str);
}

void koboxSettings::on_pushButton_clicked()
{
    // Export Extensions disk image over USB with g_mass_storage
    global::usbms::usbmsDialog = false;
    global::usbms::launchUsbms = true;
    global::usbms::koboxExportExtensions = true;

    usbmsWindow = new usbms_splash();
    usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
    usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    usbmsWindow->show();
}

void koboxSettings::on_resetKoboxBtn_clicked()
{
    openResetKoboxDialog();
}
