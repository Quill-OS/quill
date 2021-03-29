#include "usbms_splash.h"
#include "ui_usbms_splash.h"

#include <QPixmap>
#include <QScreen>

usbms_splash::usbms_splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usbms_splash)
{
    ui->setupUi(this);

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the default icon size will be

    float stdIconWidth = sW / 1.15;
    float stdIconHeight = sH / 1.15;

    this->setStyleSheet("background-color:black;");
    ui->label->setStyleSheet("QLabel { background-color : black; color : white; }");
    ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 9pt}");

    QPixmap pixmap(":/resources/usbms.png");
    QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
    ui->label_2->setPixmap(scaledPixmap);
}

usbms_splash::~usbms_splash()
{
    delete ui;
}
