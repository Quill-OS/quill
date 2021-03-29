#ifndef USBMS_SPLASH_H
#define USBMS_SPLASH_H

#include <QWidget>
#include <QFile>
#include <QTextStream>

namespace Ui {
class usbms_splash;
}

class usbms_splash : public QWidget
{
    Q_OBJECT

public:
    explicit usbms_splash(QWidget *parent = nullptr);
    ~usbms_splash();

private:
    Ui::usbms_splash *ui;
};

#endif // USBMS_SPLASH_H
