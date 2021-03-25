#include "calendarapp.h"
#include "ui_calendarapp.h"

#include <QFile>

calendarApp::calendarApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::calendarApp)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->backBtn->setProperty("type", "borderless");
}

calendarApp::~calendarApp()
{
    delete ui;
}

void calendarApp::on_backBtn_clicked()
{
    calendarApp::close();
}
