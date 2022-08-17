#include <QFile>
#include <QScreen>

#include "connectiondialog.h"
#include "ui_connectiondialog.h"

connectiondialog::connectiondialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectiondialog)
{
    ui->setupUi(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->CancelBtn->setStyleSheet("font-size: 9pt");
    ui->connectBtn->setStyleSheet("font-size: 9pt");
    ui->showPasswordBtn->setStyleSheet("font-size: 9pt");

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->setFixedWidth(screenGeometry.width() - 50);


}

connectiondialog::~connectiondialog()
{
    delete ui;
}

void connectiondialog::applyVariables() {
    ui->nameLabel->setText(connectedNetworkData.name);
    ui->macLabel->setText(connectedNetworkData.mac);
    ui->signalLabel->setText(QString::number(connectedNetworkData.signal) + "%");

    if(connectedNetworkData.encryption == false) {
        ui->showPasswordBtn->hide();
        ui->passwordTextEdit->setText("No password required");
    }
}
