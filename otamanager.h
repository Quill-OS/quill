#ifndef OTAMANAGER_H
#define OTAMANAGER_H

#include <QWidget>

namespace Ui {
class otaManager;
}

class otaManager : public QWidget
{
    Q_OBJECT

public:
    explicit otaManager(QWidget *parent = nullptr);
    ~otaManager();

private:
    Ui::otaManager *ui;

signals:
    void canInstallOtaUpdate(bool yesno);

};

#endif // OTAMANAGER_H
