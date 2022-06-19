#include "userapps.h"
#include "ui_userapps.h"
#include "mainwindow.h"
#include "generaldialog.h"

userapps::userapps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userapps)
{
    ui->setupUi(this);
    ui->launchBtn->setProperty("type", "borderless");
    ui->launchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    ui->launchBtn->setProperty("type", "borderless");

    ui->iconBtn->setProperty("type", "borderless");
    ui->iconBtn->setStyleSheet("QPushButton[type='borderless']:pressed { background: white; color: white; border: none; }");

    ui->infoBtn->setProperty("type", "borderless");
    ui->statusBtn->setProperty("type", "borderless");
    ui->statusBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
}

userapps::~userapps()
{
    delete ui;
}

void userapps::provideInfo(QJsonObject jsonInfo)
{
    QString name = jsonInfo["Name"].toString();
    appName = name; // It is for searching for json entry while disabling / enabling
    jsonObject = jsonInfo;
    // Limit name size to avoid breaking the GUI
    if(name.size() > 20) {
        // If someone wants to break the GUI, they will do it ¯\^-^/¯
        name.remove(16, 100);
    }
    ui->appNameLabel->setText(name);

    appDir.setPath("/mnt/onboard/onboard/.apps/" + name + "/" + name);

    QFile iconPath = QFile{appDir.path() + "/" + jsonInfo["IconPath"].toString()};
    if(iconPath.exists() == true) {
        QIcon appIcon = QIcon(iconPath.fileName());
        ui->iconBtn->setIconSize(QSize(40, 40));
        ui->iconBtn->setIcon(appIcon);
    }
    else {
        QString function = __func__;
        QString message = ": Warning: Icon not found: ";
        message.append(iconPath.fileName());
        log(function + message, className);
        ui->iconBtn->deleteLater();
        ui->gridLayout->deleteLater();
    }

    execPath.setFileName("/" + jsonInfo["ExecPath"].toString());

    userAppEnabled = jsonInfo["Enabled"].toBool();
    if(userAppEnabled == true) {
        ui->statusBtn->setText("Disable");
    }
    else {
        ui->statusBtn->setText("Enable");
    }
}

// This function is needed when we dont want to repaint all widgets, but only change the the page (when no changes to the main JSON file were applied)
void userapps::changePageEnabling(bool SecondPage)
{
    if(SecondPage == true){
        ui->stackedWidget->setCurrentIndex(1);
    }
    else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void userapps::on_infoBtn_clicked()
{
    log("Launching user application information dialog", className);
    // https://stackoverflow.com/questions/28181627/how-to-convert-a-qjsonobject-to-qstring
    QString jsonStringParsed = parseJsonShow(jsonObject);
    global::text::textBrowserContents = jsonStringParsed;

    global::userApps::appInfoDialog = true;
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    generalDialogWindow->show();
}

void userapps::on_statusBtn_clicked()
{
    ui->statusBtn->setEnabled(false);

    // Here the text on this button is used as a boolean; no need to create a new one
    // Disable and Enable
    if(userAppEnabled == false) {
        userAppEnabled = true;
        ui->statusBtn->setText("Disable");
    }
    else {
        userAppEnabled = false;
        ui->statusBtn->setText("Enable");
    }

    QJsonObject jsonRootObject = jsonDocument.object();
    QJsonArray jsonArrayList = jsonRootObject["list"].toArray();

    int arraySize = jsonArrayList.size();

    for(int i = 0; i < arraySize; i++) {
        QJsonObject jsonObject = jsonArrayList.at(i).toObject();
        QString entryName = jsonObject["Name"].toString();

        if(entryName == appName) {
            jsonObject.insert("Enabled", QJsonValue(userAppEnabled));

            QJsonArray sonArrayListNew = jsonDocument.object()["list"].toArray();
            sonArrayListNew.replace(i, jsonObject);

            jsonRootObject["list"] = sonArrayListNew;

            jsonDocument.setObject(jsonRootObject);
            emit updateJsonFileSignalUA(jsonDocument);
        }
    }
    ui->statusBtn->setEnabled(true);
}

void userapps::updateJsonFileSlotUA(QJsonDocument jsonDocumentProvided)
{
    jsonDocument = jsonDocumentProvided;
}

void userapps::on_launchBtn_clicked()
{
    // Some command to execute script or binary at "ExecPath"
    QString supportedDevices = jsonObject["SupportedDevices"].toString();
    QString message = "Supported devices for this app: ";
    message.append(supportedDevices);
    log(message, className);

    if(supportedDevices.contains("all") == false and supportedDevices.contains(global::deviceID.trimmed()) == false) {
        log("Warning: User app does not support this device", className);
        global::userApps::appCompatibilityDialog = true;
        global::userApps::appCompatibilityText = "<font face='u001'>Your device is not compatible with this app.<br>Launch it anyway</font><font face='Inter'>?</font>";
        generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);

        generalDialogWindow->exec();
    }
    else {
        global::userApps::launchApp = true;
    }

    if(manageRequiredFeatures() == true) {
        if(global::userApps::launchApp == true) {
            global::userApps::launchApp = false;
            QString message = "Launching user application at: ";
            message.append(appDir.path() + execPath.fileName());
            log(message, className);

            QProcess process;
            QStringList args;
            args << appDir.path() << execPath.fileName();
            process.startDetached("launch_user_application.sh", args);
            qApp->quit();
        }
    }
}

bool userapps::manageRequiredFeatures()
{
    QJsonArray jsonArray = jsonObject["RequiredFeatures"].toArray();
    for(QJsonValueRef refJsonObject: jsonArray) {
        bool launchDialog = false;
        int featureId = refJsonObject.toInt();
        // Wi-Fi connection required
        if(featureId == 0) {
            global::userApps::appCompatibilityText = "<font face='u001'>This app needs Wi-Fi connection, launch anyway</font><font face='Inter'>?</font>";
            launchDialog = true;
        }
        // Rooted kernel required
        if(featureId == 1) {
            global::userApps::appCompatibilityText = "<font face='u001'>This app needs a rooted kernel, launch anyway</font><font face='Inter'>?</font>";
            launchDialog = true;
        }

        if(launchDialog == true) {
            global::userApps::appCompatibilityDialog = true;
            generalDialogWindow = new generalDialog();
            generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
            generalDialogWindow->exec();
        }

        if(global::userApps::appCompatibilityLastContinueStatus == false) {
            return false;
        }
    }
    return true;
}

QString userapps::parseJsonShow(QJsonObject json)
{
    QString mainString;
    foreach(const QString& key, json.keys()) {
        QJsonValue value = json.value(key);

        QString appendString;

        appendString.append("<b>");
        appendString.append(key);
        appendString.append("</b>: ");

        if(value.isString()) {
            appendString.append(value.toString());
        }
        else if(value.isBool()) {
            appendString.append(QVariant(value.toBool()).toString());
        }
        else if(value.isArray()) {
            QJsonArray array = value.toArray();
            for(QJsonValueRef ref: array) {
                int id = ref.toInt();
                if(id == 0) {
                    appendString.append("Wi-Fi connection");
                }
                else if(id == 1) {
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
