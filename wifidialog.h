#ifndef WIFIDIALOG_H
#define WIFIDIALOG_H

#include <QDialog>

namespace Ui {
class wifiDialog;
}

class wifiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit wifiDialog(QWidget *parent = nullptr);
    ~wifiDialog();
    QString wifiNetworksList;
    void checkWifiNetworks();
    void printWifiNetworks();
    void centerDialog();

private:
    Ui::wifiDialog *ui;
    QTimer * wifiListTimer;

signals:
    void wifiNetworksListReady(int networksFound);
private slots:
    void on_cancelBtn_clicked();
};

#endif // WIFIDIALOG_H
