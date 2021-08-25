#include "otamanager.h"
#include "ui_otamanager.h"
#include "functions.h"

#include <QTimer>
#include <QDebug>

otaManager::otaManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::otaManager)
{
    ui->setupUi(this);
    QThread::msleep(500);
    if(global::otaUpdate::downloadOta == false) {
        qDebug() << "Checking for available OTA update ...";
        string_writeconfig("/opt/ibxd", "ota_update_check\n");
        QTimer * otaCheckTimer = new QTimer(this);
        otaCheckTimer->setInterval(100);
        connect(otaCheckTimer, &QTimer::timeout, [&]() {
            if(QFile::exists("/run/can_ota_update") == true) {
                if(checkconfig("/run/can_ota_update") == true) {
                    qDebug() << "OTA update is available!";
                    emit canOtaUpdate(true);
                }
                else {
                    qDebug() << "No OTA update available.";
                    emit canOtaUpdate(false);
                }
                otaManager::close();
            }
        } );
        otaCheckTimer->start();
    }
    else {
        qDebug() << "Downloading OTA update ...";
        QFile::remove("/run/can_install_ota_update");
        string_writeconfig("/opt/ibxd", "ota_update_download\n");
        QTimer * otaDownloadTimer = new QTimer(this);
        otaDownloadTimer->setInterval(100);
        connect(otaDownloadTimer, &QTimer::timeout,  [&]() {
            if(QFile::exists("/run/can_install_ota_update") == true) {
                if(checkconfig("/run/can_install_ota_update") == true) {
                    emit downloadedOtaUpdate(true);
                    global::otaUpdate::downloadOta = false;
                }
                else {
                    emit downloadedOtaUpdate(false);
                    global::otaUpdate::downloadOta = false;
                }
                otaManager::close();
            }
        } );
        otaDownloadTimer->start();
    }
}

otaManager::~otaManager()
{
    delete ui;
}
