#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

#include <usbms_splash.h>
#include "generaldialog.h"

using namespace std;

namespace Ui {
class settings;
}

class settings : public QWidget
{
    Q_OBJECT

public:
    int settings_page = 1;
    bool launch_sh = false;
    bool not_user_change = true;

    explicit settings(QWidget *parent = nullptr);
    ~settings();

private slots:
    void exitSlot();
    void on_demoCheckBox_toggled(bool);
    void on_aboutBtn_clicked();
    void on_okBtn_clicked();
    void on_clockCheckBox_toggled(bool checked);
    void on_quoteCheckBox_toggled(bool checked);
    void on_requestLeaseBtn_clicked();
    void on_usbmsBtn_clicked();
    void on_previousBtn_clicked();
    void on_nextBtn_clicked();
    void on_wordsNumber_valueChanged(int arg1);
    void on_updateBtn_clicked();
    void on_darkModeCheckBox_toggled(bool checked);
    void on_uiScalingSlider_valueChanged(int value);
    void on_menuBarCheckBox_toggled(bool checked);
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_resetBtn_clicked();
    void on_setPasscodeBtn_clicked();
    void on_enableLockscreenCheckBox_toggled(bool checked);

private:
    Ui::settings *ui;
    usbms_splash *usbmsWindow;
    generalDialog *generalDialogWindow;
};

#endif // SETTINGS_H
