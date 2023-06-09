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
    ui->passphraseTextEdit->setFont(QFont("Roboto Mono"));
    ui->label->setFont(QFont("Inter"));
    ui->label_2->setFont(QFont("Inter"));

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->cancelBtn->setProperty("type", "borderless");
    ui->connectBtn->setProperty("type", "borderless");
    ui->showPassphraseBtn->setProperty("type", "borderless");

    ui->label->setStyleSheet("font-weight: bold");
    ui->label_2->setStyleSheet("font-weight: bold");

    ui->cancelBtn->setIcon(QIcon(":/resources/close.png"));
    ui->connectBtn->setIcon(QIcon(":/resources/arrow-right.png"));

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    if(global::deviceID == "n705\n") {
        this->setFixedWidth(screenGeometry.width() / 1.2);
    }
    else {
        this->setFixedWidth(screenGeometry.width() / 1.5);
    }

    int halfOfHalfHeight = ((screenGeometry.height() / 2) / 2) / 2;
    int finalHeight;
    if(global::deviceID == "n705\n") {
        finalHeight = screenGeometry.height() - halfOfHalfHeight * 5.9;
    }
    else if(global::deviceID == "n873\n") {
        finalHeight = screenGeometry.height() - halfOfHalfHeight * 6.45;
    }
    else {
        finalHeight = screenGeometry.height() - halfOfHalfHeight * 6.3;
    }

    this->setFixedHeight(finalHeight);

    // Centering dialog
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

connectiondialog::~connectiondialog()
{
    delete ui;
}

void connectiondialog::applyVariables() {
    // Limit name size
    int truncateThreshold;
    if(global::deviceID == "n705\n") {
        truncateThreshold = 20;
    }
    else {
        truncateThreshold = 30;
    }
    int nameLength = connectedNetworkData.name.length();
    QString name = connectedNetworkData.name;
    if(nameLength > truncateThreshold) {
        name.chop(nameLength - truncateThreshold);
        name.append("...");
    }
    ui->nameLabel->setText(name);
    ui->macLabel->setText(connectedNetworkData.mac);
    ui->signalLabel->setText(QString::number(connectedNetworkData.signal) + "%");

    if(connectedNetworkData.encryption == false) {
        ui->showPassphraseBtn->hide();
        ui->passphraseTextEdit->setText("No passphrase required");
        ui->passphraseTextEdit->setDisabled(true);
    }
    else {
        if(passphraseDatabase.exists() == false) {
            log("Creating empty database", className);
            // https://forum.qt.io/topic/104791/how-i-can-create-json-format-in-qt/5
            QJsonObject root;
            QJsonArray array;
            QJsonDocument newJsonDocument;
            root["list"] = array;
            newJsonDocument.setObject(root);

            passphraseDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            passphraseDatabase.write(newJsonDocument.toJson());
            passphraseDatabase.flush();
            passphraseDatabase.close();
        }
        QString passphrase = searchDatabase(connectedNetworkData.name);
        if(passphrase.isEmpty() == false) {
            log("Found passphrase: '" + passphrase + "'", className);
            ui->showPassphraseBtn->setIcon(QIcon(":/resources/show.png"));
            showedPassphrase = false;
            savedPassphrase = passphrase;

            int passphraseLength = passphrase.length();
            QString hiddenPassphrase;
            for(int i = 0; i < passphraseLength; i++) {
                hiddenPassphrase.append("•");
            }
            ui->passphraseTextEdit->setText(hiddenPassphrase);
        }
        else {
            log("No passphrase found", className);
            ui->passphraseTextEdit->setText("No passphrase was saved");
            showedPassphrase = true;
            ui->showPassphraseBtn->hide();
        }
    }
}

QString connectiondialog::searchDatabase(QString key) {
    passphraseDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passphraseDatabase.readAll();
    passphraseDatabase.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

    if(jsonDocument["list"].isArray() == true) {
        QJsonArray jsonArray = jsonDocument["list"].toArray();
        for(QJsonValueRef refJsonObject: jsonArray) {
            QJsonObject jsonMainObject = refJsonObject.toObject();
            QString searchedName = jsonMainObject.keys().first().toUtf8();
            log("Found in database: '" + searchedName + "'", className);
            if(searchedName == key) {
                QString returnedPassphrase = jsonMainObject.value(key).toString();
                log("Searched name '" + searchedName + "' matched '" + key + "' and the passphrase is: '" + returnedPassphrase + "'", className);
                return returnedPassphrase;
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

void connectiondialog::writeToDatabase(QString name, QString passphrase) {
    if(searchDatabase(name).isEmpty() == false) {
        removeFromDatabase(name);
    }

    passphraseDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passphraseDatabase.readAll();
    passphraseDatabase.close();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(fileRead.toUtf8());

    if(jsonDocument["list"].isArray() == true) {
        QJsonArray jsonArray = jsonDocument["list"].toArray();
        QJsonValue newValue;

        // https://stackoverflow.com/questions/26804660/how-to-initialize-qjsonobject-from-qstring
        // I hoped this would be easier
        QJsonObject newObject = QJsonDocument::fromJson(QString("{\"" + name + "\" : \"" + passphrase + "\" }").toUtf8()).object();
        jsonArray.append(newObject);

        QJsonDocument newJsonDocument;
        QJsonObject root;
        root["list"] = jsonArray;
        newJsonDocument.setObject(root);

        passphraseDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        passphraseDatabase.write(newJsonDocument.toJson());
        passphraseDatabase.flush();
        passphraseDatabase.close();
    }
    else {
        log("Something went horribly wrong", className);
    }
}

void connectiondialog::removeFromDatabase(QString name) {
    passphraseDatabase.open(QIODevice::ReadOnly | QIODevice::Text);
    QString fileRead = passphraseDatabase.readAll();
    passphraseDatabase.close();
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

            passphraseDatabase.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            passphraseDatabase.write(newJsonDocument.toJson());
            passphraseDatabase.flush();
            passphraseDatabase.close();
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

void connectiondialog::on_passphraseTextEdit_selectionChanged()
{
    ui->passphraseTextEdit->setSelection(0, 0);
}

void connectiondialog::on_passphraseTextEdit_cursorPositionChanged(int oldpos, int newpos)
{
    log("Detected click on text edit widget", className);
    if(cursorPositionIgnore == true) {
        if(newpos != 0) {
            if(showedPassphrase == true) {
                ui->passphraseTextEdit->setCursorPosition(0);
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
                    // A bit hacky: avoid summoning the keyboard back when the text is changing (and the cursor too) showedPassphrase shouldn't be used for this, but it works and adding another boolean would start being messy
                    showedPassphrase = false;
                    ui->passphraseTextEdit->setText(global::keyboard::keyboardText);
                    ui->showPassphraseBtn->setIcon(QIcon(":/resources/hide.png"));
                    ui->showPassphraseBtn->show();
                    showedPassphrase = true;
                    savedPassphrase = global::keyboard::keyboardText;
                    ui->showPassphraseBtn->show();
                }
                global::keyboard::keyboardText = "";
            }
            else {
                log("Passphrase is not saved; ignoring text edit widget call", className);
            }
        }
    }
    else {
        log("Ignoring click on text edit widget", className);
        cursorPositionIgnore = true;
    }
}

void connectiondialog::showToastSlot(QString message) {
    emit showToastSignal(message);
}

void connectiondialog::on_showPassphraseBtn_clicked()
{
    if(showedPassphrase == false) {
        ui->showPassphraseBtn->setIcon(QIcon(":/resources/hide.png"));
        ui->passphraseTextEdit->setText(savedPassphrase);
        showedPassphrase = true;
    }
    else {
        showedPassphrase = false;
        ui->showPassphraseBtn->setIcon(QIcon(":/resources/show.png"));

        int passphraseLength = searchDatabase(connectedNetworkData.name).length();
        if(passphraseLength != 0) {
            QString hiddenPassphrase;
            for(int i = 0; i < passphraseLength; i++) {
                hiddenPassphrase.append("•");
            }
            ui->passphraseTextEdit->setText(hiddenPassphrase);
        }
        else {
            // This is executed if the user asks to hide the passphrase but it isn't saved yet (upon first connection to a network)
            QString hiddenPassphrase;
            int passphraseLength = ui->passphraseTextEdit->text().length();
            for(int i = 0; i < passphraseLength; i++) {
                hiddenPassphrase.append("•");
            }
            ui->passphraseTextEdit->setText(hiddenPassphrase);
        }
    }
}

void connectiondialog::refreshScreenSlot() {
    this->repaint();
    emit refreshScreenSignal();
}

void connectiondialog::on_connectBtn_clicked()
{
    QString finalPassphrase;
    if(connectedNetworkData.encryption == false) {
        finalPassphrase = "NONE";
    }
    else {
        if(savedPassphrase.isEmpty() == false) {
            finalPassphrase = savedPassphrase;
            writeToDatabase(connectedNetworkData.name, savedPassphrase);
        }
        else {
            showToastSlot("Provide a passphrase first");
            return void();
        }
    }
    passphraseForReconnecting = finalPassphrase;

    ui->cancelBtn->setEnabled(false);
    if(checkWifiState() == global::wifi::wifiState::configured) {
        string_writeconfig("/opt/ibxd", "stop_wifi_operations\n");
    }
    writeFile("/run/wifi_network_essid", connectedNetworkData.name);
    writeFile("/run/wifi_network_passphrase", finalPassphrase);
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

        // This will be deleted later in MainWindow's icon updater if it failed. It is also deleted in the Wi-Fi stop script.
        log("Writing to configuration directory with connection information data", className);
        string_writeconfig("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/essid", connectedNetworkData.name.toStdString());
        string_writeconfig("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/passphrase", passphraseForReconnecting.toStdString());

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
