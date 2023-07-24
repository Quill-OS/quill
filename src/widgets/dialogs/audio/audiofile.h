#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "functions.h"

#include <QWidget>

namespace Ui {
class audiofile;
}

class audiofile : public QWidget
{
    Q_OBJECT

public:
    explicit audiofile(QWidget *parent = nullptr);
    ~audiofile();
    QString className = this->metaObject()->className();
    void provideData(global::audio::musicFile fileProvided);
    global::audio::musicFile file;

public slots:
    void die();
    void enableButton();

private slots:

    void on_addBtn_clicked();

signals:
    void playFileChild(int itemInQueue);

private:
    Ui::audiofile *ui;
};

#endif // AUDIOFILE_H
