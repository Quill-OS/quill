#include "audiofile.h"
#include "ui_audiofile.h"
#include "functions.h"

audiofile::audiofile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::audiofile)
{
    ui->setupUi(this);
    ui->nameLabel->setWordWrap(true);
}

audiofile::~audiofile()
{
    delete ui;
}

void audiofile::provideData(global::audio::musicFile fileProvided) {
    file = fileProvided;
    ui->nameLabel->setText(file.name);
    ui->timeLabel->setText(file.length);
}

void audiofile::die() {
    this->deleteLater();
    this->close();
}

void audiofile::on_addBtn_clicked()
{
    ui->addBtn->setDisabled(true);
    log("Adding item (song) to queue", className);
    global::audio::audioMutex.lock();
    global::audio::queue.append(file);
    QTimer::singleShot(550, this, SLOT(enableButton()));
    if(global::audio::isSomethingCurrentlyPlaying == false) {
        log("And also playing it because nothing else is playing", className);
        global::audio::isSomethingCurrentlyPlaying = true;
        int tmpInt = global::audio::queue.length() - 1;
        global::audio::audioMutex.unlock();
        emit playFileChild(tmpInt);
        return void();
    }
    global::audio::audioMutex.unlock();
}

void audiofile::enableButton() {
    log("Enabling back the button", className);
    ui->addBtn->setEnabled(true);
    ui->addBtn->repaint();
}
