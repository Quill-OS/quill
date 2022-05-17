#include "apps.h"
#include "ui_apps.h"
#include "mainwindow.h"
#include "userapp.h"

#include <QFile>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug> // szybet testing

// Json scheme:
/*
{
    "list": [
      {
        "Name": "Sanki",
        "Author": "Szybet",
        "Version": "0.1 testing",
        "IconPath": "/sanki.png",
        "ExecPath": "/sanki",
        "Signed": true,
        "Enabled": false,
        "SupportedDevices": "n305,xxx,xxx"
      },
      {
        "Name": "Syncthing",
        "Author": "Szybet",
        "Version": "0.1 testing",
        "IconPath": "/syncthing.png",
        "ExecPath": "/syncthing_arm.bin",
        "Signed": false,
        "Enabled": false,
        "SupportedDevices": "n305,xxx,xxx"
      }
    ]
}
*/
// Its located at: /data/onboard/.apps/apps.json so in chroot: /mnt/onboard/onboard/.apps/apps.json



apps::apps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apps)
{
    ui->setupUi(this);
    ui->koboxAppsOpenButton->setProperty("type", "borderless");
    ui->scribbleLaunchBtn->setProperty("type", "borderless");
    ui->lightmapsLaunchBtn->setProperty("type", "borderless");
    ui->calendarLaunchBtn->setProperty("type", "borderless");
    ui->savedWordsLaunchBtn->setProperty("type", "borderless");
    ui->calculatorLaunchBtn->setProperty("type", "borderless");
    ui->vncLaunchBtn->setProperty("type", "borderless");
    ui->reversiLaunchBtn->setProperty("type", "borderless");
    ui->g2048LaunchBtn->setProperty("type", "borderless");

    ui->koboxAppsOpenButton->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->scribbleLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->lightmapsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calendarLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->savedWordsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calculatorLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->vncLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->reversiLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->g2048LaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    // Hiding KoBox apps button and label if X11 isn't enabled/wasn't started
    if(checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
        ui->label_5->hide();
        ui->koboxAppsOpenButton->hide();
        ui->label_5->deleteLater();
        ui->koboxAppsOpenButton->deleteLater();
    }
    // Hiding VNC viewer button and label if device is not rooted and doesn't have a working Wi-Fi adapter, or if X11 is not running
    if(checkconfig("/external_root/opt/root/rooted") == false) {
        if(global::device::isWifiAble == false or checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
            ui->vncViewerLabel->hide();
            ui->vncLaunchBtn->hide();
            ui->vncViewerLabel->deleteLater();
            ui->vncLaunchBtn->deleteLater();
        }
    }
    else {
        if(checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
            ui->vncViewerLabel->hide();
            ui->vncLaunchBtn->hide();
            ui->vncViewerLabel->deleteLater();
            ui->vncLaunchBtn->deleteLater();
        }
    }
    // Hiding Light Maps if device doesn't have a working Wi-Fi adapter
    if(global::device::isWifiAble == false and global::deviceID != "emu\n") {
        ui->label_2->hide();
        ui->lightmapsLaunchBtn->hide();
        ui->label_2->deleteLater();
        ui->lightmapsLaunchBtn->deleteLater();
    }

    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Refresh becouse qt shows the scrollbar for one second and hides it, leaving a mark on the e-ink
    QTimer::singleShot(1750, this, SLOT(refreshScreenNative()));
    if(parseJson() == true)
    {
        log("Json is correct", className);
        QJsonArray jsonListArray = jsonDocument.object()["list"].toArray();
            for(QJsonValueRef refJsonObject: jsonListArray)
            {
                QJsonObject appInfo = refJsonObject.toObject();
                userapp* newUserApp = new userapp;
                newUserApp->provideInfo(appInfo);

                ui->verticalLayout_4UserApps->addWidget(newUserApp);

            }
    } else {
        log("Json is not correct", className);
        showToastNative("ERROR: Failed to parse apps.json");
    }
}

apps::~apps()
{
    delete ui;
}

void apps::exitSlot() {
    apps::close();
}

void apps::on_scribbleLaunchBtn_clicked()
{
    log("Launching external Scribble app", className);
    QProcess process;
    process.startDetached("scribble", QStringList());
    qApp->quit();
}

void apps::on_lightmapsLaunchBtn_clicked()
{
    log("Launching external LightMaps app", className);
    QProcess process;
    process.startDetached("lightmaps", QStringList());
    qApp->quit();
}

void apps::on_savedWordsLaunchBtn_clicked()
{
    log("Launching Saved Words app", className);
    savedWordsWindow = new savedwords();
    savedWordsWindow->setAttribute(Qt::WA_DeleteOnClose);
    savedWordsWindow->showFullScreen();
}

void apps::on_calendarLaunchBtn_clicked()
{
    log("Launching Calendar app", className);
    calendarWindow = new calendarApp();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);
    calendarWindow->showFullScreen();
}

void apps::on_calculatorLaunchBtn_clicked()
{
    log("Launching external Calculator app", className);
    QProcess process;
    process.startDetached("calculator", QStringList());
    qApp->quit();
}

void apps::on_koboxAppsOpenButton_clicked()
{
    log("Showing KoBox Apps Dialog", className);
    koboxAppsDialogWindow = new koboxAppsDialog();
    connect(koboxAppsDialogWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
    koboxAppsDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    koboxAppsDialogWindow->show();
}

void apps::on_vncLaunchBtn_clicked()
{
    log("Showing VNC dialog", className);
    global::keyboard::keyboardDialog = true;
    global::keyboard::vncDialog = true;
    global::keyboard::keyboardText = "";
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreenNative()));
    generalDialogWindow->show();
}

void apps::refreshScreenNative() {
    emit refreshScreen();
}

void apps::on_reversiLaunchBtn_clicked()
{
    log("Launching external Reversi app", className);
    QProcess process;
    process.startDetached("qreversi", QStringList());
    qApp->quit();
}

void apps::on_g2048LaunchBtn_clicked()
{
    log("Launching external 2048 app", className);
    QProcess process;
    process.startDetached("2048", QStringList());
    qApp->quit();
}

void apps::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

// This function Opens the json file, parses the json
// ( checks if its 100% correct ), to avoid problems and that in the future
// its not needed to check if a variable is a string, for example
//
// Here the code is a bit not elegant, but thanks to it in all other json
// related code will be. Also debugging the json is easier with this
// ~Szybet
bool apps::parseJson() {
    jsonFile.setFileName("/mnt/onboard/onboard/.apps/apps.json");
    bool check_sucess = true;

    if(jsonFile.exists() == false)
    {
        // Doesn't work currently
        log("App Json File is missing, creating it", className);
        check_sucess = false;
    } else {
        log("App Json File exists", className);
        jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString fileRead = jsonFile.readAll();
        jsonFile.close();

        jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());
        if(jsonDocument.isNull() == true)
        {
            log("Json file is not valid", className);
            check_sucess = false;
        } else {
            QJsonObject jsonObject = jsonDocument.object();
            if(jsonObject["list"].isArray() == true)
            {
                QJsonArray jsonArray = jsonObject["list"].toArray();
                for(QJsonValueRef refJsonObject: jsonArray)
                {
                    if(refJsonObject.isObject())
                    {
                        QJsonObject JsonMainObject = refJsonObject.toObject();
                        if(JsonMainObject.size() == 8)
                        {
                            if(!JsonMainObject["Name"].isString())
                            {
                                log("JSON: Invalid Name type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["Author"].isString())
                            {
                                log("JSON: Invalid Author type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["Version"].isString())
                            {
                                log("JSON: Invalid Version type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["IconPath"].isString())
                            {
                                log("JSON: Invalid IconPath type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["ExecPath"].isString())
                            {
                                log("JSON: Invalid ExecPath type inside object", className);
                                check_sucess = false;
                            } else {
                                if(JsonMainObject["ExecPath"].toString().contains(".."))
                                {
                                    // Security risk a bit it is
                                    showToastNative("ERROR: ExecPath has invalid path");
                                    log("JSON: ExecPath contains \"..\"", className);
                                }
                            }
                            if(!JsonMainObject["Signed"].isBool())
                            {
                                log("JSON: Invalid Signed type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["Enabled"].isBool())
                            {
                                log("JSON: Invalid Enabled type inside object", className);
                                check_sucess = false;

                            }
                            if(!JsonMainObject["SupportedDevices"].isString())
                            {
                                log("JSON: Invalid SupportedDevices type inside object", className);
                                check_sucess = false;

                            }
                        } else {
                            log("JSON: an object inside list array has too many items", className);
                            check_sucess = false;
                        }
                    } else {
                        log("JSON: list array has an item of other type than object", className);
                        check_sucess = false;
                    }
                }
            }
        }
    }
    return check_sucess;
}
