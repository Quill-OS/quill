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

    ui->koboxAppsOpenButton->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->scribbleLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->lightmapsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calendarLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->savedWordsLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->calculatorLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");
    ui->vncLaunchBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    // Hiding KoBox apps button and label if X11 isn't enabled/wasn't started
    if(checkconfig("/external_root/boot/flags/X11_START") != true) {
        ui->label_5->hide();
        ui->koboxAppsOpenButton->hide();
        ui->label_5->deleteLater();
        ui->koboxAppsOpenButton->deleteLater();
    }
    if(checkconfig("/external_root/boot/flags/X11_START") != true) {
        ui->label_5->hide();
        ui->koboxAppsOpenButton->hide();
        ui->label_5->deleteLater();
        ui->koboxAppsOpenButton->deleteLater();
    }

    QFile stylesheetFile(":/resources/eink.qss");
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

}
