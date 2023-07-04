#include "audiofilequeue.h"
#include "ui_audiofilequeue.h"

audiofilequeue::audiofilequeue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::audiofilequeue)
{
    ui->setupUi(this);
    ui->nameLabel->setWordWrap(true);
}

audiofilequeue::~audiofilequeue()
{
    delete ui;
}

void audiofilequeue::provideData(global::audio::musicFile fileProvided, bool grey) {
    file = fileProvided;
    ui->nameLabel->setText(file.name);
    ui->timeLabel->setText(file.length);
    if(grey == true) {
        log("Setting background grey...", className);
        ui->deleteBtn->setStyleSheet("background: grey;");
        ui->playButton->setStyleSheet("background: grey;");
        isPlaying = true;
    }
}

void audiofilequeue::die() {
    this->deleteLater();
    this->close();
}

void audiofilequeue::on_deleteBtn_clicked()
{
    // TODO: Doesn't work
    int id = -1;
    log("global::audio::queue.size(): " + QString::number(global::audio::queue.size()), className);
    global::audio::audioMutex.lock();

    for(int i = 0; i < global::audio::queue.size(); i++) {
        if(file.id == global::audio::queue[i].id) {
            log("Found id!", className);
            id = i;
            id = id + 1;
        }
    }
    log("File id is: " + QString::number(id), className);
    if(id == global::audio::queue.size() - 1) {
        global::audio::queue.removeAt(id);
        log("After removing first if global::audio::queue.size(): " + QString::number(global::audio::queue.size()), className);
        if(global::audio::queue.size() == 0) {
            global::audio::currentAction.append(global::audio::Action::Stop);
            global::audio::audioMutex.unlock();
            return void();
        }
        global::audio::audioMutex.unlock();
        emit playFileChild(id - 2);
        return void();
    }
    else if(id + 1 != global::audio::queue.size()) {
        global::audio::queue.removeAt(id);
        log("After removing second if global::audio::queue.size(): " + QString::number(global::audio::queue.size()), className);
        if(global::audio::queue.size() == 0) {
            global::audio::currentAction.append(global::audio::Action::Stop);
            global::audio::audioMutex.unlock();
            return void();
        }
        global::audio::audioMutex.unlock();
        emit playFileChild(id - 1);
        return void();
    }
    else {
        log("What happened?", className);
        global::audio::currentAction.append(global::audio::Action::Stop);
        global::audio::audioMutex.unlock();
        return void();
    }
    log("Something went wrong with deleting item in query", className);
    global::audio::audioMutex.unlock();
}

void audiofilequeue::on_playButton_clicked()
{
    global::audio::audioMutex.lock();
    for(int i = 0; i < global::audio::queue.size(); i++) {
        if(global::audio::queue[i].id == file.id) {
            global::audio::audioMutex.unlock();
            emit playFileChild(i);
            return void();
        }
    }
    global::audio::audioMutex.unlock();
    log("Something went really wrong", className);
}

