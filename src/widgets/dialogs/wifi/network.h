#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>

#include "functions.h"

namespace Ui {
class network;
}

class network : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit network(QWidget *parent = nullptr);
    ~network();
    global::wifi::wifiNetworkData mainData;
    QString currentlyConnectedNetwork;

signals:
    void showToastSignal(QString message);

public slots:
    void applyVariables();
    void closeWrapper();
    void showToastSlot(QString message);

private slots:
    void on_enterButton_clicked();

private:
    Ui::network *ui;
};

#endif // NETWORK_H
