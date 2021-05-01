#include "koboxsettings.h"
#include "ui_koboxsettings.h"
#include "functions.h"

#include <QFile>

koboxSettings::koboxSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::koboxSettings)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // UI tweaks
    ui->okBtn->setProperty("type", "borderless");

    if(checkconfig("/external_root/boot/flags/X11_START") == true) {
        ui->koboxStatusLabel->setText("KoBox is <b>enabled</b>");
        not_user_change = true;
        ui->checkBox->click();
    }
    else {
        ui->koboxStatusLabel->setText("KoBox is <b>disabled</b>");
    }
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
