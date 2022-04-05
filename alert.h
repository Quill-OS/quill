#ifndef ALERT_H
#define ALERT_H

#include <QWidget>

namespace Ui {
class alert;
}

using namespace std;

class alert : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit alert(QWidget *parent = nullptr);
    ~alert();
    bool signatureError = false;
    bool downgradeError = false;
    bool criticalBattery = false;
    void updateReset();

private slots:
    void on_continueBtn_clicked();
    void on_resetBtn_clicked();
    void on_continue2Btn_clicked();

private:
    Ui::alert *ui;
};

#endif // ALERT_H
