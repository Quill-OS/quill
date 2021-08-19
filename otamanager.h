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
    void canOtaUpdate(bool yesno);
    void downloadedOta(bool wasDownloadSuccessful);

};

#endif // OTAMANAGER_H
