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
    ui->infoBtn->setProperty("type", "borderless");
    ui->previousBtn->setText("");
    ui->previousBtn->setIcon(QIcon(":/resources/arrow-left.png"));
    ui->nextBtn->setText("");
    ui->nextBtn->setIcon(QIcon(":/resources/arrow-right.png"));
    ui->quitBtn->setText("");
    ui->quitBtn->setIcon(QIcon(":/resources/close.png"));
    ui->infoBtn->setText("");
    ui->infoBtn->setIcon(QIcon(":/resources/info.png"));
    ui->titleLabel->setFont(QFont("Inter"));
    ui->contributorName->setFont(QFont("Inter"));
    if(global::deviceID == "n873\n") {
        ui->nextBtn->setStyleSheet("padding: 13.5px");
        ui->previousBtn->setStyleSheet("padding: 13.5px");
    }
    else if(global::deviceID == "n437\n") {
        ui->nextBtn->setStyleSheet("padding: 12.5px");
        ui->previousBtn->setStyleSheet("padding: 12.5px");
    }
    else {
        ui->nextBtn->setStyleSheet("padding: 10px");
        ui->previousBtn->setStyleSheet("padding: 10px");
    }

    graphicsScene = new QGraphicsScene(this);
    // Set first contributor name
    ui->contributorName->setText("<div align='center'><b>Szybet<br>(Contributor)</b></div>");
    QTimer::singleShot(500, this, SLOT(changeIndexSlot()));
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

    // Contributor name
    QString name = "<div align='center'><b>";
    if(index == 0) {
        name.append("Szybet<br>(Contributor)");
    }
    else if(index == 1) {
        name.append("tux-linux<br>(Maintainer)");
    }
    name.append("</b></div>");
    ui->contributorName->setText(name);

    ui->graphicsView->items().clear();
    graphicsScene->clear();

    QPixmap pixmap(":/resources/egg/" + QString::number(index) + ".jpg");
    graphicsScene->addPixmap(pixmap);
    ui->graphicsView->setScene(graphicsScene);
    // Shrinking scene if item is smaller than previous one
    QRectF rect = graphicsScene->itemsBoundingRect();
    graphicsScene->setSceneRect(rect);
    ui->graphicsView->fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void egg::changeIndexSlot() {
    changeIndex(index);
}

void egg::on_previousBtn_clicked()
{
    if(index - 1 <= maximumIndex && index - 1 >= 0) {
        index--;
        changeIndex(index);
    }
    else {
        QMessageBox::critical(this, "Critical", "<font face='u001'>Index out of range.</font>");
    }
}

void egg::on_nextBtn_clicked()
{
    if(index + 1 <= maximumIndex && index + 1 >= 0) {
        index++;
        changeIndex(index);
    }
    else {
        QMessageBox::critical(this, "Critical", "<font face='u001'>Index out of range.</font>");
    }
}

void egg::on_quitBtn_clicked()
{
    this->close();
}

void egg::on_infoBtn_clicked()
{
    QMessageBox::information(this, "Information", "<font face='u001'>Congratulations, you've found the easter egg!</font>");
}
