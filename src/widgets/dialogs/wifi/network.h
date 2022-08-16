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
    explicit network(QWidget *parent = nullptr);
    ~network();
    global::wifi::wifiNetworkData mainData;
    QString currentlyConnectedNetwork;

public slots:
    void applyVariables();

private:
    Ui::network *ui;
};

#endif // NETWORK_H
