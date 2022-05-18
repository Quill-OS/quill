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
        name.remove(14, 50);
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

    execPath.setFileName(appDir.path() + "/" + jsonInfo["ExecPath"].toString());
}

void userapp::changePageEnabling(bool SecondPage)
{
    if(SecondPage == true)
    {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void userapp::on_pushButtonAppInfo_clicked()
{
    // Show a big qdialog with the whole part json in it :)
}



void userapp::on_pushButton_2EnablingButton_clicked()
{
    // Here the text on this button is used as a bool. No need to create a new one
    // Disable and Enable
    bool setJsonEnabled;
    if(ui->pushButton_2EnablingButton->text() == "Disable")
    {
        ui->pushButton_2EnablingButton->setText("Enable");
        setJsonEnabled = false;

    }
    if(ui->pushButton_2EnablingButton->text() == "Enable")
    {
        ui->pushButton_2EnablingButton->setText("Disable");
        setJsonEnabled = true;
    }
    emit remakeApps();
    // this looks the entire json for its entry and changes the Enabled bool. also checks if it actually changed
    // https://forum.qt.io/topic/100810/how-to-modify-data-and-save-to-json-file/4
    QJsonObject jsonRootObject = jsonDocument.object();
    QJsonArray jsonArrayList = jsonRootObject["list"].toArray();
    // Here its maybe possible to use QJsonValueRef ref = RootObject.find("Address").value();
    // But im not sure
    for(QJsonValueRef refJsonObject: jsonArrayList)
    {
        QJsonObject jsonObject = refJsonObject.toObject();
        QString entryName = jsonObject["Name"].toString();
        if(entryName == ui->labelAppName->text())
        {
            jsonObject.insert("Enabled", setJsonEnabled);
            refJsonObject = jsonObject;
            jsonDocument.setObject(jsonRootObject);

            QFile jsonFile = jsonFilePath; // Will this work?
            jsonFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            jsonFile.write(jsonDocument.toJson());
            jsonFile.close();
        }

    }
}
