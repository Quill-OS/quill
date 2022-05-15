#include "userapp.h"
#include "ui_userapp.h"

userapp::userapp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userapp)
{
    ui->setupUi(this);
}

userapp::~userapp()
{
    delete ui;
}
