#include <QTimer>
#include <QStringListModel>
#include <QScreen>
#include <QDesktopWidget>
#include <QMessageBox>

#include "wifidialog.h"
#include "ui_wifidialog.h"
#include "functions.h"
#include "mainwindow.h"

wifiDialog::wifiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wifiDialog)
{
    ui->setupUi(this);
    wifiListTimer = new QTimer(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    this->setModal(true);

    ui->cancelBtn->setProperty("type", "borderless");
    ui->connectBtn->setProperty("type", "borderless");
    ui->cancelBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->connectBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->mainLabel->setStyleSheet("padding-left: 125px; padding-right: 125px");
    ui->networksListWidget->setFont(QFont("u001"));
    ui->networksListWidget->setStyleSheet("font-size: 9pt");

    checkWifiNetworks();
}

wifiDialog::~wifiDialog()
{
    delete ui;
}

void wifiDialog::checkWifiNetworks() {
    string_writeconfig("/opt/ibxd", "list_wifi_networks\n");
    wifiListTimer->setInterval(100);
    connect(wifiListTimer, &QTimer::timeout, [&]() {
        if(QFile::exists("/run/wifi_networks_list")) {
           printWifiNetworks();
           wifiListTimer->stop();
        }
    } );
    wifiListTimer->start();
}

void wifiDialog::printWifiNetworks() {
    string_checkconfig_ro("/run/wifi_networks_list");
    if(checkconfig_str_val == "") {
        emit quit(1);
        wifiDialog::close();
    }
    else {
        QFile wifiNetworksListFile("/run/wifi_networks_list");
        wifiNetworksListFile.open(QIODevice::ReadWrite);
        QTextStream in (&wifiNetworksListFile);
        wifiNetworksList = in.readAll();
        wifiNetworksListFile.close();
        QFile::remove("/run/wifi_networks_list");

        QStringListModel* model = new QStringListModel(this);
        QStringList list = wifiNetworksList.split("\n", QString::SkipEmptyParts);
        model->setStringList(list);
        ui->networksListWidget->setModel(model);
        ui->networksListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        emit wifiNetworksListReady(1);
    }
}

void wifiDialog::centerDialog() {
    // Centering dialog
    // Get current screen size
    QRect rec = QGuiApplication::screenAt(this->pos())->geometry();
    // Using minimum size of window
    QSize size = this->minimumSize();
    // Set top left point
    QPoint topLeft = QPoint((rec.width() / 2) - (size.width() / 2), (rec.height() / 2) - (size.height() / 2));
    // set window position
    setGeometry(QRect(topLeft, size));
}

void wifiDialog::on_cancelBtn_clicked()
{
    string_writeconfig("/opt/ibxd", "toggle_wifi_off\n");
    while(true) {
        if(QFile::exists("/run/toggle_wifi_off_done")) {
            QFile::remove("/run/toggle_wifi_off_done");
            break;
        }
    }
    emit quit(0);
    wifiDialog::close();
}

void wifiDialog::on_connectBtn_clicked()
{
    index = ui->networksListWidget->currentIndex();
    itemText = index.data(Qt::DisplayRole).toString();
    if(itemText == "") {
        QMessageBox::critical(this, tr("Invalid argument"), tr("You must select a network."));
    }
    else {
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
    }
}

void wifiDialog::refreshScreenNative() {
    emit refreshScreen();
}

void wifiDialog::updateWifiIcon(int mode) {
    emit updateWifiIconSig(mode);
}

void wifiDialog::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

void wifiDialog::closeIndefiniteToastNative() {
    emit closeIndefiniteToast();
}
