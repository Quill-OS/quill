#include "toast.h"
#include "ui_toast.h"
#include "functions.h"

#include <QScreen>
#include <QTimer>

toast::toast(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toast)
{
    ui->setupUi(this);
    ui->messageLabel->setStyleSheet("padding: 35px");
    ui->messageLabel->setText(global::toast::message);
    this->adjustSize();
    centerToast();
    if(global::toast::wifiToast == true) {
        global::toast::wifiToast = false;
        wifiDialogWindow = new wifiDialog(this);
        wifiDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(wifiDialogWindow, SIGNAL(wifiNetworksListReady(int)), SLOT(showWifiDialog(int)));
        connect(wifiDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(exitSlot()));
    }
    else {
        QTimer::singleShot(5000, this, SLOT(exitSlot()));
    }
}

toast::~toast()
{
    delete ui;
}

void toast::showWifiDialog(int networksFound) {
    if(networksFound == 1) {
        emit updateWifiIconSig(2);
        this->hide();
        wifiDialogWindow->show();
        wifiDialogWindow->adjustSize();
        wifiDialogWindow->centerDialog();
    }
    else {
        ui->messageLabel->setText("No networks found");
        QThread::sleep(5);
        toast::close();
    }
}

void toast::centerToast() {
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

void toast::exitSlot() {
    toast::close();
}
