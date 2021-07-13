#ifndef WIFIDIALOG_H
#define WIFIDIALOG_H

#include <QDialog>

namespace Ui {
class wifiDialog;
}

class wifiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit wifiDialog(QWidget *parent = nullptr);
    ~wifiDialog();

private:
    Ui::wifiDialog *ui;
};

#endif // WIFIDIALOG_H
