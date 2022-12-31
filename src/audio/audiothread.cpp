#include "audiothread.h"
#include "functions.h"

#include <QTimer>
// Socket things
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#include <QTimer>

audiothread::audiothread() {}

void audiothread::start() {
    log("Audio thread monitor available", className);

    // QTimer doesnt work in such loops... well anyways
    int count = 0;
    int secondsToCount = 5;
    while(true) {
        if(global::audio::currentAction != global::audio::Action::None) {
            log("Action received", className);
            if(global::audio::currentAction == global::audio::Action::Stop) {
                log("Stop action received", className);
                sendInfo("pause:"); // Yea, only that
                global::audio::audioMutex.lock();
                global::audio::paused = true;
                global::audio::isSomethingCurrentlyPlaying = false;
                monitorProgress = false;

                global::audio::currentAction = global::audio::Action::None;
                global::audio::actionDone = true;
                global::audio::audioMutex.unlock();
            }
        }
        if(global::audio::currentAction == global::audio::Action::Play) {
            log("play action received", className);
            global::audio::audioMutex.lock();
            QString message = "play:\"";
            QString betterPath = global::audio::queue[global::audio::itemCurrentlyPLaying].path.remove(0, 21); // remove /mnt/onboard/onboard/
            message.append(betterPath);
            message.append("\"");
            sendInfo(message);
            global::audio::paused = false;
            global::audio::isSomethingCurrentlyPlaying = true;
            global::audio::progressSeconds = 0;
            monitorProgress = true;

            global::audio::currentAction = global::audio::Action::None;
            global::audio::actionDone = true;
            global::audio::audioMutex.unlock();
        }
        count = count + 1;;
        QThread::msleep(200);
        if(count == secondsToCount) {
            count = 0;
            if(monitorProgress == true) {
                audioProgress();
            }
        }
    }
}

// I have no explanation why this needs to connect / disconnect every time.
// while (recv(client_sockfd, buffer_tmp, 1, 0) > 0) {
// This line in audio inkbox freezes, but logs inside while doesn't so idk what is going on
void audiothread::sendInfo(QString message) {
    // Send
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        log("Error creating socket?", className);
    }

    // Connect to the socket
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/dev/iaudio.socket");
    res = ::connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (res < 0) {
        log("Error connecting to socket", className);
    }

    log("Sending message: " + message, className);
    res = send(sockfd, message.toStdString().c_str(), strlen(message.toStdString().c_str()), 0);
    if (res < 0) {
        log("Error sending to socket", className);
    }

    close(sockfd);
}

void audiothread::audioProgress() {
    global::audio::progressSeconds = global::audio::progressSeconds + 1;
    log("Progress, +1 sec: " + QString::number(global::audio::progressSeconds), className);
    if(global::audio::progressSeconds == global::audio::queue[global::audio::itemCurrentlyPLaying].lengths) {
        monitorProgress = false;
    }
}
