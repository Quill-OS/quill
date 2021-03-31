#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <fstream>

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
    bool checked_box = false;
    bool launch_sh = false;
    bool not_user_change = true;
    QString checkconfig_str_val;

    explicit settings(QWidget *parent = nullptr);
    ~settings();
    bool checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        const QString content = in.readAll();
        string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
    }
    void writeconfig(string file, string config) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config << boolalpha << checked_box << endl;
        fhandler.close();
    }
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    void string_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void string_checkconfig_ro(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }

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

private:
    Ui::settings *ui;
    usbms_splash *usbmsWindow;
    generalDialog *generalDialogWindow;
};

#endif // SETTINGS_H
