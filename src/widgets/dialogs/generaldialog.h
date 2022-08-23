#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>

#include "usbmsSplash.h"
#include "textwidget.h"
#include "virtualkeyboard.h"
#include "virtualkeypad.h"
#include "dictionarywidget.h"
#include "otamanager.h"
#include "searchresultswidget.h"

using namespace std;

namespace Ui {
class generalDialog;
}

class generalDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit generalDialog(QWidget *parent = nullptr);
    ~generalDialog();
    bool resetDialog = false;
    bool updateDialog = false;
    bool settingsRebootDialog = false;
    bool koboxSettingsRebootDialog = false;
    bool lowBatteryDialog = false;
    bool usbmsDialog = false;
    bool textBrowserDialog = false;
    bool appCompatibilityDialog = false;
    bool appInfoDialog = false;
    bool resetKoboxDialog = false;
    bool keyboardDialog = false;
    bool keypadDialog = false;
    bool dictionaryResults = false;
    bool vncServerSet = false;
    bool vncPasswordSet = false;
    bool gutenbergSyncDone = false;
    bool gutenbergSyncStatus = false;
    bool noGutenbergSyncToDo = false;
    bool searchTimerDone = false;
    QString vncServerAddress;
    QString vncServerPassword;
    QString vncServerPort;
    QString wifiEssid;
    QString wifiPassphrase;
    void setupKeyboardDialog();
    void startVNC(QString server, QString password, QString port);
    float yIncrease;

public slots:
    void increaseSize();

private slots:
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_acceptBtn_clicked();
    void adjust_size();
    void restartSearchDialog();
    void refreshScreenNative();
    void startOtaUpdate(bool wasDownloadSuccessful);
    void openBookFileNative(QString book, bool relativePath);
    void showToastNative(QString messageToDisplay);
    void closeIndefiniteToastNative();
    void quit_restart();
    void syncGutenbergCatalog();
    void waitForGutenbergSearchDone();

private:
    Ui::generalDialog *ui;
    usbmsSplash *usbmsWindow;
    textwidget *textwidgetWindow;
    virtualkeyboard *keyboardWidget;
    virtualkeypad *keypadWidget;
    dictionaryWidget *dictionaryWidgetWindow;
    otaManager *otaManagerWindow;
    searchResultsWidget * searchResultsWidgetWindow;

signals:
    void gotoPageSelected(int value);
    void refreshScreen();
    void updateWifiIcon(int mode);
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();
    void openBookFile(QString book, bool relativePath);
    void cancelDisableStorageEncryption();
    void disableStorageEncryption();
};

#endif // GENERALDIALOG_H
