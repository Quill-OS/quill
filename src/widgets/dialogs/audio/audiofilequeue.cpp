#include "audiofilequeue.h"
#include "ui_audiofilequeue.h"

audiofilequeue::audiofilequeue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::audiofilequeue)
{
    ui->setupUi(this);
}

audiofilequeue::~audiofilequeue()
{
    delete ui;
}

void audiofilequeue::provideData(global::audio::musicFile fileProvided) {
    file = fileProvided;
    QString name = file.name;
    if(name > 20) {
        name.chop(name.length() - 20);
    }
    ui->nameLabel->setText(name);
    ui->timeLabel->setText(file.length);
}

void audiofilequeue::die() {
    this->deleteLater();
    this->close();
}
