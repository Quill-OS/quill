#ifndef KOBOXSETTINGS_H
#define KOBOXSETTINGS_H

#include <QWidget>

#include "generaldialog.h"

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

private slots:
    void on_okBtn_clicked();
    void on_checkBox_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::koboxSettings *ui;
    generalDialog *generalDialogWindow;
};

#endif // KOBOXSETTINGS_H
