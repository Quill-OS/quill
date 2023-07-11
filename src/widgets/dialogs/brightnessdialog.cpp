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
    ui->brightnessLabel->setStyleSheet("padding-left: 125px; padding-right: 125px; font:bold");
    ui->valueLabel->setStyleSheet("font-size: 9pt");
    ui->warmthValueLabel->setStyleSheet("font-size: 9pt");

    if(global::isN705 == true or global::isN905C == true or global::isN613 == true or global::isKT == true) {
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
    if(global::isN705 == true or global::isN905C == true or global::isKT == true or global::isN873 == true) {
        value = getBrightness();
        if(global::isN873 == true) {
            warmthValue = getWarmth();
            ui->warmthSlider->setValue(warmthValue);
        }
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        value = brightnessCheckconfig(".config/03-brightness/config");
    }
    else {
        value = getBrightness();
    }

    // Setting the slider to the appropriate position
    ui->horizontalSlider->setValue(value);

    // Showing brightness percentage on label
    QString valueStr = QString::number(value);
    valueStr = valueStr.append("%");
    ui->valueLabel->setText(valueStr);
    // Warmth value label
    if(global::isN873 == true) {
        warmthValue = getWarmth();
        QString warmthValueStr = QString::number(warmthValue);
        ui->warmthValueLabel->setText(warmthValueStr);
    }

    // Saving current brightness value in case we want to go backwards
    if(global::isN705 == true or global::isN905C == true or global::isKT == true or global::isN873 == true) {
        oldValue = getBrightness();
        if(global::isN873 == true) {
            oldWarmthValue = getWarmth();
        }
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        oldValue = brightnessCheckconfig(".config/03-brightness/config");
    }
    else {
        oldValue = getBrightness();
    }
}

brightnessDialog::~brightnessDialog()
{
    delete ui;
}

void brightnessDialog::on_quitBtn_clicked()
{
    // Reverting back to the old value
    brightnessDialog::preSetBrightness(oldValue);
    if(global::isN873 == true) {
        setWarmth(oldWarmthValue);
    }

    // Just in case ;)
    brightnessWriteconfig(oldValue);
    if(global::isN873 == true) {
        warmthWriteconfig(oldWarmthValue);
    }

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::on_horizontalSlider_valueChanged(int value)
{
    brightnessDialog::preSetBrightness(value);
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
    log("Display brightness set to " + QString::number(brightnessValue), className);
    brightnessWriteconfig(brightnessValue);
    if(global::isN873 == true) {
        warmthValue = ui->warmthSlider->value();
        log("Display warmth set to " + QString::number(warmthValue), className);
        warmthWriteconfig(warmthValue);
    }

    // Leaving
    brightnessDialog::close();
}

void brightnessDialog::preSetBrightness(int brightnessValue) {
    if(global::isN705 == true or global::isN905C == true or global::isKT == true or global::isN873 == true) {
        setBrightness(brightnessValue);
    }
    else if(global::isN613 == true) {
        setBrightness_ntxio(brightnessValue);
    }
    else {
        setBrightness(brightnessValue);
    }
}

void brightnessDialog::on_warmthSlider_valueChanged(int value)
{
    setWarmth(value);
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

