#include "otamanager.h"
#include "ui_otamanager.h"
#include "functions.h"

#include <QTimer>

otaManager::otaManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::otaManager)
{
    ui->setupUi(this);
    string_writeconfig("/opt/ibxd", "ota_update_check\n");
    QTimer *otaCheckTimer = new QTimer();
    otaCheckTimer->setInterval(100);
    connect(otaCheckTimer, &QTimer::timeout, [&]() {
        if(QFile::exists("/run/can_install_ota_update") == true) {
            if(checkconfig("/run/can_install_ota_update") == true) {
                emit canInstallOtaUpdate(true);
                otaManager::close();
            }
            else {
                emit canInstallOtaUpdate(false);
                otaManager::close();
            }
        }
    } );
}

otaManager::~otaManager()
{
    delete ui;
}
