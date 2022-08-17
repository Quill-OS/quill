#include <QFile>

#include "wifilogger.h"
#include "ui_wifilogger.h"
#include "functions.h"

wifilogger::wifilogger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wifilogger)
{
    ui->setupUi(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    this->setModal(true);


    setWifiInfoPage();
    QObject::connect(&updateLogsTimer, &QTimer::timeout, this, &wifilogger::updateLogs);
    updateLogsTimer.start(1000);
}

wifilogger::~wifilogger()
{
    delete ui;
}

void wifilogger::setWifiInfoPage() {
    if(checkWifiState() == global::wifi::WifiState::Configured) {
        QTimer::singleShot(0, this, SLOT(getWifiInformations()));
        ui->stackedWidget->setCurrentIndex(0);
        ui->nameLabel->setText("Network informations");
    }
    else {
        ui->stackedWidget->setCurrentIndex(1);
        ui->nameLabel->setText("No network connected");
    }
}

void wifilogger::setFancyLoggingPage() {
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameLabel->setText("Important logs");
}

void wifilogger::setAllLogsPage() {
    ui->stackedWidget->setCurrentIndex(3);
    ui->nameLabel->setText("All logs");
}

void wifilogger::on_nextBtn_clicked()
{
    if(currentPage == 2) {
        currentPage = 0;
    }
    else {
        currentPage = currentPage + 1;
    }
    changePage();
}

void wifilogger::on_previousBtn_clicked()
{
    if(currentPage == 0) {
        currentPage = 2;
    }
    else {
        currentPage = currentPage - 1;
    }
    changePage();
}

void wifilogger::changePage() {
    if(currentPage == 0) {
        setWifiInfoPage();
    }
    else if(currentPage == 1) {
        setFancyLoggingPage();
    }
    else if(currentPage == 2) {
        setAllLogsPage();
    } else {
        log("This shouldnt happen.", className);
    }
}

void wifilogger::getWifiInformations() {
    QFile wifiInformationsPath = QFile("/external_root/run/wifi_informations");
    wifiInformationsPath.remove();

    string_writeconfig("/opt/ibxd", "get_wifi_informations\n");
    while(wifiInformationsPath.exists() == false) {
        sleep(1);
    }

    QString wifiInfo = readFile(wifiInformationsPath.fileName());
    QStringList wifiInfoList = wifiInfo.split("\n");
    int counter = 0;
    for(QString infomation: wifiInfoList) {
        if(counter == 0) {
            ui->netNameLabel->setText(infomation);
        }
        if(counter == 1) {
            ui->ipLabel->setText(infomation);
        }
        if(counter == 2) {
            ui->maskLabel->setText(infomation);
        }
        if(counter == 3) {
            ui->gatewayLabel->setText(infomation);
        }
        counter = counter + 1;
    }

    ui->encryptionLabel->setText(QVariant(connectedNetworkData.encryption).toString());

    ui->signalLabel->setText(QString::number(connectedNetworkData.signal) + "%");

    ui->macLabel->setText(connectedNetworkData.mac);
}


void wifilogger::on_returnBtn_clicked()
{
    updateLogsTimer.stop();
    this->deleteLater();
}

void wifilogger::updateLogs() {
    if(fancyLogs.exists() == true and allLogs.exists() == true) {
        QString fancyLogsText = readFile(fancyLogs.fileName());
        QString allLogsText = readFile(allLogs.fileName());

        ui->fancyLogsText->setText(fancyLogsText);
        ui->allLogsText->setText(allLogsText);
    }
}
