#ifndef SLEEPDIALOG_H
#define SLEEPDIALOG_H

#include <QDialog>

namespace Ui {
class sleepDialog;
}

class sleepDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit sleepDialog(QDialog *parent = nullptr);
    ~sleepDialog();

public slots:
    void launchSleepDialog();
    void hideSleepDialog();


private:
    Ui::sleepDialog * ui;
};

#endif // SLEEPDIALOG_H
