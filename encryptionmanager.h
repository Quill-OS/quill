#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

#include <QWidget>
#include "generaldialog.h"
#include "toast.h"

namespace Ui {
class encryptionManager;
}

class encryptionManager : public QWidget
{
    Q_OBJECT

public:
    explicit encryptionManager(QWidget *parent = nullptr);
    ~encryptionManager();

private slots:
    void on_setupContinueBtn_clicked();
    void on_setupAbortBtn_clicked();
    void quit_restart();
    void refreshScreen();
    void showToast(QString messageToDisplay);
    void setupEncryptedStorage();
    void mkEncfsDirs();

private:
    Ui::encryptionManager *ui;
    generalDialog * generalDialogWindow;
    toast * toastWindow;
};

#endif // ENCRYPTIONMANAGER_H
