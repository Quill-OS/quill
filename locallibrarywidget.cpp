#include "locallibrarywidget.h"
#include "ui_locallibrarywidget.h"

localLibraryWidget::localLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::localLibraryWidget)
{
    ui->setupUi(this);
}

localLibraryWidget::~localLibraryWidget()
{
    delete ui;
}
