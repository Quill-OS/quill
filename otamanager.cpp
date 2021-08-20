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
    if(QFile::exists("/mnt/onboard/onboard/.inkbox/SKIP_OTACHECK") == false) {
        qDebug() << "Checking for available OTA update ...";
        if(global::otaUpdate::downloadOta != true) {
            string_writeconfig("/opt/ibxd", "ota_update_check\n");
            while(true) {
                if(QFile::exists("/run/can_ota_update") == true) {
                    if(checkconfig("/run/can_ota_update") == true) {
                        emit canOtaUpdate(true);
                        qDebug() << "OTA update is available!";
                        break;
                    }
                    else {
                        emit canOtaUpdate(false);
                        qDebug() << "No OTA update available.";
                        break;
                    }
                }
            }
            QThread::msleep(500);
            otaManager::close();
        }
        else {
            string_writeconfig("/opt/ibxd", "ota_update_download\n");
            while(true) {
                if(QFile::exists("/run/can_install_ota_update") == true) {
                    if(checkconfig("/run/can_install_ota_update") == true) {
                        emit downloadedOta(true);
                        global::otaUpdate::downloadOta = false;
                        break;
                    }
                    else {
                        emit downloadedOta(false);
                        global::otaUpdate::downloadOta = false;
                        break;
                    }
                }
            }
            QThread::msleep(500);
            otaManager::close();
        }
    }
    else {
        qDebug() << "Skip OTA update check!";
    }
}

otaManager::~otaManager()
{
    delete ui;
}
