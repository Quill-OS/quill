#include "userapp.h"
#include "ui_userapp.h"
#include "mainwindow.h"
#include "generaldialog.h"

userapp::userapp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userapp)
{
    ui->setupUi(this);
    ui->pushButtonLaunch->setProperty("type", "borderless");
    ui->pushButtonLaunch->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    ui->pushButtonIcon->setProperty("type", "borderless");

    ui->pushButtonAppInfo->setProperty("type", "borderless");
    ui->pushButton_2EnablingButton->setProperty("type", "borderless");
    ui->pushButton_2EnablingButton->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
}

userapp::~userapp()
{
    delete ui;
}

void userapp::provideInfo(QJsonObject jsonInfo)
{
    QString name = jsonInfo["Name"].toString();
    appName = name; // Its for searching for json entry while disabling / enabling
    jsonObject = jsonInfo;
    // Limit name size to avoid breaking the gui
    if(name.size() > 20)
    {
        // If someone wants to break the gui, they will do it ¯\^-^/¯
        // Idk if this will work for every device
        name.remove(16, 100);
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
    // Show a big qdialog with the whole part json in it
    // this is higly bad code
    log("Launching json information dialog", className);
    // https://stackoverflow.com/questions/28181627/how-to-convert-a-qjsonobject-to-qstring
    QString jsonStringParsed = parseJsonShow(jsonObject);
    global::text::textBrowserContents = jsonStringParsed;

    global::userApps::appInfoDialog = true;
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);


    //connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupSearchDialog()));
    //connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    //connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
    //connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
    //connect(generalDialogWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
    generalDialogWindow->show();
}

void userapp::on_pushButton_2EnablingButton_clicked()
{
    ui->pushButton_2EnablingButton->setEnabled(false);

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

    for(int i = 0; i < arraySize; i++)
    {
        QJsonObject jsonObject = jsonArrayList.at(i).toObject();
        QString entryName = jsonObject["Name"].toString();

        /*
        QString message = "JSON Searching for: ";
        message.append(appName);
        message.append(" Found: ");
        message.append(entryName);
        log(message, className);
        */

        if(entryName == appName)
        {
            jsonObject.insert("Enabled", QJsonValue(userAppEnabled));

            // Its accessing jsonDocument again becouse it could be changed via updateJsonFileSlot
            QJsonArray sonArrayListNew = jsonDocument.object()["list"].toArray();
            sonArrayListNew.replace(i, jsonObject);

            jsonRootObject["list"] = sonArrayListNew;

            jsonDocument.setObject(jsonRootObject);
            emit updateJsonFileSignalUA(jsonDocument);
        }
    }
    ui->pushButton_2EnablingButton->setEnabled(true);
}

void userapp::updateJsonFileSlotUA(QJsonDocument jsonDocumentProvided)
{
    jsonDocument = jsonDocumentProvided;
}

void userapp::on_pushButtonLaunch_clicked()
{
    // Some command to execute binary at "execPath"
    // For now this:
    QString supportedDevices = jsonObject["SupportedDevices"].toString();
    QString message = "Supported devices for this app: ";
    message.append(supportedDevices);
    log(message, className);

    // you know that holding \n in deviceID is horrible?
    QString betterDeviceId = global::deviceID.replace("\n", "");

    if(supportedDevices.contains("all") == false and supportedDevices.contains(betterDeviceId) == false)
    {
        global::userApps::appCompabilityDialog = true;
        generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        global::userApps::appCompabilityText = "<font face='u001'>Your device is not compatible with this app, launch anyway</font><font face='Inter'>?</font>";

        //connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupSearchDialog()));
        //connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
        //connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
        //connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
        //connect(generalDialogWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
        // Needs to be exec!
        generalDialogWindow->exec();
    } else {
        global::userApps::launchApp = true;
    }

    // syntax question:
    if(manageRequiredFeatures() == true)
    {
        if(global::userApps::launchApp == true)
        {
            global::userApps::launchApp = false;
            QString message = "Launching user app at: ";
            message.append(execPath.fileName());
            log(message, className);

            QProcess process;
            process.startDetached(execPath.fileName(), QStringList());
            qApp->quit();
            // mount -o remount,exec /kobo/mnt/onboard/onboard is needed
        }
    }
}

bool userapp::manageRequiredFeatures()
{
    QJsonArray jsonArray = jsonObject["RequiredFeatures"].toArray();
    for(QJsonValueRef refJsonObject: jsonArray)
    {
        bool launchDialog = false;
        int featureId = refJsonObject.toInt();
        // Wi-Fi connection required
        if(featureId == 0)
        {
            global::userApps::appCompabilityText = "<font face='u001'>This app needs Wi-Fi connection, launch anyway</font><font face='Inter'>?</font>";
            launchDialog = true;
        }
        // Rooted kernel required
        if(featureId == 1)
        {
            global::userApps::appCompabilityText = "<font face='u001'>This app needs a rooted kernel, launch anyway</font><font face='Inter'>?</font>";
            launchDialog = true;
        }

        if(launchDialog == true)
        {
            global::userApps::appCompabilityDialog = true;
            generalDialogWindow = new generalDialog();
            generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
            //connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupSearchDialog()));
            //connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
            //connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
            //connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
            //connect(generalDialogWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFile(QString, bool)));
            // Needs to be exec!
            generalDialogWindow->exec();
        }

        if(global::userApps::appCompabilityLastContinueStatus == false)
        {
            return false;
        }
    }
    return true;
}

QString userapp::parseJsonShow(QJsonObject json)
{
    QString mainString;
    foreach(const QString& key, json.keys())
    {
        QJsonValue value = json.value(key);

        QString appendString;

        appendString.append("<b>");
        appendString.append(key);
        appendString.append("</b>: ");

        if(value.isString())
        {
            appendString.append(value.toString());
        } else if(value.isBool())
        {
            appendString.append(QVariant(value.toBool()).toString());
        } else if(value.isArray())
        {
            QJsonArray array = value.toArray();
            for(QJsonValueRef ref: array)
            {
                int id = ref.toInt();
                if(id == 0)
                {
                    appendString.append("Wi-Fi connection");
                } else if(id == 1)
                {
                    appendString.append("Rooted kernel");
                }
                appendString.append(", ");
            }
            appendString.remove(appendString.size() - 2, 2);
        }

        appendString.append("<br>");

        mainString.append(appendString);
    }

    return mainString;
}

