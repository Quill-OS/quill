#include "koboxsettings.h"
#include "ui_koboxsettings.h"

#include <QFile>

koboxSettings::koboxSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::koboxSettings)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // UI tweaks
    ui->okBtn->setProperty("type", "borderless");
}

koboxSettings::~koboxSettings()
{
    delete ui;
}

void koboxSettings::on_okBtn_clicked()
{
    koboxSettings::close();
}
