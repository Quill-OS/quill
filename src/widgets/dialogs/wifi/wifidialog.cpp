#include <QTimer>
#include <QStringListModel>
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>

#include "wifidialog.h"
#include "ui_wifidialog.h"
#include "functions.h"
#include "mainwindow.h"
#include "network.h"

wifiDialog::wifiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wifiDialog)
{
    ui->setupUi(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    this->setModal(true);

    ui->stopBtn->setIcon(QIcon(":/resources/stop.png"));
    ui->logBtn->setIcon(QIcon(":/resources/file-text.png"));
    ui->refreshBtn->setIcon(QIcon(":/resources/refresh.png"));

    ui->Wificheckbox->setStyleSheet("QCheckBox::indicator { width:50px; height: 50px; }");

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->setFixedWidth(screenGeometry.width());

    int halfOfHalfHeight = ((screenGeometry.height() / 2) / 2) / 2;
    int finalHeight = screenGeometry.height() - halfOfHalfHeight * 2;

    this->setFixedHeight(finalHeight);
    this->move(0, halfOfHalfHeight);

    // Button sizes
    ui->stopBtn->setFixedWidth(screenGeometry.width() / 8);
    ui->logBtn->setFixedWidth(screenGeometry.width() / 8);
    ui->refreshBtn->setFixedWidth(screenGeometry.width() / 8);

    int heighIncrease = 20;
    ui->stopBtn->setFixedHeight(ui->stopBtn->height() + heighIncrease);
    ui->logBtn->setFixedHeight(ui->logBtn->height() + heighIncrease);
    ui->refreshBtn->setFixedHeight(ui->refreshBtn->height() + heighIncrease);

   //ui->cancelBtn->setProperty("type", "borderless");

}

wifiDialog::~wifiDialog()
{
    delete ui;
}

/*

        this->hide();
        global::keyboard::keyboardDialog = true;
        global::keyboard::wifiPassphraseDialog = true;
        global::keyboard::keyboardText = "";
        generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        generalDialogWindow->wifiEssid = itemText;
        connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreenNative()));
        connect(generalDialogWindow, SIGNAL(updateWifiIcon(int)), SLOT(updateWifiIcon(int)));
        connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
        connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToastNative()));
        connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(close()));
        generalDialogWindow->show();

*/

/*
    bool connectToNetwork(QString essid, QString passphrase) {
        log("Connecting to network '" + essid + "'", "functions");
        std::string essid_str = essid.toStdString();
        std::string passphrase_str = passphrase.toStdString();
        string_writeconfig("/run/wifi_network_essid", essid_str);
        string_writeconfig("/run/wifi_network_passphrase", passphrase_str);
        string_writeconfig("/opt/ibxd", "connect_to_wifi_network\n");

        int connectionSuccessful = 0;

        while(connectionSuccessful == 0) {
            if(QFile::exists("/run/wifi_connected_successfully")) {
                if(checkconfig("/run/wifi_connected_successfully") == true) {
                    QFile::remove("/run/wifi_connected_successfully");
                    connectionSuccessful = 1;
                    global::network::isConnected = true;
                    setDefaultWorkDir();
                    string_writeconfig(".config/17-wifi_connection_information/essid", essid_str);
                    string_writeconfig(".config/17-wifi_connection_information/passphrase", passphrase_str);
                    QString function = __func__; log(function + ": Connection successful", "functions");
                    return true;
                }
                else {
                    QFile::remove("/run/wifi_connected_successfully");
                    connectionSuccessful = 0;
                    global::network::isConnected = false;
                    QString function = __func__; log(function + ": Connection failed", "functions");
                    return false;
                }
            }
            else {
                QThread::msleep(100);
            }
        }
    }
*/
/*
    int testPing(bool blocking) {
        QProcess *pingProcess = new QProcess();
        if(blocking == true) {
            QString pingProg = "ping";
            QStringList pingArgs;
            pingArgs << "-c" << "1" << "1.1.1.1";
            pingProcess->start(pingProg, pingArgs);
            pingProcess->waitForFinished();
            int exitCode = pingProcess->exitCode();
            pingProcess->deleteLater();
            if(exitCode == 0) {
                global::network::isConnected = true;
            }
            else {
                global::network::isConnected = false;
            }
            return exitCode;
        }
        // For some reason, implementing a non-blocking version of this functions triggers a "terminate called without an active exception" error with a platform plugin compiled with a newer GCC 11 toolchain. The problem has been solved by transplanting this function into the related area which uses it.
        pingProcess->deleteLater();
    }
    bool getTestPingResults() {
        // To be used when the testPing() function is used in non-blocking mode.
        if(QFile::exists("/run/test_ping_status")) {
            if(checkconfig("/run/test_ping_status") == true) {
                global::network::isConnected = true;
                return true;
            }
            else {
                global::network::isConnected = false;
                return false;
            }
        }
        else {
            global::network::isConnected = false;
            return false;
        }
    }
*/

void wifiDialog::on_refreshBtn_clicked()
{
    if(checkWifiState() == global::wifi::WifiState::Disabled) {
        emit showToast("To scan, turn on wi-fi first");
        log("To scan, turn on wi-fi first", className);
    }
    else {
        if(launchRefresh() == true) {
            refreshNetworksList();
        }
    }
}

bool wifiDialog::launchRefresh() {
    QFile fullList =  QFile("/external_root/run/wifi_list_full");
    QFile formattedList = QFile("/external_root/run/wifi_list_format");
    fullList.remove();
    formattedList.remove();
    string_writeconfig("/opt/ibxd", "list_wifi_networks\n");
    QElapsedTimer elapsedTime;
    elapsedTime.start();
    bool continueLoop = true;
    while(fullList.exists() == false and formattedList.exists() == false and continueLoop == true) {
        sleep(1);
        if(elapsedTime.elapsed() > 6000) {
            log("Searching for networks took too long");
            continueLoop = false;
        }
    }
    if(fullList.exists() == false or formattedList.exists() == false) {
        emit showToast("Failed to get network list");
        log("Failed to get network list", className);
        return false;
    }
    log("Happily got network list", className);
    return true;
}

void wifiDialog::refreshNetworksList() {
    emit killNetworkWidgets();
    QStringList networkList = readFile("/external_root/run/wifi_list_format").split("%%==SPLIT==%%\n");
    QVector<global::wifi::wifiNetworkData> pureNetworkList;
    for(QString network: networkList) {
        QStringList data = network.split("\n");
        int count = 1;
        global::wifi::wifiNetworkData singleNetwork;
        if(data.count() < 4) {
            log("Data lines count is below 4, skipping");
            continue;
        }
        for(QString singleData: data) {
            if(count == 1) {
                singleNetwork.mac = singleData;
                log("Mac is: " + singleData, className);
            }
            if(count == 2) {
                log("wifi name is: " + singleData, className);
                if(singleData.isEmpty() == true) {
                    log("Wifi name is empty", className);
                }
                singleNetwork.name = singleData;
            }
            if(count == 3) {
                log("encryption is: " + singleData, className);
                singleNetwork.encryption = QVariant(singleData).toBool();
            }
            if(count == 4) {
                log("signal strength is: " + singleData, className);
                singleNetwork.signal = QVariant(singleData).toInt();
            }
            if(count >= 5) {
                log("Skipping additionall items in wifi", className);
            }
            count = count + 1;
        }
        pureNetworkList.append(singleNetwork);
    }
    log("found valid networks: " + QString::number(pureNetworkList.count()));
    QFile currentWifiNameFile = QFile("/external_root/run/current_wifi_name");
    currentWifiNameFile.remove();
    string_writeconfig("/opt/ibxd", "get_current_wifi_name\n");
    usleep(300000); // 0.3s
    // Here its looking for the now connected network to put it on top
    QString currentNetwork = "";
    if(currentWifiNameFile.exists() == true) {
        QString currentWifiNetwork = readFile(currentWifiNameFile.fileName());
        currentWifiNetwork = currentWifiNetwork.replace("\n", "");
        log("current network name is: " + currentWifiNetwork, className);
        int countVec = 0;
        int vectorNetworkLocation = 9999;
        for(global::wifi::wifiNetworkData wifiNetwork: pureNetworkList) {
            if(wifiNetwork.name.contains(currentWifiNetwork) == true) {
                log("Found current network in vector", className);
                vectorNetworkLocation = countVec;
                currentNetwork = wifiNetwork.name;
                network* connectedNetwork = new network;
                connectedNetwork->mainData = wifiNetwork;
                connectedNetwork->currentlyConnectedNetwork = currentNetwork;
                // this doesnt work so a layout is needed
                // ui->scrollArea->addScrollBarWidget(connectedNetwork, Qt::AlignTop);
                ui->scrollBarLayout->addWidget(connectedNetwork, Qt::AlignTop);
            }
            countVec = countVec + 1;
        }
    }

}

