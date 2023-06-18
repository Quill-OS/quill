#include <QFile>
#include <QScrollBar>
#include <QScreen>

#include "wifilogger.h"
#include "ui_wifilogger.h"
#include "functions.h"

wifilogger::wifilogger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wifilogger)
{
    ui->setupUi(this);
    this->setFont(QFont("u001"));
    // 'Not currently connected to a network' label
    ui->label_8->setFont(QFont("Inter"));
    ui->returnBtn->setFont(QFont("Inter"));
    ui->nameLabel->setFont(QFont("Inter"));
    ui->allLogsText->setFont(QFont("Roboto Mono"));
    ui->fancyLogsText->setFont(QFont("Roboto Mono"));
    ui->nameLabel->setStyleSheet("font-weight: bold");
    ui->returnBtn->setStyleSheet("font-weight: bold");
    ui->allLogsText->setStyleSheet("font-size: 7.5pt");
    ui->fancyLogsText->setStyleSheet("font-size: 7.5pt");

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    this->setModal(true);

    // Scroll bar
    // Needed for the nia.
    // ui->allLogsText->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 50px; }");
    // ui->fancyLogsText->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 50px; }");

    log("Entered wifilogger", className);
    setWifiInfoPage();
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->returnBtn->setProperty("type", "borderless");
    ui->refreshBtn->setProperty("type", "borderless");

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->setFixedWidth(screenGeometry.width() / 1.2);

    int halfOfHalfHeight = ((screenGeometry.height() / 2) / 2) / 2;
    int finalHeight = screenGeometry.height() - halfOfHalfHeight * 2.4;

    this->setFixedHeight(finalHeight);

    // Centering dialog
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

wifilogger::~wifilogger()
{
    delete ui;
}

void wifilogger::setWifiInfoPage() {
    if(checkWifiState() == global::wifi::wifiState::configured) {
        getWifiInformationTimer.singleShot(0, this, SLOT(getWifiInformation()));
        ui->stackedWidget->setCurrentIndex(0);
        ui->nameLabel->setText("Network information");
    }
    else {
        ui->stackedWidget->setCurrentIndex(1);
        ui->nameLabel->setText("Not connected to a network");
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
    }
    else {
        log("This shouldn't happen", className);
    }
}

void wifilogger::getWifiInformation() {
    log("Retrieving Wi-Fi information", className);
    QFile wifiInformationPath = QFile("/external_root/run/wifi_information");
    if(waitingForFile == false) {
        wifiInformationPath.remove();
        log("Sending get_wifi_information ibxd call", className);
        string_writeconfig("/opt/ibxd", "get_wifi_information\n");
        waitingForFile = true;
    }

    if(waitingForFile == true) {
        if(wifiInformationPath.exists() == false) {
            getWifiInformationTimer.singleShot(1000, this, SLOT(getWifiInformation()));
            return void();
        }
    }

    waitingForFile = false;
    log("Setting variables", className);
    QString wifiInfo = readFile(wifiInformationPath.fileName());
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
        // Shouldn't happen for 99%, but if anyway... it's designed to be non-blocking, so I can't really wait for this.
        ui->encryptionLabel->setText("Rescan needed");
        ui->signalLabel->setText("Rescan needed");
        ui->macLabel->setText("Rescan needed");
    }
}

void wifilogger::on_returnBtn_clicked()
{
    log("Exiting wifilogger", className);
    getWifiInformationTimer.stop();
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
