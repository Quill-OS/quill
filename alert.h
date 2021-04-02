#ifndef ALERT_H
#define ALERT_H

#include <QWidget>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>

namespace Ui {
class alert;
}

using namespace std;

class alert : public QWidget
{
    Q_OBJECT

public:
    explicit alert(QWidget *parent = nullptr);
    ~alert();
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
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

private slots:
    void on_continueBtn_clicked();

    void on_resetBtn_clicked();

    void on_continue2Btn_clicked();

private:
    Ui::alert *ui;
};

#endif // ALERT_H
