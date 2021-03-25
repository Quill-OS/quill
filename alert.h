#ifndef ALERT_H
#define ALERT_H

#include <QWidget>
#include <iostream>
#include <fstream>

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

private slots:
    void on_continueBtn_clicked();

    void on_resetBtn_clicked();

private:
    Ui::alert *ui;
};

#endif // ALERT_H
