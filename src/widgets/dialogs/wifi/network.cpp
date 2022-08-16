#include "network.h"
#include "ui_network.h"

network::network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::network)
{
    ui->setupUi(this);
}

network::~network()
{
    delete ui;
}
