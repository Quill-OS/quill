#ifndef TOAST_H
#define TOAST_H

#include <QDialog>

#include "wifidialog.h"

namespace Ui {
class toast;
}

class toast : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit toast(QWidget *parent = nullptr);
    ~toast();
    void centerToast();

private:
    Ui::toast *ui;
    wifiDialog *wifiDialogWindow;

private slots:
    void showWifiDialog(int networksFound);
    void exitSlot(int exitCode);
    void refreshScreenNative();
    void updateWifiIcon(int mode);
    void showToastNative(QString messageToDisplay);
    void closeIndefiniteToastNative();

signals:
    void updateWifiIconSig(int mode);
    void refreshScreen();
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();
};

#endif // TOAST_H
