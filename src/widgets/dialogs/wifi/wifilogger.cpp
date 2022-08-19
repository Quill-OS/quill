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


    log("Entered wifilogger", className);
    setWifiInfoPage();
    ui->refreshBtn->setProperty("type", "borderless");

    ui->nextBtn->setFixedWidth(70);
    ui->previousBtn->setFixedWidth(70);
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
    updateLogs();
    ui->stackedWidget->setCurrentIndex(2);
    ui->nameLabel->setText("Important logs");
}

void wifilogger::setAllLogsPage() {
    updateLogs();
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
    log("getting wifi informations", className);
    QFile wifiInformationsPath = QFile("/external_root/run/wifi_informations");
    if(waitingForFile == false) {
        wifiInformationsPath.remove();

        log("Launching get_wifi_informations ibxd call", className);
        string_writeconfig("/opt/ibxd", "get_wifi_informations\n");
        waitingForFile = true;
    }

    if(waitingForFile == true) {
        if(wifiInformationsPath.exists() == false) {
            QTimer::singleShot(1000, this, SLOT(getWifiInformations()));
            return void();
        }
    }

    waitingForFile = false;
    log("Setting variables", className);
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

    if(isThereData == true) {
        ui->encryptionLabel->setText(QVariant(connectedNetworkData.encryption).toString());

        ui->signalLabel->setText(QString::number(connectedNetworkData.signal) + "%");

        ui->macLabel->setText(connectedNetworkData.mac);
    }
    else {
        // Shouldn't happen for 99%, but if anyway... its designed to be non blocking, so i cant really wait for this.
        ui->encryptionLabel->setText("Rescan needed");

        ui->signalLabel->setText("Rescan needed");

        ui->macLabel->setText("Rescan needed");
    }
}


void wifilogger::on_returnBtn_clicked()
{
    log("Exiting wifilogger", className);
    this->deleteLater();
    this->close();
}

void wifilogger::updateLogs() {
    if(fancyLogs.exists() == true and allLogs.exists() == true) {
        QString fancyLogsText = readFile(fancyLogs.fileName());
        QString allLogsText = readFile(allLogs.fileName());

        ui->fancyLogsText->setText(fancyLogsText);
        ui->allLogsText->setText(allLogsText);
    }
}

void wifilogger::on_refreshBtn_clicked()
{
    if(currentPage == 0) {
        if(waitingForFile == false) {
            setWifiInfoPage();
        }
    }
    else {
        updateLogs();
    }
}
