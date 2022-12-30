#include "audiothread.h"
#include "functions.h"

audiothread::audiothread() {}

void audiothread::start() {
    log("Audio thread monitor available", className);
    while(true) {
        QThread::msleep(300);


    }
}
