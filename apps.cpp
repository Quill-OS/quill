#include "apps.h"
#include "ui_apps.h"
#include <QFile>
#include <QProcess>

apps::apps(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apps)
{
    ui->setupUi(this);
    ui->backBtn->setProperty("type", "borderless");
    ui->scribbleLaunchBtn->setProperty("type", "borderless");
    ui->lightmapsLaunchBtn->setProperty("type", "borderless");
    ui->calendarLaunchBtn->setProperty("type", "borderless");
    ui->savedWordsLaunchBtn->setProperty("type", "borderless");

    ui->scribbleLaunchBtn->setStyleSheet("background: lightGrey");
    ui->lightmapsLaunchBtn->setStyleSheet("background: lightGrey");
    ui->calendarLaunchBtn->setStyleSheet("background: lightGrey");
    ui->savedWordsLaunchBtn->setStyleSheet("background: lightGrey");

    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    QObject::connect(ui->backBtn, SIGNAL(clicked()), this, SLOT(exitSlot()));
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
