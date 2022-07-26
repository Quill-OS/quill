#ifndef SLEEPTHREAD_H
#define SLEEPTHREAD_H

#include <QObject>

class sleepThread : public QObject {
        Q_OBJECT
public:
    QString className = this->metaObject()->className();
    sleepThread();

public slots:
    void start();

signals:
    void startDialog();
    void stopDialog();
};

#endif // SLEEPTHREAD_H
