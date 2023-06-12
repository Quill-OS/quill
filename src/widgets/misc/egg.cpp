#include "egg.h"
#include "ui_egg.h"

egg::egg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::egg)
{
    ui->setupUi(this);
    this->setStyleSheet(readFile("/mnt/onboard/.adds/inkbox/eink.qss"));
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->quitBtn->setProperty("type", "borderless");
    ui->previousBtn->setFont(QFont("u001"));
    ui->nextBtn->setFont(QFont("u001"));
    ui->quitBtn->setFont(QFont("u001"));
    ui->titleLabel->setFont(QFont("Inter"));
    ui->contributorName->setFont(QFont("Inter"));

    graphicsScene = new QGraphicsScene(this);
    changeIndex(index);
}

egg::~egg()
{
    delete ui;
}

void egg::changeIndex(int index) {
    /*
        Contributors list:
            Szybet (0)
            NiLuJe (1)
            akemnade (2)
        Maintainer:
            tux-linux (3)
    */

    if(firstRun == true) {
        firstRun = false;
    }
    else {
        graphicsScene->clear();
        ui->graphicsView->items().clear();
    }

    QPixmap pixmap(":/resources/egg/" + QString::number(index) + ".jpg");
    graphicsScene->addPixmap(pixmap);
    // Shrinking scene if item is smaller than previous one
    QRectF rect = graphicsScene->itemsBoundingRect();
    graphicsScene->setSceneRect(rect);
    ui->graphicsView->setScene(graphicsScene);

    // Contributor name
    QString name = "<div align='center'><b>";
    if(index == 0) {
        name.append("Szybet");
    }
    else if(index == 1) {
        name.append("NiLuJe");
    }
    else if(index == 2) {
        name.append("akemnade");
    }
    else if(index == 3) {
        name.append("tux-linux (Maintainer)");
    }
    name.append("</b></div>");
    ui->contributorName->setText(name);
}

void egg::on_previousBtn_clicked()
{
    if(index - 1 <= maximumIndex && index - 1 >= 0) {
        index--;
        changeIndex(index);
    }
    else {
        QMessageBox::critical(this, "Critical", "Index out of range");
    }
}

void egg::on_nextBtn_clicked()
{
    if(index + 1 <= maximumIndex && index + 1 >= 0) {
        index++;
        changeIndex(index);
    }
    else {
        QMessageBox::critical(this, "Critical", "Index out of range");
    }
}

void egg::on_quitBtn_clicked()
{
    this->close();
}

