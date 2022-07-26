#include "sleepdialog.h"
#include "ui_sleepdialog.h"

#include "functions.h"

sleepDialog::sleepDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::sleepDialog)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
}

sleepDialog::~sleepDialog()
{
    delete ui;
}

void sleepDialog::launchSleepDialog()
{
    log("Launching sleep dialog", className);
    this->exec();
}

void sleepDialog::hideSleepDialog()
{
    log("Hiding sleep dialog", className);
    this->hide();
}
