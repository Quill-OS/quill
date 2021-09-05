#include "toast.h"
#include "ui_toast.h"
#include "functions.h"

#include <QScreen>
#include <QTimer>
#include <QDebug>

toast::toast(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toast)
{
    ui->setupUi(this);

    if(global::toast::modalToast == true) {
        global::toast::modalToast = false;
        this->setModal(true);
    }

    ui->messageLabel->setStyleSheet("padding: 35px");
    ui->messageLabel->setText(global::toast::message);
    this->adjustSize();
    centerToast();
    qDebug() << global::toast::delay;
    if(global::toast::indefiniteToast == false) {
        if(global::toast::delay == 0) {
            global::toast::delay = 5000;
        }
    }
    if(global::toast::wifiToast == true) {
        global::toast::wifiToast = false;
        this->setModal(true);
        wifiDialogWindow = new wifiDialog(this);
        wifiDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(wifiDialogWindow, SIGNAL(wifiNetworksListReady(int)), SLOT(showWifiDialog(int)));
        connect(wifiDialogWindow, SIGNAL(quit(int)), SLOT(exitSlot(int)));
        connect(wifiDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreenNative()));
        connect(wifiDialogWindow, SIGNAL(updateWifiIconSig(int)), SLOT(updateWifiIcon(int)));
        connect(wifiDialogWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
        connect(wifiDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToastNative()));
        connect(wifiDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(close()));
    }
    else {
        if(global::toast::indefiniteToast == false) {
            QTimer::singleShot(global::toast::delay, this, SLOT(close()));
            global::toast::delay = 0;
        }
        else {
            global::toast::indefiniteToast = false;
        }
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
}

void toast::centerToast() {
    // Centering toast (https://stackoverflow.com/a/58682351)
    // Get current screen size
    QRect rec = QGuiApplication::screenAt(this->pos())->geometry();
    // Using minimum size of window
    QSize size = this->minimumSize();
    // Set top left point
    QPoint topLeft = QPoint((rec.width() / 2) - (size.width() / 2), (rec.height() / 2) - (size.height() / 2));
    // Set window position
    setGeometry(QRect(topLeft, size));
}

void toast::exitSlot(int exitCode) {
    if(exitCode == 0) {
        toast::close();
    }
    else {
        ui->messageLabel->setText("No networks found");
        QTimer::singleShot(global::toast::delay, this, SLOT(close()));
    }
}

void toast::refreshScreenNative() {
    emit refreshScreen();
}

void toast::updateWifiIcon(int mode) {
    emit updateWifiIconSig(mode);
}

void toast::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

void toast::closeIndefiniteToastNative() {
    emit closeIndefiniteToast();
}
