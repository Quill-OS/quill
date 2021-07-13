#include "wifidialog.h"
#include "ui_wifidialog.h"

wifiDialog::wifiDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wifiDialog)
{
    ui->setupUi(this);
}

wifiDialog::~wifiDialog()
{
    delete ui;
}
