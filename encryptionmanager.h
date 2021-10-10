#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

#include <QWidget>
#include "generaldialog.h"
#include "toast.h"
#include "hourglassanimationwidget.h"

namespace Ui {
class encryptionManager;
}

class encryptionManager : public QWidget
{
    Q_OBJECT

public:
    explicit encryptionManager(QWidget *parent = nullptr);
    ~encryptionManager();
    bool setupExitWidgetRan = false;

private slots:
    void on_setupContinueBtn_clicked();
    void on_setupAbortBtn_clicked();
    void quit_restart();
    void refreshScreen();
    void showToast(QString messageToDisplay);
    void setupEncryptedStorage();
    void mkEncfsDirs();
    void on_exitSuccessBtn_clicked();
    void setupExitWidget(bool exitStatus);
    void on_failureContinueBtn_clicked();
    void setupPassphraseDialog(int mode);
    void unlockEncryptedStorage();

private:
    Ui::encryptionManager *ui;
    generalDialog * generalDialogWindow;
    toast * toastWindow;
    hourglassAnimationWidget * hourglassAnimationWidgetWindow;
};

#endif // ENCRYPTIONMANAGER_H
