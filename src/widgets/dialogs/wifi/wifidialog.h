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

signals:
    void refreshScreen();
    void updateWifiIconSig(int mode);
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();

private slots:
    void refreshScreenNative();
    void updateWifiIcon(int mode);
    void showToastNative(QString messageToDisplay);
    void closeIndefiniteToastNative();
};

#endif // WIFIDIALOG_H
