#ifndef USBMS_SPLASH_H
#define USBMS_SPLASH_H

#include <QWidget>

namespace Ui {
class usbms_splash;
}

class usbms_splash : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit usbms_splash(QWidget *parent = nullptr);
    ~usbms_splash();
    float sW;
    float sH;

    void usbms_launch();

private slots:
    void brightnessDown();
    void quit_restart();
    void restartServices();

private:
    Ui::usbms_splash *ui;
};

#endif // USBMS_SPLASH_H
