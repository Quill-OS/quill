#include "network.h"
#include "ui_network.h"
#include "connectiondialog.h"

#include <QScreen>

network::network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::network)
{
    ui->setupUi(this);
    this->setFont(QFont("u001"));

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Buttons
    ui->encryptionIcon->setProperty("type", "borderless");
    ui->encryptionIcon->setStyleSheet("QPushButton[type='borderless']:pressed { background: white; color: white; border: none; }");
    ui->signalStrengthIcon->setProperty("type", "borderless");
    ui->signalStrengthIcon->setStyleSheet("QPushButton[type='borderless']:pressed { background: white; color: white; border: none; }");
    ui->enterButton->setProperty("type", "borderless");
}

network::~network()
{
    delete ui;
}

void network::applyVariables() {
    log("Applying variables for network", className);

    QString percent = "%";
    if(mainData.signal < 100) {
        percent.append(" ");
    }
    ui->signalStrengthLabel->setText(QString::number(mainData.signal) + percent);

    // Limit name size
    int truncateThreshold;
    if(global::deviceID == "n705\n") {
        truncateThreshold = 12;
    }
    else {
        truncateThreshold = 20;
    }
    int nameLength = mainData.name.length();
    QString name = mainData.name;
    if(nameLength > truncateThreshold) {
        name.chop(nameLength - truncateThreshold);
        name.append("...");
    }
    ui->nameLabel->setText(name);

    if(mainData.encryption == true) {
        ui->encryptionIcon->setIcon(QIcon(":/resources/lock.png"));
    }
    else {
        ui->encryptionIcon->setIcon(QIcon(":/resources/public.png"));
    }

    if(currentlyConnectedNetwork == mainData.name) {
        ui->frame->setStyleSheet(".QFrame{background-color: lightGray; border: 2px solid black; border-radius: 10px;}");
        ui->signalStrengthLabel->setStyleSheet("background-color: lightGray;");
        ui->nameLabel->setStyleSheet("background-color: lightGray;");
        ui->encryptionIcon->setStyleSheet("background-color: lightGray;");
        ui->enterButton->setStyleSheet("background-color: lightGray;");

        ui->signalStrengthIcon->setStyleSheet("QPushButton {background-color: lightGray; border:  none}; QPushButton[type='borderless']:pressed { background: lightGray; color: lightGray; border: none; }");
        ui->encryptionIcon->setStyleSheet("QPushButton {background-color: lightGray; border:  none}; QPushButton[type='borderless']:pressed { background: lightGray; color: lightGray; border: none; }");
        ui->enterButton->setStyleSheet("QPushButton {background-color: lightGray; border:  none}; QPushButton[type='borderless']:pressed { background: lightGray; color: lightGray; border: none; }");
    }
    else {
        ui->frame->setStyleSheet(".QFrame{background-color: white; border: 2px solid black; border-radius: 10px;}");
    }

    if(mainData.signal >= 0 and mainData.signal <= 25) {
        ui->signalStrengthIcon->setIcon(QIcon(":/resources/wifi-0.png"));
    }
    else if(mainData.signal >= 25 and mainData.signal < 50) {
        ui->signalStrengthIcon->setIcon(QIcon(":/resources/wifi-25.png"));
    }
    else if(mainData.signal >= 50 and mainData.signal < 75) {
        ui->signalStrengthIcon->setIcon(QIcon(":/resources/wifi-50.png"));
    }
    else if(mainData.signal >= 75 and mainData.signal < 100) {
        ui->signalStrengthIcon->setIcon(QIcon(":/resources/wifi-75.png"));
    }
    else if(mainData.signal == 100) {
        ui->signalStrengthIcon->setIcon(QIcon(":/resources/wifi-100.png"));
    }
}

void network::on_enterButton_clicked()
{
    connectiondialog* newConnectionDialog = new connectiondialog;
    newConnectionDialog->connectedNetworkData = mainData;
    newConnectionDialog->currentlyConnectedNetworkName = currentlyConnectedNetwork;
    newConnectionDialog->applyVariables();
    connect(newConnectionDialog, &connectiondialog::showToastSignal, this, &network::showToastSlot);
    connect(newConnectionDialog, &connectiondialog::refreshScreenSignal, this, &network::refreshScreenSlot);
    newConnectionDialog->exec();
}

void network::closeWrapper() {
    this->deleteLater();
    this->close();
}

void network::showToastSlot(QString message) {
    emit showToastSignal(message);
}

void network::refreshScreenSlot() {
    emit refreshScreenSignal();
}
