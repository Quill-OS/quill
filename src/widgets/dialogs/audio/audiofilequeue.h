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

    void provideData(global::audio::musicFile fileProvided);
    global::audio::musicFile file;

public slots:
    void die();

private:
    Ui::audiofilequeue *ui;

signals:
    void playFileChild(int itemInQueue);

};

#endif // AUDIOFILEQUEUE_H
