#ifndef WIFILOGGER_H
#define WIFILOGGER_H

#include <QDialog>
#include "functions.h"

namespace Ui {
class wifilogger;
}

class wifilogger : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit wifilogger(QWidget *parent = nullptr);
    ~wifilogger();
    global::wifi::wifiNetworkData connectedNetworkData;

private:
    Ui::wifilogger *ui;
    /*
     * 0 is wifi info / no wifi info page
     * 1 is fancy logs page
     * 2 is all logs
    */
    int currentPage = 0;
    QTimer updateLogsTimer;
    QFile fancyLogs = QFile("/external_root/run/wifi_stats");
    QFile allLogs = QFile("/external_root/run/wifi_logs");

private slots:
    void setWifiInfoPage();
    void setFancyLoggingPage();
    void setAllLogsPage();
    void on_nextBtn_clicked();
    void on_previousBtn_clicked();
    void changePage();
    void getWifiInformations();
    void on_returnBtn_clicked();
    void updateLogs();
};

#endif // WIFILOGGER_H
