#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "functions.h"

namespace Ui {
class connectiondialog;
}

class connectiondialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit connectiondialog(QWidget *parent = nullptr);
    ~connectiondialog();
    global::wifi::wifiNetworkData connectedNetworkData;
    QString currentlyConnectedNetworkName;
    QFile passwordDatabase = QFile("/mnt/onboard/.adds/inkbox/.config/17-wifi_connection_information/passwords.json");

signals:
    void showToastSignal(QString message);
    void refreshScreenSignal();

public slots:
    void applyVariables();
    void showToastSlot(QString message);
    void refreshScreenSlot();

private slots:
    // I know im opening / loading json many times, its maybe not efficient but:
    // 1. Its modular
    // 2. Those operations are rare
    QString searchDatabase(QString key);
    void writeToDatabase(QString name, QString password);
    void removeFromDatabase(QString name);


    void finalConnectWait();
    bool checkIfWifiBussy();

    void on_CancelBtn_clicked();

    void on_passwordTextEdit_selectionChanged();

    void on_passwordTextEdit_cursorPositionChanged(int arg1, int arg2);

    void on_showPasswordBtn_clicked();

    void on_connectBtn_clicked();

private:
    Ui::connectiondialog *ui;
    bool cursorPositionIgnore = false;
    bool showedPassword;
    QString savedPassword;
    int waitTry = 0;
    QString passwordForReconnecting;
};

#endif // CONNECTIONDIALOG_H
