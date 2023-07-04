#ifndef AUDIOFILEQUEUE_H
#define AUDIOFILEQUEUE_H

#include <QWidget>
#include "functions.h"

namespace Ui {
class audiofilequeue;
}

class audiofilequeue : public QWidget
{
    Q_OBJECT

public:
    explicit audiofilequeue(QWidget *parent = nullptr);
    ~audiofilequeue();
    QString className = this->metaObject()->className();
    bool isPlaying = false;

    void provideData(global::audio::musicFile fileProvided, bool grey);
    global::audio::musicFile file;

public slots:
    void die();

private:
    Ui::audiofilequeue *ui;

signals:
    void playFileChild(int itemInQueue);

private slots:
    void on_deleteBtn_clicked();
    void on_playButton_clicked();
};

#endif // AUDIOFILEQUEUE_H
