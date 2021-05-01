#ifndef KOBOXAPPSDIALOG_H
#define KOBOXAPPSDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "usbms_splash.h"

namespace Ui {
class koboxAppsDialog;
}

class koboxAppsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit koboxAppsDialog(QWidget *parent = nullptr);
    ~koboxAppsDialog();

    void checkApps();

    QString apps;
    QString itemText;
    std::string dpiSetting;
    std::string dpModeSetting;
    QModelIndex index;

private slots:
    void on_cancelBtn_clicked();
    void on_launchBtn_clicked();

private:
    Ui::koboxAppsDialog *ui;
    usbms_splash *usbmsSplashWindow;
};

#endif // KOBOXAPPSDIALOG_H
