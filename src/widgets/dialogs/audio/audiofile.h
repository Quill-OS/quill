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
    void provideData(global::audio::musicFile file);

public slots:
    void die();

private:
    Ui::audiofile *ui;
};

#endif // AUDIOFILE_H
