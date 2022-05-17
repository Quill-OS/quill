#include "userapp.h"
#include "ui_userapp.h"
#include "mainwindow.h"

userapp::userapp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userapp)
{
    ui->setupUi(this);
    ui->pushButtonLaunch->setProperty("type", "borderless");
    ui->pushButtonLaunch->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    ui->pushButtonIcon->setProperty("type", "borderless");
}

userapp::~userapp()
{
    delete ui;
}

void userapp::provideInfo(QJsonObject jsonInfo)
{
    QString name = jsonInfo["Name"].toString();
    // Limit name size to avoid breaking the gui
    if(name.size() > 20)
    {
        // If someone wants to break the gui, they will do it ¯\^-^/¯
        // Idk if this will work for every device
        name.remove(14, 100);
    }
    ui->labelAppName->setText(name);

    appDir.setPath("/mnt/onboard/onboard/.apps/" + name);

    QFile iconPath = QFile{appDir.path() + "/" + jsonInfo["IconPath"].toString()};
    if(iconPath.exists() == true)
    {
        QIcon appIcon = QIcon(iconPath.fileName());
        ui->pushButtonIcon->setIconSize(QSize(40, 40));
        ui->pushButtonIcon->setIcon(appIcon);
    } else {
        QString message = "Icon not found: ";
        message.append(iconPath.fileName());
        log(message, className);
    }



}
