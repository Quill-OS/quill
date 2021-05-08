#include "usbms_splash.h"
#include "ui_usbms_splash.h"

#include <QPixmap>
#include <QScreen>

#include "functions.h"

usbms_splash::usbms_splash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::usbms_splash)
{
    ui->setupUi(this);

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    if(global::kobox::showKoboxSplash == true) {
        float stdIconWidth = sW / 1.30;
        float stdIconHeight = sH / 1.30;

        // Stylesheet
        QFile stylesheetFile(":/resources/eink.qss");
        stylesheetFile.open(QFile::ReadOnly);
        this->setStyleSheet(stylesheetFile.readAll());
        stylesheetFile.close();

        ui->label->setText("Launching KoBox subsystem");
        ui->label->setStyleSheet("font-size: 14pt");
        ui->label_3->setText("Please wait, this could take a while.");
        ui->label_3->setStyleSheet("font-size: 10pt");

        QPixmap pixmap(":/resources/kobox-icon.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->label_2->setPixmap(scaledPixmap);
    }
    else {
        float stdIconWidth = sW / 1.15;
        float stdIconHeight = sH / 1.15;

        this->setStyleSheet("background-color:black;");
        ui->label->setStyleSheet("QLabel { background-color : black; color : white; }");
        ui->label_3->setStyleSheet("QLabel { background-color : black; color : white; font-size: 9pt}");

        QPixmap pixmap(":/resources/usbms.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->label_2->setPixmap(scaledPixmap);
    }
}

usbms_splash::~usbms_splash()
{
    delete ui;
}
