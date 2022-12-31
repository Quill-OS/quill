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

void audiofile::provideData(global::audio::musicFile fileProvided) {
    file = fileProvided;
    QString name = file.name;
    if(name > 30) {
        name.chop(name.length() - 30);
    }
    ui->nameLabel->setText(name);
    ui->timeLabel->setText(file.length);
}

void audiofile::die() {
    this->deleteLater();
    this->close();
}

void audiofile::on_addBtn_clicked()
{
    log("Adding item to queue", className);
    global::audio::queue.append(file);
    if(global::audio::isSomethingCurrentlyPlaying == false) {
        log("And also playing it because nothing else is playing");
        global::audio::isSomethingCurrentlyPlaying = true;
        emit playFileChild(global::audio::queue.length() - 1);
    }
}
