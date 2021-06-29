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
    explicit usbms_splash(QWidget *parent = nullptr);
    ~usbms_splash();

    void usbms_launch();

private slots:
    void brightnessDown();

private:
    Ui::usbms_splash *ui;
};

#endif // USBMS_SPLASH_H
