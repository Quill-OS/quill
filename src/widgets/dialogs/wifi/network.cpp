#include "network.h"
#include "ui_network.h"

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


}

network::~network()
{
    delete ui;
}

void network::applyVariables() {
    ui->signalStrengthLabel->setText(QString::number(mainData.signal) + "%");

    // limit name size, maybe device specific
    QString cuttedSingleData = mainData.name;
    if(cuttedSingleData.count() > 27)
    {
        cuttedSingleData = cuttedSingleData.remove(24, cuttedSingleData.count() - 24);
        cuttedSingleData.append("...");
    }
    ui->nameLabel->setText(cuttedSingleData);

    if(mainData.encryption == true) {
        ui->encryptionIcon->setIcon(QIcon("://resources/lock-fill.png"));
    }
    else {
        ui->encryptionIcon->setIcon(QIcon("://resources/lock-unlock-fill.png"));
    }

    if(currentlyConnectedNetwork == mainData.name) {
        ui->frame->setStyleSheet(".QFrame{background-color: grey; border: 3px solid black; border-radius: 10px;}");
    } else {
        ui->frame->setStyleSheet(".QFrame{background-color: white; border: 3px solid black; border-radius: 10px;}");
    }
}
