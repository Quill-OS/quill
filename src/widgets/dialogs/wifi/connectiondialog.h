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

public slots:
    void applyVariables();
    void showToastSlot(QString message);

private slots:
    void on_CancelBtn_clicked();
    QString searchDatabase(QString key);
    void writeToDatabase(QString name, QString password);


    void on_passwordTextEdit_selectionChanged();

    void on_passwordTextEdit_cursorPositionChanged(int arg1, int arg2);

private:
    Ui::connectiondialog *ui;
    bool cursorPositionIgnore = false;
};

#endif // CONNECTIONDIALOG_H
