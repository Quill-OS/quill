#ifndef KOBOXSETTINGS_H
#define KOBOXSETTINGS_H

#include <QWidget>

#include "generaldialog.h"
#include "usbms_splash.h"

namespace Ui {
class koboxSettings;
}

class koboxSettings : public QWidget
{
    Q_OBJECT

public:
    explicit koboxSettings(QWidget *parent = nullptr);
    ~koboxSettings();

    bool not_user_change = false;
    void openSettingsRebootDialog();
    void openResetKoboxDialog();

private slots:
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);
    void on_spinBox_valueChanged(int arg1);
    void on_pushButton_clicked();
    void on_resetKoboxBtn_clicked();

private:
    Ui::koboxSettings *ui;
    generalDialog *generalDialogWindow;
    usbms_splash *usbmsWindow;
};

#endif // KOBOXSETTINGS_H
