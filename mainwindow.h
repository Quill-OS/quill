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
    bool existing_recent_books = false;
    bool reboot_after_update = false;
    int timerTime = 0;
    QString relative_path;
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
    void openLowBatteryDialog();

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
