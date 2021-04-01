#include "generaldialog.h"
#include "ui_generaldialog.h"

#include <QFile>
#include <QDebug>
#include <QProcess>

generalDialog::generalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::generalDialog)
{
    ui->setupUi(this);

    // Preventing outside interaction
    this->setModal(true);

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->okBtn->setProperty("type", "borderless");
    ui->cancelBtn->setProperty("type", "borderless");

    ui->okBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->cancelBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->bodyLabel->setStyleSheet("font-size: 9pt");

    if(checkconfig("/inkbox/resetDialog") == true) {
        resetDialog = true;
        ui->okBtn->setText("Proceed");
        ui->cancelBtn->setText("Go back");
        ui->bodyLabel->setText("This will erase any books you have on the device. Settings will be reset.");
        ui->headerLabel->setText("Warning");
        this->adjustSize();
        string_writeconfig("/inkbox/resetDialog", "false");
    }
    if(checkconfig("/inkbox/updateDialog") == true) {
        updateDialog = true;
        ui->okBtn->setText("Update");
        ui->cancelBtn->setText("Not now");
        ui->bodyLabel->setText("Do you want to update InkBox now?");
        ui->headerLabel->setText("Update available");
        this->adjustSize();
        string_writeconfig("/inkbox/updateDialog", "false");
    }
    else {
        // We shouldn't be there ;)
        ;
    }
}

generalDialog::~generalDialog()
{
    delete ui;
}

void generalDialog::on_cancelBtn_clicked()
{
    if(resetDialog == true) {
        generalDialog::close();
    }
    if(updateDialog == true) {
        string_writeconfig("/tmp/cancelUpdateDialog", "true");
        generalDialog::close();
    }
}

void generalDialog::on_okBtn_clicked()
{
    if(resetDialog == true) {
        // Reset the device ... some code to come
        ;
    }
    if(updateDialog == true) {
        string_writeconfig("/mnt/onboard/onboard/.inkbox/can_really_update", "true");
        string_writeconfig("/external_root/opt/update/will_update", "true");
        string_writeconfig("/external_root/boot/flags/WILL_UPDATE", "true");
        QString prog ("reboot");
        QStringList args;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();
    }
}
