#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>

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
    bool lowBatteryDialog = false;

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

    void on_acceptBtn_clicked();

private:
    Ui::generalDialog *ui;
};

#endif // GENERALDIALOG_H
