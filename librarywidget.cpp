#include "librarywidget.h"
#include "ui_librarywidget.h"

libraryWidget::libraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::libraryWidget)
{
    ui->setupUi(this);
}

libraryWidget::~libraryWidget()
{
    delete ui;
}
