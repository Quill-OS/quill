#ifndef WIFIDIALOG_H
#define WIFIDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "generaldialog.h"

namespace Ui {
class wifiDialog;
}

class wifiDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit wifiDialog(QWidget *parent = nullptr);
    ~wifiDialog();
    QString wifiNetworksList;
    QString itemText;
    QModelIndex index;
    void checkWifiNetworks();
    void printWifiNetworks();
    void centerDialog();

private:
    Ui::wifiDialog *ui;
    QTimer * wifiListTimer;
    generalDialog * generalDialogWindow;

signals:
    void wifiNetworksListReady(int networksFound);
    void quit(int exitCode);
    void refreshScreen();
    void updateWifiIconSig(int mode);
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();

private slots:
    void on_cancelBtn_clicked();
    void on_connectBtn_clicked();
    void refreshScreenNative();
    void updateWifiIcon(int mode);
    void showToastNative(QString messageToDisplay);
    void closeIndefiniteToastNative();
};

#endif // WIFIDIALOG_H
