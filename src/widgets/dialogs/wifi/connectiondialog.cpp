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

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->CancelBtn->setStyleSheet("font-size: 9pt");
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
    // Here for some devices it will be propably needed to limit the size. the nia is fine
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
        else {
            QString password = searchDatabase(connectedNetworkData.name);
            if(password.isEmpty() == false) {
                log("found password: " + password, className);
                ui->showPasswordBtn->setIcon(QIcon("://resources/show.png"));
                showedPasword = false;
                savedPassword = password;

                ui->passwordTextEdit->setText("********");
            }
            else {
                log("No password found", className);
                ui->passwordTextEdit->setText("No password was saved");
                ui->showPasswordBtn->hide();
            }
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
            log("Found in database: " + searchedName, className);
            if(searchedName == key) {
                return jsonMainObject.take(key).toString();
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
        // I hoped this will be easier
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
            log("Found in database: " + searchedName, className);
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
            log("ERROR: tryied to remove from database, but couldn't find key", className);
        }
    }
}

void connectiondialog::on_CancelBtn_clicked()
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
    log("detected click on text edit", className);
    if(cursorPositionIgnore == true) {
        if(newpos != 0) {
            if(showedPasword == true) {
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
                    // A bit hacky: avoid summoning the keyboard back when the text is changing ( and the cursor too ) showedPasword shouldnt be used for this, but it works and adding another bool would start being messy
                    showedPasword = false;
                    ui->passwordTextEdit->setText(global::keyboard::keyboardText);
                    ui->showPasswordBtn->setIcon(QIcon("://resources/hide.png"));
                    ui->showPasswordBtn->show();
                    showedPasword = true;
                    savedPassword = global::keyboard::keyboardText;
                }
                global::keyboard::keyboardText = "";
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
    if(showedPasword == false) {
        ui->showPasswordBtn->setIcon(QIcon("://resources/hide.png"));
        ui->passwordTextEdit->setText(savedPassword);
        showedPasword = true;
    }
    else {
        showedPasword = false;
        ui->showPasswordBtn->setIcon(QIcon("://resources/show.png"));
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
        finalPassword = savedPassword;
        writeToDatabase(connectedNetworkData.name, savedPassword);
    }

    ui->CancelBtn->setEnabled(false);
    if(checkWifiState() == global::wifi::WifiState::Configured) {
        string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
    }
    string_writeconfig("/run/wifi_network_essid", connectedNetworkData.name.toStdString());
    string_writeconfig("/run/wifi_network_passphrase", finalPassword.toStdString());
    setDefaultWorkDir();
    // this will be deleited later in mainwindow icon updater if it failed
    string_writeconfig(".config/17-wifi_connection_information/essid", connectedNetworkData.name.toStdString());
    string_writeconfig(".config/17-wifi_connection_information/passphrase", finalPassword.toStdString());
    finalConnectWait();

}

void connectiondialog::finalConnectWait() {
    if(checkIfWifiBussy() == true) {
        // To be sure
        if(waitTry == 10) {
            string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
        }
        // Max 10s to wait for everything to shut down
        if(waitTry == 20) {
            string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
            emit showToastSignal("Failed to stop other wifi processes");
            ui->CancelBtn->setEnabled(true);
        }
        else {
            QTimer::singleShot(500, this, SLOT(finalConnectWait()));
            waitTry = waitTry + 1;
        }
    }
    else {
        string_writeconfig("/opt/ibxd", "connect_to_wifi_network\n");
        ui->CancelBtn->setEnabled(true);
        this->deleteLater();
        this->close();
    }
}

bool connectiondialog::checkIfWifiBussy() {
    if(checkProcessName("connect_to_network.sh") == true or
       checkProcessName("connection_manager.sh") == true or
       checkProcessName("only_connect_to_network.sh") == true) {
        return true;
    }
    else {
        return false;
    }
}
