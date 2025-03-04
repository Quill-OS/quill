#ifndef USBMS_SPLASH_H
#define USBMS_SPLASH_H

#include <QWidget>

namespace Ui {
class usbmsSplash;
}

class usbmsSplash : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit usbmsSplash(QWidget *parent = nullptr);
    ~usbmsSplash();
    float sW;
    float sH;
    QString massStorageModule;

    void usbmsLaunch();

private slots:
    void brightnessAndWarmthDown();
    void quit_restart();
    void restartServices();

private:
    Ui::usbmsSplash *ui;
};

#endif // USBMS_SPLASH_H
