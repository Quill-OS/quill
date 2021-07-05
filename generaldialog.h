#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>

#include "usbms_splash.h"
#include "textwidget.h"
#include "virtualkeypad.h"

using namespace std;

namespace Ui {
class generalDialog;
}

class generalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit generalDialog(QWidget *parent = nullptr);
    ~generalDialog();
    bool resetDialog = false;
    bool updateDialog = false;
    bool settingsRebootDialog = false;
    bool koboxSettingsRebootDialog = false;
    bool lowBatteryDialog = false;
    bool usbmsDialog = false;
    bool textBrowserDialog = false;
    bool resetKoboxDialog = false;
    bool keyboardDialog = false;
    bool keypadDialog = false;

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_acceptBtn_clicked();

private:
    Ui::generalDialog *ui;
    usbms_splash *usbmsWindow;
    textwidget *textwidgetWindow;
    virtualkeypad *keypadWidget;

signals:
    void gotoPageSelected(int value);
};

#endif // GENERALDIALOG_H
