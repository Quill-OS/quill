#include "quit.h"
#include "ui_quit.h"
#include "functions.h"
#include <QProcess>
#include <QIcon>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QScreen>
#include <linux/input.h>

quit::quit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::quit)
{
    ui->setupUi(this);
    ui->backBtn->setFont(QFont("u001"));

    ui->pushButton->setProperty("type", "borderless");
    ui->pushButton_2->setProperty("type", "borderless");
    ui->pushButton_3->setProperty("type", "borderless");
    ui->pushButton_4->setProperty("type", "borderless");
    ui->backBtn->setProperty("type", "borderless");

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the "Quit" icon size will be
    float stdIconWidth = sW / 1.25;
    float stdIconHeight = sH / 1.25;

    QPixmap pixmap(":/resources/exit.png");
    QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
    ui->label->setPixmap(scaledPixmap);

    if(checkconfig("/mnt/onboard/.adds/inkbox/.config/20-sleep_daemon/9-deepSleep") == true) {
        ui->pushButton_3->setText("Deep suspend");
    }
}

quit::~quit()
{
    delete ui;
}

void quit::on_pushButton_clicked()
{
    // Prevent strange "appearing from nowhere" low battery or critical battery alerts due to sysfs unmount
    global::battery::showCriticalBatteryAlert = false;
    global::battery::showLowBatteryDialog = false;

    // GUI apps
    updateUserAppsMainJsonFile();

    poweroff(true);
    qApp->quit();
}

void quit::on_pushButton_2_clicked()
{
    global::battery::showCriticalBatteryAlert = false;
    global::battery::showLowBatteryDialog = false;

    // GUI apps
    updateUserAppsMainJsonFile();

    reboot(true);
    qApp->quit();
}

void quit::on_pushButton_4_clicked()
{
    log("Restarting InkBox", className);
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void quit::on_backBtn_clicked()
{
    quit::close();
}

void quit::on_pushButton_3_clicked()
{
    log("Suspending", className);
    if(checkconfig("/mnt/onboard/.adds/inkbox/.config/20-sleep_daemon/9-deepSleep") == true) {
        writeFile("/mnt/onboard/.adds/inkbox/.config/20-sleep_daemon/sleepCall", "deepsleep");
    }
    else {
        emulatePowerButtonInputEvent();
    }
}

void quit::emulatePowerButtonInputEvent() {
    log("Emulating power button input event", className);
    // Input event
    struct input_event inputEvent = {};
    inputEvent.type = EV_KEY;
    inputEvent.code = KEY_POWER;
    // SYN report event
    struct input_event synReportEvent = {};
    synReportEvent.type = EV_SYN;
    synReportEvent.code = SYN_REPORT;
    synReportEvent.value = 0;

    int fd = open("/dev/input/event0", O_WRONLY);
    if(fd != -1) {
        // Send press event
        inputEvent.value = 1;
        ::write(fd, &inputEvent, sizeof(inputEvent));

        // Send SYN report event
        ::write(fd, &synReportEvent, sizeof(synReportEvent));

        // Some sleep
        QThread::msleep(50);

        // Send release event
        inputEvent.value = 0;
        ::write(fd, &inputEvent, sizeof(inputEvent));
        ::write(fd, &synReportEvent, sizeof(synReportEvent));

        ::close(fd);
    }
    else {
        QString function = __func__; log(function + ": Failed to open input device node at '/dev/input/event0'", className);
    }
}
