#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "apps.h"
#include "reader.h"
#include "quit.h"
#include "alert.h"
#include "usbms_splash.h"
#include "brightnessdialog.h"
#include "generaldialog.h"
#include <iostream>
#include <QFile>
#include <QTextStream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool checked_box = false;
    bool existing_recent_books = false;
    bool reboot_after_update = false;
    int batt_level_int;
    bool updateDialog = false;
    int timerTime = 0;
    QString checkconfig_str_val;
    QString relative_path;
    QString batt_level;
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
    };
    bool checkconfig_rw(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
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
    };
    void writeconfig(string file, string config) {
            ofstream fhandler;
            fhandler.open(file);
            fhandler << config << boolalpha << checked_box << endl;
            fhandler.close();
    }
    void get_battery_level() {
        QFile batt_level_file("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/capacity");
        batt_level_file.open(QIODevice::ReadOnly);
        batt_level = batt_level_file.readAll();
        batt_level = batt_level.trimmed();
        batt_level_int = batt_level.toInt();
        batt_level = batt_level.append("%");
        batt_level_file.close();
    }
    int brightness_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        int content_int = content.toInt();
        return content_int;
        config.close();
    };
    int get_brightness() {
        QFile brightness("/var/run/brightness");
        brightness.open(QIODevice::ReadOnly);
        QString valuestr = brightness.readAll();
        int value = valuestr.toInt();
        brightness.close();
        return value;
    }
    void set_brightness(int value) {
        ofstream fhandler;
        fhandler.open("/var/run/brightness");
        fhandler << value;
        fhandler.close();
    }
    void brightness_writeconfig(int value) {
        ofstream fhandler;
        fhandler.open(".config/03-brightness/config");
        fhandler << value;
        fhandler.close();
    }
    int int_checkconfig(QString file) {
        QFile int_config(file);
        int_config.open(QIODevice::ReadOnly);
        QString valuestr = int_config.readAll();
        int value = valuestr.toInt();
        int_config.close();
        return value;
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
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    int display_quote() {
        QDir::setCurrent(".config/05-quote");
        QString quote_prog ("sh");
        QStringList quote_args;
        quote_args << "quote.sh";
        QProcess *quote_proc = new QProcess();
        quote_proc->start(quote_prog, quote_args);
        quote_proc->waitForFinished();
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");

        int quote_value = int_checkconfig(".config/05-quote/quote");
        return quote_value;
    }
public slots:

private slots:
    void on_settingsBtn_clicked();
    void on_appsBtn_clicked();
    void on_pushButton_clicked();
    void on_searchBtn_clicked();
    void on_quitBtn_clicked();
    void on_book1Btn_clicked();
    void on_book2Btn_clicked();
    void on_book3Btn_clicked();
    void on_book4Btn_clicked();
    void on_brightnessBtn_clicked();
    void openUpdateDialog();

private:
    Ui::MainWindow *ui;
    settings *settingsWindow;
    apps *appsWindow;
    reader *readerWindow;
    quit *quitWindow;
    alert *alertWindow;
    usbms_splash *usbmsWindow;
    brightnessDialog *brightnessDialogWindow;
    generalDialog *generalDialogWindow;
};
#endif // MAINWINDOW_H
