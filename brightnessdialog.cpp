#include "brightnessdialog.h"
#include "ui_brightnessdialog.h"
#include "functions.h"

#include <QFont>
#include <QFontDatabase>
#include <QFile>
#include <QDebug>

brightnessDialog::brightnessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::brightnessDialog)
{
    ui->setupUi(this);

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // I know, Mini and Touch don't have frontlights but that's a template to include others later...
    int value;
    if(global::isN705 == true or global::isN905C == true) {
        value = get_brightness();
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        value = brightness_checkconfig(".config/03-brightness/config");
    }
    else {
        value = get_brightness();
    }

    // Setting the slider to the appropriate position
    ui->horizontalSlider->setValue(value);

    // Showing brightness percentage on label
    QString valueStr = QString::number(value);
    valueStr = valueStr.append("%");
    ui->valueLabel->setText(valueStr);

    ui->quitBtn->setProperty("type", "borderless");
    ui->quitBtn->setText("");
    ui->quitBtn->setIcon(QIcon(":/resources/close.png"));
    ui->okBtn->setProperty("type", "borderless");
    ui->okBtn->setText("");
    ui->okBtn->setIcon(QIcon(":/resources/check.png"));
    ui->decBtn->setProperty("type", "borderless");
    ui->decBtn->setText("");
    ui->decBtn->setIcon(QIcon(":/resources/minus.png"));
    ui->incBtn->setProperty("type", "borderless");
    ui->incBtn->setText("");
    ui->incBtn->setIcon(QIcon(":/resources/plus.png"));
    ui->brightnessLabel->setStyleSheet("font-size: 11pt; padding-left: 125px; padding-right: 125px; font:bold");
    ui->valueLabel->setStyleSheet("font-size: 9pt");

    // UI fonts
    int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Bold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont crimson_bold(family);

    ui->brightnessLabel->setFont(QFont(crimson_bold));

    // Saving current brightness value in case we want to go backwards
    if(global::isN705 == true or global::isN705 == true) {
        oldValue = get_brightness();
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        oldValue = brightness_checkconfig(".config/03-brightness/config");
    }
    else {
        oldValue = get_brightness();
    }
}

brightnessDialog::~brightnessDialog()
{
    delete ui;
}

void brightnessDialog::on_quitBtn_clicked()
{
    // Reverting back to the old value
    pre_set_brightness(oldValue);

    // Just in case ;)
    brightness_writeconfig(oldValue);

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::on_horizontalSlider_valueChanged(int value)
{
    pre_set_brightness(value);
    QString valueStr = QString::number(value);
    valueStr = valueStr.append("%");
    ui->valueLabel->setText(valueStr);
}

void brightnessDialog::on_incBtn_clicked()
{
    int value = ui->horizontalSlider->value();
    value = value + 1;
    ui->horizontalSlider->setValue(value);
}

void brightnessDialog::on_decBtn_clicked()
{
    int value = ui->horizontalSlider->value();
    value = value - 1;
    ui->horizontalSlider->setValue(value);
}

void brightnessDialog::on_okBtn_clicked()
{
    // Get set brightness value
    int value = ui->horizontalSlider->value();

    // Write brightness config
    brightness_writeconfig(value);

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::pre_set_brightness(int brightnessValue) {
    if(global::isN705 == true or global::isN905C == true) {
        set_brightness(brightnessValue);
    }
    else if(global::isN613 == true) {
        set_brightness_ntxio(brightnessValue);
    }
    else {
        set_brightness(brightnessValue);
    }
}
