#include <QFile>
#include <QScreen>

#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "generaldialog.h"
#include "functions.h"

connectiondialog::connectiondialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectiondialog)
{
    ui->setupUi(this);
    this->setFont(QFont("u001"));

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->cancelBtn->setStyleSheet("font-size: 9pt");
    ui->connectBtn->setStyleSheet("font-size: 9pt");
    ui->showPasswordBtn->setStyleSheet("font-size: 9pt");

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->setFixedWidth(screenGeometry.width());


}

connectiondialog::~connectiondialog()
{
    delete ui;
}

void connectiondialog::applyVariables() {
    // Here, for some devices it will be propably needed to limit the size
    ui->nameLabel->setText(connectedNetworkData.name);
    ui->macLabel->setText(connectedNetworkData.mac);
    ui->signalLabel->setText(QString::number(connectedNetworkData.signal) + "%");

    if(connectedNetworkData.encryption == false) {
        ui->showPasswordBtn->hide();
        ui->passwordTextEdit->setText("No password required");
        ui->passwordTextEdit->setDisabled(true);
    }
    else {
        if(passwordDatabase.exists() == false) {
            log("Creating empty database", className);
            // https://forum.qt.io/topic/104791/how-i-can-create-json-format-in-qt/5
            QJsonObject root;
            QJsonArray array;
            QJsonDocument newJsonDocument;
            root["list"] = array;
            newJsonDocument.setObject(root);

            passwordDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            passwordDatabase.write(newJsonDocument.toJson());
            passwordDatabase.flush();
            passwordDatabase.close();
        }
        QString password = searchDatabase(connectedNetworkData.name);
        if(password.isEmpty() == false) {
            log("Found password: " + password, className);
            ui->showPasswordBtn->setIcon(QIcon(":/resources/show.png"));
            showedPassword = false;
            savedPassword = password;

            ui->passwordTextEdit->setText("********");
        }
        else {
            log("No password found", className);
            ui->passwordTextEdit->setText("No password was saved");
            showedPassword = true;
            ui->showPasswordBtn->hide();
        }
    }
}

QString connectiondialog::searchDatabase(QString key) {
    passwordDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passwordDatabase.readAll();
    passwordDatabase.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

    if(jsonDocument["list"].isArray() == true) {
        QJsonArray jsonArray = jsonDocument["list"].toArray();
        for(QJsonValueRef refJsonObject: jsonArray) {
            QJsonObject jsonMainObject = refJsonObject.toObject();
            QString searchedName = jsonMainObject.keys().first().toUtf8();
            log("Found in database: '" + searchedName + "'", className);
            if(searchedName == key) {
                QString returnedPassword = jsonMainObject.value(key).toString();
                log("Searched name '" + searchedName + "' matched '" + key + "' and the password is: '" + returnedPassword + "'", className);
                return returnedPassword;
            }
            else {
                log("Searched name '" + searchedName + "' doesn't match '" + key + "'", className);
            }
        }
        return "";
    }
    else {
        log("Something went horribly wrong", className);
    }
}

void connectiondialog::writeToDatabase(QString name, QString password) {
    if(searchDatabase(name).isEmpty() == false) {
        removeFromDatabase(name);
    }

    passwordDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passwordDatabase.readAll();
    passwordDatabase.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

    if(jsonDocument["list"].isArray() == true) {
        QJsonArray jsonArray = jsonDocument["list"].toArray();
        QJsonValue newValue;

        // https://stackoverflow.com/questions/26804660/how-to-initialize-qjsonobject-from-qstring
        // I hoped this would be easier
        QJsonObject newObject = QJsonDocument::fromJson(QString("{\"" + name + "\" : \"" + password + "\" }").toUtf8()).object();
        jsonArray.append(newObject);

        QJsonDocument newJsonDocument;
        QJsonObject root;
        root["list"] = jsonArray;
        newJsonDocument.setObject(root);

        passwordDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        passwordDatabase.write(newJsonDocument.toJson());
        passwordDatabase.flush();
        passwordDatabase.close();
    }
    else {
        log("Something went horribly wrong", className);
    }
}

void connectiondialog::removeFromDatabase(QString name) {
    passwordDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passwordDatabase.readAll();
    passwordDatabase.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

    int counter = 0;
    bool remove = false;
    if(jsonDocument["list"].isArray() == true) {
        QJsonArray jsonArray = jsonDocument["list"].toArray();
        for(QJsonValueRef refJsonObject: jsonArray) {
            QJsonObject jsonMainObject = refJsonObject.toObject();
            QString searchedName = jsonMainObject.keys().first().toUtf8();
            log("Found in database: '" + searchedName + "'", className);
            if(searchedName == name) {
                remove = true;
            }
            if(remove == false) {
                counter = counter + 1;
            }
        }
        if(remove == true) {
            jsonArray.removeAt(counter);
            QJsonDocument newJsonDocument;
            QJsonObject root;
            root["list"] = jsonArray;
            newJsonDocument.setObject(root);

            passwordDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            passwordDatabase.write(newJsonDocument.toJson());
            passwordDatabase.flush();
            passwordDatabase.close();
        }
        else {
            log("ERROR: tried to remove from database, but couldn't find key", className);
        }
    }
}

void connectiondialog::on_cancelBtn_clicked()
{
    this->deleteLater();
    this->close();
}

void connectiondialog::on_passwordTextEdit_selectionChanged()
{
    ui->passwordTextEdit->setSelection(0, 0);
}

void connectiondialog::on_passwordTextEdit_cursorPositionChanged(int oldpos, int newpos)
{
    log("Detected click on text edit widget", className);
    if(cursorPositionIgnore == true) {
        if(newpos != 0) {
            if(showedPassword == true) {
                ui->passwordTextEdit->setCursorPosition(0);
                global::keyboard::keyboardDialog = true;
                global::keyboard::wifiPassphraseDialog = true;
                global::keyboard::keyboardText = "";
                generalDialog* generalDialogWindow = new generalDialog();
                generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
                generalDialogWindow->wifiEssid = connectedNetworkData.name;
                connect(generalDialogWindow, &generalDialog::showToast, this, &connectiondialog::showToastSlot);
                connect(generalDialogWindow, &generalDialog::refreshScreen, this, &connectiondialog::refreshScreenSlot);

                generalDialogWindow->exec();

                global::keyboard::keyboardDialog = false;
                global::keyboard::wifiPassphraseDialog = false;
                if(global::keyboard::keyboardText.isEmpty() == false) {
                    // A bit hacky: avoid summoning the keyboard back when the text is changing (and the cursor too) showedPassword shouldn't be used for this, but it works and adding another boolean would start being messy
                    showedPassword = false;
                    ui->passwordTextEdit->setText(global::keyboard::keyboardText);
                    ui->showPasswordBtn->setIcon(QIcon(":/resources/hide.png"));
                    ui->showPasswordBtn->show();
                    showedPassword = true;
                    savedPassword = global::keyboard::keyboardText;
                    ui->showPasswordBtn->show();
                }
                global::keyboard::keyboardText = "";
            }
            else {
                log("Password is not saved; ignoring text edit call", className);
            }
        }
    }
    else {
        log("Ignoring click on text edit", className);
        cursorPositionIgnore = true;
    }
}

void connectiondialog::showToastSlot(QString message) {
    emit showToastSignal(message);
}

void connectiondialog::on_showPasswordBtn_clicked()
{
    if(showedPassword == false) {
        ui->showPasswordBtn->setIcon(QIcon(":/resources/hide.png"));
        ui->passwordTextEdit->setText(savedPassword);
        showedPassword = true;
    }
    else {
        showedPassword = false;
        ui->showPasswordBtn->setIcon(QIcon(":/resources/show.png"));
        ui->passwordTextEdit->setText("********");
    }
}

void connectiondialog::refreshScreenSlot() {
    this->repaint();
    emit refreshScreenSignal();
}

void connectiondialog::on_connectBtn_clicked()
{
    QString finalPassword;
    if(connectedNetworkData.encryption == false) {
        finalPassword = "NONE";
    }
    else {
        if(savedPassword.isEmpty() == false) {
            finalPassword = savedPassword;
            writeToDatabase(connectedNetworkData.name, savedPassword);
        }
        else {
            showToastSlot("Provide a password first");
            return void();
        }
    }
    passwordForReconnecting = finalPassword;

    ui->cancelBtn->setEnabled(false);
    if(checkWifiState() == global::wifi::wifiState::configured) {
        string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
    }
    writeFile("/run/wifi_network_essid", connectedNetworkData.name);
    writeFile("/run/wifi_network_passphrase", finalPassword);
    finalConnectWait();
}

void connectiondialog::finalConnectWait() {
    if(checkIfWifiBusy() == true) {
        // To be sure
        if(waitTry == 10) {
            string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
        }
        // Wait for everything to shut down; 10 seconds timeout
        if(waitTry == 20) {
            string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
            emit showToastSignal("Failed to stop other Wi-Fi processes");
            ui->cancelBtn->setEnabled(true);
        }
        else {
            QTimer::singleShot(500, this, SLOT(finalConnectWait()));
            waitTry = waitTry + 1;
        }
    }
    else {
        string_writeconfig("/opt/ibxd", "connect_to_wifi_network\n");

        // This will be deleted later in MainWindow's icon updater if it failed. It is also deleted in the Wi-Fi stop script
        log("Writing to config directory with connection information data", className);
        string_writeconfig("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/essid", connectedNetworkData.name.toStdString());
        string_writeconfig("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/passphrase", passwordForReconnecting.toStdString());

        this->deleteLater();
        this->close();
    }
}

bool connectiondialog::checkIfWifiBusy() {
    if(checkProcessName("connect_to_network.sh") == true or checkProcessName("connection_manager.sh") == true or checkProcessName("prepare_changing_wifi.sh") == true) {
        return true;
    }
    else {
        return false;
    }
}
