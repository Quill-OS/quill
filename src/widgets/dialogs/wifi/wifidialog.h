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

private:
    Ui::wifiDialog *ui;

public slots:
    bool launchRefresh();
    void refreshNetworksList();

signals:
    void refreshScreen();
    void updateWifiIconSig(int mode);
    void showToast(QString messageToDisplay);

    void killNetworkWidgets();

private slots:
    void on_refreshBtn_clicked();
};

#endif // WIFIDIALOG_H
