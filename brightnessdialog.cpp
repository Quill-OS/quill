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
    brightnessDialog::setFont(QFont("u001"));

    // Stylesheet, style & misc.
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

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
    ui->warmthValueLabel->setStyleSheet("font-size: 9pt");

    if(global::isN705 == true or global::isN905C == true or global::isN613 == true) {
        ui->warmthSlider->hide();
        ui->warmthDecBtn->hide();
        ui->warmthIncBtn->hide();
        ui->warmthValueLabel->hide();
        ui->warmthSlider->deleteLater();
        ui->warmthDecBtn->deleteLater();
        ui->warmthIncBtn->deleteLater();
        ui->gridLayout_5->deleteLater();
        ui->warmthValueLabel->deleteLater();
        this->adjustSize();
    }
    else if (global::isN873 == true){
        ui->warmthDecBtn->setProperty("type", "borderless");
        ui->warmthIncBtn->setProperty("type", "borderless");
        ui->warmthDecBtn->setText("");
        ui->warmthIncBtn->setText("");
        ui->warmthDecBtn->setIcon(QIcon(":/resources/nightmode-empty.png"));
        ui->warmthIncBtn->setIcon(QIcon(":/resources/nightmode-full.png"));
    }
    else {
        ui->warmthSlider->hide();
        ui->warmthDecBtn->hide();
        ui->warmthIncBtn->hide();
        ui->warmthValueLabel->hide();
        ui->warmthSlider->deleteLater();
        ui->warmthDecBtn->deleteLater();
        ui->warmthIncBtn->deleteLater();
        ui->gridLayout_5->deleteLater();
        ui->warmthValueLabel->deleteLater();
        this->adjustSize();
    }

    // I know, Mini and Touch don't have frontlights but that's a template to include others later...
    int value;
    int warmthValue;
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        value = get_brightness();
        if(global::isN873 == true) {
            warmthValue = get_warmth();
            ui->warmthSlider->setValue(warmthValue);
        }
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
    // Warmth value label
    if(global::isN873 == true) {
        warmthValue = get_warmth();
        QString warmthValueStr = QString::number(warmthValue);
        ui->warmthValueLabel->setText(warmthValueStr);
    }

    // UI fonts
    int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Bold.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont crimson_bold(family);

    ui->brightnessLabel->setFont(QFont(crimson_bold));

    // Saving current brightness value in case we want to go backwards
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        oldValue = get_brightness();
        if(global::isN873 == true) {
            oldWarmthValue = get_warmth();
        }
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
    brightnessDialog::pre_set_brightness(oldValue);
    if(global::isN873 == true) {
        set_warmth(oldWarmthValue);
    }

    // Just in case ;)
    brightness_writeconfig(oldValue);
    if(global::isN873 == true) {
        warmth_writeconfig(oldWarmthValue);
    }

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::on_horizontalSlider_valueChanged(int value)
{
    brightnessDialog::pre_set_brightness(value);
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
    int brightnessValue = ui->horizontalSlider->value();
    int warmthValue;

    // Write brightness config
    brightness_writeconfig(brightnessValue);
    if(global::isN873 == true) {
        warmthValue = ui->warmthSlider->value();
        warmth_writeconfig(warmthValue);
    }

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::pre_set_brightness(int brightnessValue) {
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        set_brightness(brightnessValue);
    }
    else if(global::isN613 == true) {
        set_brightness_ntxio(brightnessValue);
    }
    else {
        set_brightness(brightnessValue);
    }
}

void brightnessDialog::on_warmthSlider_valueChanged(int value)
{
    set_warmth(value);
    QString valueStr = QString::number(value);
    ui->warmthValueLabel->setText(valueStr);
}


void brightnessDialog::on_warmthDecBtn_clicked()
{
    int value = ui->warmthSlider->value();
    value = value - 1;
    ui->warmthSlider->setValue(value);
}


void brightnessDialog::on_warmthIncBtn_clicked()
{
    int value = ui->warmthSlider->value();
    value = value + 1;
    ui->warmthSlider->setValue(value);
}

