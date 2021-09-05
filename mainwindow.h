#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settings.h"
#include "settingschooser.h"
#include "apps.h"
#include "reader.h"
#include "quit.h"
#include "alert.h"
#include "usbms_splash.h"
#include "brightnessdialog.h"
#include "generaldialog.h"
#include "koboxsettings.h"
#include "koboxappsdialog.h"
#include "textwidget.h"
#include "virtualkeypad.h"
#include "toast.h"
#include "otamanager.h"

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

    float stdIconWidth;
    float stdIconHeight;
    float brightnessIconWidth;
    float brightnessIconHeight;
    float homeIconWidth;
    float homeIconHeight;
    float wifiIconWidth;
    float wifiIconHeight;
    float sW;
    float sH;

    bool existing_recent_books = false;
    bool reboot_after_update = false;
    int timerTime = 0;
    QString relative_path;
    QString usbmsStatus;

    void openLowBatteryDialog();
    void openCriticalBatteryAlertWindow();
    void openUsbmsDialog();
    void resetWindow(bool resetStackedWidget);
    void resetIcons();
    void setBatteryIcon();
    int testPing();
    bool checkWifiState();

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
    void openWifiDialog();
    void setInitialBrightness();
    void on_homeBtn_clicked();
    void refreshScreen();
    void setupSearchDialog();
    void setWifiIcon();
    void on_wifiBtn_clicked();
    void updateWifiIcon(int mode);
    void hello(int testNumber);
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();
    void openUpdateDialogOTA(bool open);
    void launchOtaUpdater();
    void openBookFile(QString book, bool relativePath);
    void openReaderFramework();

private:
    Ui::MainWindow *ui;
    settingsChooser *settingsChooserWindow;
    apps *appsWindow;
    reader *readerWindow;
    quit *quitWindow;
    alert *alertWindow;
    usbms_splash *usbmsWindow;
    brightnessDialog *brightnessDialogWindow;
    generalDialog *generalDialogWindow;
    koboxSettings *koboxSettingsWindow;
    koboxAppsDialog *koboxAppsDialogWindow;
    textwidget *textwidgetWindow;
    virtualkeypad *keypadWidget;
    toast *toastWindow;
    otaManager *otaManagerWindow;
};

#endif // MAINWINDOW_H
