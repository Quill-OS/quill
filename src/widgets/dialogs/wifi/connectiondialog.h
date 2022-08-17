#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "functions.h"

namespace Ui {
class connectiondialog;
}

class connectiondialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit connectiondialog(QWidget *parent = nullptr);
    ~connectiondialog();
    global::wifi::wifiNetworkData connectedNetworkData;
    QString currentlyConnectedNetworkName;

public slots:
    void applyVariables();

private:
    Ui::connectiondialog *ui;
};

#endif // CONNECTIONDIALOG_H
