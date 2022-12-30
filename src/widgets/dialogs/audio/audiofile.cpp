#include "audiofile.h"
#include "ui_audiofile.h"
#include "functions.h"

audiofile::audiofile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::audiofile)
{
    ui->setupUi(this);
}

audiofile::~audiofile()
{
    delete ui;
}

void audiofile::provideData(global::audio::musicFile file) {
    ui->nameLabel->setText(file.name);
    ui->timeLabel->setText(file.length);
}

void audiofile::die() {
    this->deleteLater();
    this->close();
}
