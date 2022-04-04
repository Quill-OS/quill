#include "apps.h"
#include "ui_apps.h"
#include "mainwindow.h"
#include <QFile>
#include <QProcess>

apps::apps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apps)
{
    ui->setupUi(this);
    ui->koboxAppsOpenButton->setProperty("type", "borderless");
    ui->scribbleLaunchBtn->setProperty("type", "borderless");
    ui->lightmapsLaunchBtn->setProperty("type", "borderless");
    ui->calendarLaunchBtn->setProperty("type", "borderless");
    ui->savedWordsLaunchBtn->setProperty("type", "borderless");
    ui->calculatorLaunchBtn->setProperty("type", "borderless");
    ui->vncLaunchBtn->setProperty("type", "borderless");
    ui->reversiLaunchBtn->setProperty("type", "borderless");
    ui->g2048LaunchBtn->setProperty("type", "borderless");

    ui->koboxAppsOpenButton->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->scribbleLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->lightmapsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calendarLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->savedWordsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calculatorLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->vncLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->reversiLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->g2048LaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    // Hiding KoBox apps button and label if X11 isn't enabled/wasn't started
    if(checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
        ui->label_5->hide();
        ui->koboxAppsOpenButton->hide();
        ui->label_5->deleteLater();
        ui->koboxAppsOpenButton->deleteLater();
    }
    // Hiding VNC viewer button and label if device is not rooted and doesn't have a working Wi-Fi adapter, or if X11 is not running
    if(checkconfig("/external_root/opt/root/rooted") == false) {
        if(global::device::isWifiAble == false or checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
            ui->vncViewerLabel->hide();
            ui->vncLaunchBtn->hide();
            ui->vncViewerLabel->deleteLater();
            ui->vncLaunchBtn->deleteLater();
        }
    }
    else {
        if(checkconfig("/external_root/boot/flags/X11_START") == false or checkconfig("/external_root/boot/flags/X11_STARTED") == false) {
            ui->vncViewerLabel->hide();
            ui->vncLaunchBtn->hide();
            ui->vncViewerLabel->deleteLater();
            ui->vncLaunchBtn->deleteLater();
        }
    }
    // Hiding Light Maps if device doesn't have a working Wi-Fi adapter
    if(global::device::isWifiAble == false and global::deviceID != "emu\n") {
        ui->label_2->hide();
        ui->lightmapsLaunchBtn->hide();
        ui->label_2->deleteLater();
        ui->lightmapsLaunchBtn->deleteLater();
    }

    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
}

apps::~apps()
{
    delete ui;
}

void apps::exitSlot() {
    apps::close();
}

void apps::on_scribbleLaunchBtn_clicked()
{
    QProcess process;
    process.startDetached("scribble", QStringList());
    qApp->quit();
}

void apps::on_lightmapsLaunchBtn_clicked()
{
    QProcess process;
    process.startDetached("lightmaps", QStringList());
    qApp->quit();
}

void apps::on_savedWordsLaunchBtn_clicked()
{
    savedWordsWindow = new savedwords();
    savedWordsWindow->setAttribute(Qt::WA_DeleteOnClose);
    savedWordsWindow->showFullScreen();
}

void apps::on_calendarLaunchBtn_clicked()
{
    calendarWindow = new calendarApp();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);
    calendarWindow->showFullScreen();
}

void apps::on_calculatorLaunchBtn_clicked()
{
    QProcess process;
    process.startDetached("calculator", QStringList());
    qApp->quit();
}

void apps::on_koboxAppsOpenButton_clicked()
{
    koboxAppsDialogWindow = new koboxAppsDialog();
    koboxAppsDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    koboxAppsDialogWindow->show();
}

void apps::on_vncLaunchBtn_clicked()
{
    global::keyboard::keyboardDialog = true;
    global::keyboard::vncDialog = true;
    global::keyboard::keyboardText = "";
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreenNative()));
    generalDialogWindow->show();
}

void apps::refreshScreenNative() {
    emit refreshScreen();
}

void apps::on_reversiLaunchBtn_clicked()
{
    QProcess process;
    process.startDetached("qreversi", QStringList());
    qApp->quit();
}

void apps::on_g2048LaunchBtn_clicked()
{
    QProcess process;
    process.startDetached("2048", QStringList());
    qApp->quit();
}
