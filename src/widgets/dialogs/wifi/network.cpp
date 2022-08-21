#include "network.h"
#include "ui_network.h"
#include "connectiondialog.h"

network::network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::network)
{
    ui->setupUi(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Buttons
    ui->encryptionIcon->setProperty("type", "borderless");
    ui->encryptionIcon->setStyleSheet("QPushButton[type='borderless']:pressed { background: white; color: white; border: none; }");

    ui->enterButton->setProperty("type", "borderless");

    ui->enterButton->setFixedWidth(40);
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

    // Limit name size, maybe device specific
    QString cuttedSingleData = mainData.name;
    if(cuttedSingleData.count() > 27)
    {
        cuttedSingleData = cuttedSingleData.remove(24, cuttedSingleData.count() - 24);
        cuttedSingleData.append("...");
    }
    ui->nameLabel->setText(cuttedSingleData);

    if(mainData.encryption == true) {
        ui->encryptionIcon->setIcon(QIcon("://resources/lock.png"));
    }
    else {
        ui->encryptionIcon->setIcon(QIcon("://resources/public.png"));
    }

    if(currentlyConnectedNetwork == mainData.name) {
        ui->frame->setStyleSheet(".QFrame{background-color: grey; border: 3px solid black; border-radius: 10px;}");
        ui->signalStrengthLabel->setStyleSheet("background-color:grey;");
        ui->nameLabel->setStyleSheet("background-color:grey;");
        ui->encryptionIcon->setStyleSheet("background-color:grey;");
        ui->enterButton->setStyleSheet("background-color:grey;");

        // Some stylesheet magician could make it work that it cant be clicked
        ui->encryptionIcon->setStyleSheet("QPushButton {background-color: grey; border:  none}; QPushButton[type='borderless']:pressed { background: grey; color: grey; border: none; }");

        ui->enterButton->setStyleSheet("QPushButton {background-color: grey; border:  none}; QPushButton[type='borderless']:pressed { background: grey; color: grey; border: none; }");

    } else {
        ui->frame->setStyleSheet(".QFrame{background-color: white; border: 3px solid black; border-radius: 10px;}");
    }
}

void network::on_enterButton_clicked()
{
    connectiondialog* newConnectionDiallog = new connectiondialog;
    newConnectionDiallog->connectedNetworkData = mainData;
    newConnectionDiallog->currentlyConnectedNetworkName = currentlyConnectedNetwork;
    newConnectionDiallog->applyVariables();
    connect(newConnectionDiallog, &connectiondialog::showToastSignal, this, &network::showToastSlot);
    connect(newConnectionDiallog, &connectiondialog::refreshScreenSignal, this, &network::refreshScreenSlot);
    newConnectionDiallog->exec();
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
