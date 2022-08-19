#include <QTimer>
#include <QStringListModel>
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QScrollBar>

#include "wifidialog.h"
#include "ui_wifidialog.h"
#include "functions.h"
#include "mainwindow.h"
#include "network.h"
#include "wifilogger.h"

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

    ui->returnBtn->setProperty("type", "borderless");

    // Scroll bar
    // Needed for the nia.
    ui->scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 50px; }");

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

   // And set wifi checkbox state. also ignore this first call
   global::wifi::WifiState currentWifiState = checkWifiState();
   if(currentWifiState != global::wifi::WifiState::Disabled and currentWifiState != global::wifi::WifiState::Unknown) {
        ui->Wificheckbox->setChecked(true);
        // To be sure nothing breaks
        refreshFromWatcher = true;
        ui->refreshBtn->click();
   } else {
       wifiButtonEnabled = true;
       ui->stopBtn->setStyleSheet("background-color:grey;");
       ui->stopBtn->setEnabled(false);
   }

   QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
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
    log("Clicked refresh button", className);
    if(checkWifiState() == global::wifi::WifiState::Disabled) {
        if(refreshFromWatcher == true) {
            refreshFromWatcher = false;
            emit showToast("To scan, turn on wi-fi first");
            log("To scan, turn on wi-fi first", className);
        }
    }
    else {
        refreshFromWatcher = false;
        launchRefresh();
    }
}

void wifiDialog::launchRefresh() {
    // Order is important
    if(scanInProgress == false) {
        scanInProgress = true;
        ui->refreshBtn->setStyleSheet("background-color:grey;");
        ui->refreshBtn->setEnabled(false);

        elapsedSeconds = 0;
        fullList.remove();
        formattedList.remove();
        string_writeconfig("/opt/ibxd", "list_wifi_networks\n");
        QTimer::singleShot(0, this, SLOT(refreshWait()));
    }
}

void wifiDialog::refreshWait() {
    if(fullList.exists() == false and formattedList.exists() == false) {
        if(elapsedSeconds == 6) {
            emit showToast("Failed to get network list");
            log("Failed to get network list", className);
            ui->refreshBtn->setStyleSheet("background-color:white;");
            ui->refreshBtn->setEnabled(true);
            scanInProgress = false;
        }
        else {
            elapsedSeconds = elapsedSeconds + 1;
            QTimer::singleShot(1000, this, SLOT(refreshWait()));
        }
    } else {
        log("Happily got network list", className);
        refreshNetworksList();
    }
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
            log("Data lines count is below 4, skipping", className);
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
        // Really filter out empty networks
        if(singleNetwork.name.isEmpty() == false) {
            pureNetworkList.append(singleNetwork);
        }
    }
    log("found valid networks: " + QString::number(pureNetworkList.count()), className);
    if(pureNetworkList.count() == 0) {
        log("No networks found, skipping", className);
        showToastSlot("No networks found");
        ui->refreshBtn->setEnabled(true);
        ui->refreshBtn->setStyleSheet("background-color:white;");
        scanInProgress = false;
        return void();
    }
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
            if(currentWifiNetwork.isEmpty() == false and wifiNetwork.name.contains(currentWifiNetwork) == true) {
                log("Found current network in vector", className);
                vectorNetworkLocation = countVec;
                currentNetwork = wifiNetwork.name;
                log("Test", className);
                network* connectedNetwork = new network;
                connectedNetwork->mainData = wifiNetwork;
                log("Test", className);
                // To be really sure that the the info is put there
                connectedNetwork->currentlyConnectedNetwork = currentNetwork;
                log("Test", className);
                connectedNetworkDataParent = wifiNetwork;
                connectedNetworkDataParentSetted = true;
                log("Test", className);

                // This doesnt work so a layout is needed
                // ui->scrollArea->addScrollBarWidget(connectedNetwork, Qt::AlignTop);
                connectedNetwork->applyVariables();
                log("Test", className);
                connect(this, &wifiDialog::killNetworkWidgets, connectedNetwork, &network::closeWrapper);
                connect(connectedNetwork, &network::showToastSignal, this, &wifiDialog::showToastSlot);
                connect(connectedNetwork, &network::refreshScreenSignal, this, &wifiDialog::refreshScreenSlot);
                ui->scrollBarLayout->addWidget(connectedNetwork, Qt::AlignTop);
            }
            else {
                countVec = countVec + 1;
            }
        }
        if(vectorNetworkLocation != 9999) {
            log("pureNetworkList size is: " + QString::number(pureNetworkList.count()) + " And i want to remove at: " + QString::number(vectorNetworkLocation), className);
            pureNetworkList.removeAt(vectorNetworkLocation);
        }
    }
    for(global::wifi::wifiNetworkData wifiNetwork: pureNetworkList) {
        log("signal strength without sorting: " + QString::number(wifiNetwork.signal), className);
    }


    // Sort based on signal strength
    QVector<global::wifi::wifiNetworkData> sortedPureNetworkList;
    sortedPureNetworkList.append(pureNetworkList.first());
    pureNetworkList.removeFirst();
    for(global::wifi::wifiNetworkData wifiNetwork: pureNetworkList) {
        bool stopIterating = false;
        int counter = 0;
        for(global::wifi::wifiNetworkData wifiNetworkToSort: sortedPureNetworkList) {
            if(stopIterating == false) {
                if(wifiNetwork.signal >= wifiNetworkToSort.signal) {
                    sortedPureNetworkList.insert(counter, wifiNetwork);
                    stopIterating = true;
                }
                counter = counter + 1;
            }
        }
        // This happens if its the smallest value, so insert it at the end
        if(stopIterating == false) {
            sortedPureNetworkList.append(wifiNetwork);
        }
    }

    log("There are " + QString::number(sortedPureNetworkList.count()) + " sorted networks", className);

    for(global::wifi::wifiNetworkData wifiNetwork: sortedPureNetworkList) {
        log("signal strength with sorting: " + QString::number(wifiNetwork.signal), className);
    }

    // And now rest of the networks
    for(global::wifi::wifiNetworkData wifiNetwork: sortedPureNetworkList) {
        network* connectedNetwork = new network;
        connectedNetwork->mainData = wifiNetwork;
        connectedNetwork->currentlyConnectedNetwork = currentNetwork;
        connectedNetwork->applyVariables();
        connect(this, &wifiDialog::killNetworkWidgets, connectedNetwork, &network::closeWrapper);
        connect(connectedNetwork, &network::showToastSignal, this, &wifiDialog::showToastSlot);
        connect(connectedNetwork, &network::refreshScreenSignal, this, &wifiDialog::refreshScreenSlot);
        ui->scrollBarLayout->addWidget(connectedNetwork, Qt::AlignTop);
    }
    scannedAtLeastOnce = true;
    ui->refreshBtn->setEnabled(true);
    ui->refreshBtn->setStyleSheet("background-color:white;");
    scanInProgress = false;
}


void wifiDialog::on_Wificheckbox_stateChanged(int arg1)
{
    connectedNetworkDataParentSetted = false;
    log("wifi dialog clicked: " + QString::number(arg1), className);
    if(wifiButtonEnabled == true) {
        if(arg1 == 2) {
            log("turning wifi on", className);
            // the watcher will scan wifi
            forceRefresh = true;
            QTimer::singleShot(0, this, SLOT(turnOnWifi()));
            ui->stopBtn->setStyleSheet("background-color:white;");
            ui->stopBtn->setEnabled(true);
        } else {
            log("turning wifi off", className);
            QTimer::singleShot(0, this, SLOT(turnOffWifi()));
            ui->stopBtn->setStyleSheet("background-color:grey;");
            ui->stopBtn->setEnabled(false);
        }
        emit killNetworkWidgets();
    }

    if(wifiButtonEnabled == false){
        wifiButtonEnabled = true;
    }
}

void wifiDialog::turnOnWifi() {
    string_writeconfig("/opt/ibxd", "toggle_wifi_on\n");
}

void wifiDialog::turnOffWifi() {
    string_writeconfig("/opt/ibxd", "toggle_wifi_off\n");
}

void wifiDialog::on_logBtn_clicked()
{
    // To avoid half informations
    if(scannedAtLeastOnce == false and checkWifiState() == global::wifi::WifiState::Configured) {
        log("Scanning at least once is needed");
        emit showToast("Scan at least once");
    } else {
        wifilogger* wifiLoggerDialog = new wifilogger;
        if(connectedNetworkDataParentSetted == true) {
            wifiLoggerDialog->connectedNetworkData = connectedNetworkDataParent;
            wifiLoggerDialog->isThereData = true;
        }
        wifiLoggerDialog->exec();
    }
}

void wifiDialog::showToastSlot(QString message) {
    emit showToast(message);
}

void wifiDialog::refreshScreenSlot() {
    this->repaint();
}

/*
Some documentation used by the watcher
connection_manager.sh - Manages all things, launches other processes
connect_to_network.sh - All in one connection manager. manages everything, used by ipd, should be used for recconections after sleeping / booting
get_dhcp.sh - Gets dhcp addresses
prepare_changing_wifi.sh - Kills everything, prepares to changing network
smarter_time_sync.sh - Synces time
toggle.sh - Turns on / off
list_networks.bin - Well lists networks
theWatcher() first watches at processes that could kill other ones
*/

void wifiDialog::theWatcher() {

    bool killing = checkProcessName("toggle.sh");
    bool changing = checkProcessName("prepare_changing_wifi.sh");
    if(killing == true) {
        setStatusText("Changing wifi state");
        log("toggle.sh is active", className);
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    if(changing == true) {
        setStatusText("Disconnecting from a network or cleaning");
        log("prepare_changing_wifi.sh is active", className);
        forceRefresh = true;
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    bool recconection = checkProcessName("connect_to_network.sh");
    if(recconection == true) {
        forceRefresh = true;
        QFile recName = QFile(".config/17-wifi_connection_information/essid");
        if(recName.exists() == true) {
             setStatusText("Recconecting after suspending to " + readFile(recName.fileName()).replace("\n", ""));
        }
        else {
            // Shouldn't be possible
            setStatusText("Recconecting after sleep, but no network found?");
        }
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    bool listing = checkProcessName("list_networks.bin");
    if(listing == true) {
        setStatusText("Scanning networks...");
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    bool time = checkProcessName("smarter_time_sync.sh");
    if(time == true) {
        forceRefresh = true;
        setStatusText("Syncing time");
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    bool dhcp = checkProcessName("get_dhcp.sh");
    if(dhcp == true) {
        forceRefresh = true;
        setStatusText("Getting IP address");
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    bool connecting = checkProcessName("connection_manager.sh");
    if(connecting == true) {
        forceRefresh = true;
        setStatusText("Connecting to wifi...");
        QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
        return void();
    }

    if(ui->statusLabel->text() != "Idling") {
        setStatusText("Idling");
    }

    if(forceRefresh == true) {
        forceRefresh = false;
        refreshFromWatcher = true;
        ui->refreshBtn->click();
    }

    if(unlockCheckbox == true) {
        ui->Wificheckbox->setEnabled(true);
        unlockCheckbox = false;
    }

    QTimer::singleShot(relaunchMs, this, SLOT(theWatcher()));
}

void wifiDialog::setStatusText(QString message) {
    ui->statusLabel->setText(message);
}

void wifiDialog::on_stopBtn_clicked()
{
    connectedNetworkDataParentSetted = false;
    ui->Wificheckbox->setEnabled(false);
    unlockCheckbox = true;

    // To inform the wifi icon GUI to don't show the connected / failed to connect message
    string_writeconfig(".config/17-wifi_connection_information/stopped", "true");

    // Maybe limit this, idk
    string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");

    setDefaultWorkDir();
    QFile(".config/17-wifi_connection_information/essid").remove();
    QFile(".config/17-wifi_connection_information/passphrase").remove();

    // This variable just avoids showing the toast so i can use it here too...
    refreshFromWatcher = true;
    ui->refreshBtn->click();
}

void wifiDialog::on_returnBtn_clicked()
{
    this->deleteLater();
    this->close();
}
