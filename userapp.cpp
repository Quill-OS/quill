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
    appName = name; // Its for searching for json entry while disabling / enabling
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

    execPath.setFileName(appDir.path() + "/" + jsonInfo["ExecPath"].toString());

    userAppEnabled = jsonInfo["Enabled"].toBool();
    if(userAppEnabled == true)
    {
        ui->pushButton_2EnablingButton->setText("Disable");
    } else {
        ui->pushButton_2EnablingButton->setText("Enable");
    }
}

// This function is needed when we dont want to repaint all widgets, but only change the the page ( when no changes to json were applied )
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
    if(userAppEnabled == false)
    {
        userAppEnabled = true;
        ui->pushButton_2EnablingButton->setText("Disable");
    } else {
        userAppEnabled = false;
        ui->pushButton_2EnablingButton->setText("Enable");
    }

    // this looks the entire json for its entry and changes the Enabled bool. also checks if it actually changed
    // https://forum.qt.io/topic/100810/how-to-modify-data-and-save-to-json-file/4
    // https://stackoverflow.com/questions/69492325/update-value-in-qjsonarray-and-write-back-to-json-file-in-qt
    // https://stackoverflow.com/questions/19267335/qt-modifying-a-json-file
    // Resources for the future

    QJsonObject jsonRootObject = jsonDocument.object();
    QJsonArray jsonArrayList = jsonRootObject["list"].toArray();

    int arraySize = jsonArrayList.size();

    for(int i = 0; i <= arraySize; i++)
    {
        QJsonObject jsonObject = jsonArrayList.at(i).toObject();
        QString entryName = jsonObject["Name"].toString();
        if(entryName == appName)
        {
            jsonObject.insert("Enabled", QJsonValue(userAppEnabled));

            jsonArrayList.replace(i, jsonObject);

            jsonRootObject["list"] = jsonArrayList;

            jsonDocument.setObject(jsonRootObject);

            QFile jsonFile = jsonFilePath;
            QString message = "Accesing json: ";
            message.append(jsonFile.fileName());
            log(message, className);

            // Here is maybe a filesystem sync problem - needs further testing
            jsonFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            jsonFile.write(jsonDocument.toJson());
            jsonFile.close();
        }
    }
}
