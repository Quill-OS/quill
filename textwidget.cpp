#include "textwidget.h"
#include "ui_textwidget.h"
#include "functions.h"

#include <QFile>
#include <QProcess>

textwidget::textwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::textwidget)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->textBrowser->setText(global::text::textBrowserContents);
}

textwidget::~textwidget()
{
    delete ui;
}
