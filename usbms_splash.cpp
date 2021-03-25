#include "usbms_splash.h"
#include "ui_usbms_splash.h"

#include <QPixmap>

usbms_splash::usbms_splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usbms_splash)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color:black;");
    ui->label->setStyleSheet("QLabel { background-color : black; color : white; }");
    ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 9pt}");

    QPixmap pixmap(":/resources/usbms.png");
    ui->label_2->setPixmap(pixmap);
}

usbms_splash::~usbms_splash()
{
    delete ui;
}
