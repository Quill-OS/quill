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
    explicit toast(QWidget *parent = nullptr);
    ~toast();
    void centerToast();

private:
    Ui::toast *ui;
    wifiDialog *wifiDialogWindow;

private slots:
    void showWifiDialog(int networksFound);
    void exitSlot();

signals:
    void updateWifiIconSig(int mode);
};

#endif // TOAST_H
