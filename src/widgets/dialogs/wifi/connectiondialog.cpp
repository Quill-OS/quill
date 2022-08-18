#include <QFile>
#include <QScreen>

#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include "generaldialog.h"

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
                ui->passwordTextEdit->setText(password);
            }
            else {
                log("No password found", className);
                ui->passwordTextEdit->setText("No password found");
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

void connectiondialog::on_CancelBtn_clicked()
{
    this->deleteLater();
    this->close();
}

void connectiondialog::on_passwordTextEdit_selectionChanged()
{
    ui->passwordTextEdit->setSelection(0, 0);
}

void connectiondialog::on_passwordTextEdit_cursorPositionChanged(int arg1, int arg2)
{
    log("detected click on textedit");
    if(cursorPositionIgnore == true) {
        global::keyboard::keyboardDialog = true;
        global::keyboard::wifiPassphraseDialog = true;
        global::keyboard::keyboardText = "";
        generalDialog* generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        generalDialogWindow->wifiEssid = connectedNetworkData.name;
        connect(generalDialogWindow, &generalDialog::showToast, this, &connectiondialog::showToastSlot);
        generalDialogWindow->exec();

        global::keyboard::keyboardDialog = false;
        global::keyboard::wifiPassphraseDialog = false;
        if(global::keyboard::keyboardText.isEmpty() == false) {
            ui->passwordTextEdit->setText(global::keyboard::keyboardText);
        }
        global::keyboard::keyboardText = "";

    }
    else {
        cursorPositionIgnore = true;
    }
    ui->passwordTextEdit->setCursorPosition(0);
}

void connectiondialog::showToastSlot(QString message) {
    emit showToastSignal(message);
}
