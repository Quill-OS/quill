#include "quit.h"
#include "ui_quit.h"
#include "functions.h"
#include <QProcess>
#include <QIcon>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QScreen>

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

    poweroff(true);
    qApp->quit();
}

void quit::on_pushButton_2_clicked()
{
    global::battery::showCriticalBatteryAlert = false;
    global::battery::showLowBatteryDialog = false;

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
    // inotifywait waits for a MODIFY event, so we just do it instead of evtest and the power button
    string_writeconfig("/external_root/tmp/power", "KEY_POWER");
}
