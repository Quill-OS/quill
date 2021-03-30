#ifndef BRIGHTNESSDIALOG_H
#define BRIGHTNESSDIALOG_H

#include <QDialog>
#include <fstream>
#include <iostream>
#include <QTextStream>
#include <QFile>

using namespace std;

namespace Ui {
class brightnessDialog;
}

class brightnessDialog : public QDialog
{
    Q_OBJECT

public:
    QString checkconfig_str_val;
    int oldValue;
    explicit brightnessDialog(QWidget *parent = nullptr);
    ~brightnessDialog();
    void brightness_writeconfig(int value) {
        ofstream fhandler;
        fhandler.open(".config/03-brightness/config");
        fhandler << value;
        fhandler.close();
    }
    void string_checkconfig_ro(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
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

private slots:
    void on_quitBtn_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_incBtn_clicked();

    void on_decBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::brightnessDialog *ui;
};

#endif // BRIGHTNESSDIALOG_H
