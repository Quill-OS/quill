#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QObject>

class audiothread : public QObject
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    audiothread();

public slots:
    void start();

private:
};

#endif // AUDIOTHREAD_H
