#include "otamanager.h"
#include "ui_otamanager.h"
#include "functions.h"

#include <QTimer>

otaManager::otaManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::otaManager)
{
    ui->setupUi(this);
    if(global::otaUpdate::downloadOta != true) {
        string_writeconfig("/opt/ibxd", "ota_update_check\n");
        QTimer *otaCheckTimer = new QTimer();
        otaCheckTimer->setInterval(100);
        connect(otaCheckTimer, &QTimer::timeout, [&]() {
            if(QFile::exists("/run/can_ota_update") == true) {
                if(checkconfig("/run/can_ota_update") == true) {
                    emit canOtaUpdate(true);
                    otaManager::close();
                }
                else {
                    emit canOtaUpdate(false);
                    otaManager::close();
                }
            }
        } );
    }
    else {
        string_writeconfig("/opt/ibxd", "ota_update_download\n");
        QTimer *otaDownloadTimer = new QTimer();
        otaDownloadTimer->setInterval(500);
        connect(otaDownloadTimer, &QTimer::timeout,  [&]() {
            if(QFile::exists("/run/can_install_ota_update") == true) {
                if(checkconfig("/run/can_install_ota_update") == true) {
                    emit downloadedOta(true);
                    global::otaUpdate::downloadOta = false;
                    otaManager::close();
                }
                else {
                    emit downloadedOta(false);
                    global::otaUpdate::downloadOta = false;
                    otaManager::close();
                }
            }
        } );
    }
}

otaManager::~otaManager()
{
    delete ui;
}
