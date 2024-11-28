#include "flashexam.h"
#include "ui_flashexam.h"

#include "functions.h"

flashExam::flashExam(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::flashExam)
{
    ui->setupUi(this);
    this->setStyleSheet(readFile("/mnt/onboard/.adds/inkbox/eink.qss"));
    this->setFont(QFont("u001"));
    ui->listWidget->setFont(QFont("u001"));
    ui->textBrowser->setFont(QFont("u001"));

    ui->startBtn->setProperty("type", "borderless");
    ui->closeBtn->setProperty("type", "borderless");
    ui->revealBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->backBtn->setProperty("type", "borderless");
    ui->startBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none }");
    ui->closeBtn->setStyleSheet("QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->revealBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->nextBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->backBtn->setStyleSheet("QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->textBrowser->setStyleSheet("font-size: 12pt");
    ui->closeBtn->setIcon(QIcon(":/resources/close.png"));
    ui->backBtn->setIcon(QIcon(":/resources/arrow-left.png"));
    ui->randomizeCheckBox->click();

    graphicsScene = new QGraphicsScene(this);
    setupCardsList();
}

flashExam::~flashExam()
{
    delete ui;
}

void flashExam::on_closeBtn_clicked()
{
    this->close();
}

void flashExam::setupCardsList() {
    QDir dir("/mnt/onboard/onboard/.flashexam");
    for (const QString &filename : dir.entryList(QDir::Files)) {
        ui->listWidget->addItem(filename);
    }
}

void flashExam::on_startBtn_clicked()
{
    listName = ui->listWidget->currentItem()->text();
    if(ui->listWidget->selectedItems().isEmpty()) {
        emit showToast("You must select a cards list");
    }
    else {
        QString cardsList = "/mnt/onboard/onboard/.flashexam/" + listName;
        QString answersList = "/mnt/onboard/onboard/.flashexam/" + listName + ".answers";
        if(QFile::exists(answersList)) {
            log("Setting up cards list '" + listName + "'", className);
            initCardsList(cardsList, answersList);
        }
        else {
            emit showToast("No answers file found");
        }
    }
}

void flashExam::initCardsList(QString cardsList, QString answersList) {
    cardsStringList = readFile(cardsList).split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
    answersStringList = readFile(answersList).split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
    randomize = ui->randomizeCheckBox->isChecked();
    cardsTotal = cardsStringList.count() + 1;
    displayCard(false);
    ui->stackedWidget->setCurrentIndex(1);
}

void flashExam::on_backBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void flashExam::on_revealBtn_clicked()
{
    if(answerShown) {
        displayCard(true);
    }
    else {
        QString answerText = displayImage(answersStringList.at(currentCardNumber));
        ui->textBrowser->setText(answerText);
        answerShown = true;
        ui->revealBtn->setText("Hide answer");
    }
}


void flashExam::on_nextBtn_clicked()
{
    displayCard(false);
}

void flashExam::displayCard(bool existingCardNumber) {
    if(!existingCardNumber) {
        if(randomize) {
            currentCardNumber = QRandomGenerator::global()->bounded(cardsTotal - 1);
        }
    }
    QString cardText = displayImage(cardsStringList.at(currentCardNumber));
    ui->cardNumberLabel->setText("Card " + QString::number(currentCardNumber + 1));
    ui->textBrowser->setText(cardText);

    ui->revealBtn->setText("Show answer");
    answerShown = false;
}

QString flashExam::displayImage(QString cardText) {
    ui->textBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->graphicsView->hide();
    QRegularExpression imageRegex("IMG='([^']+)'");
    QRegularExpressionMatch match = imageRegex.match(cardText);

    if (match.hasMatch()) {
        QString imagePath = match.captured(1); // Captured group 1 is the value of IMG
        log("Displaying image '" + imagePath + "'", className);

        ui->graphicsView->items().clear();
        graphicsScene->clear();
        QPixmap pixmap("/mnt/onboard/onboard/.flashexam/resources/" + listName + "/" + imagePath);
        graphicsScene->addPixmap(pixmap);
        ui->graphicsView->setScene(graphicsScene);
        // Shrinking scene if item is smaller than previous one
        QRectF rect = graphicsScene->itemsBoundingRect();
        graphicsScene->setSceneRect(rect);
        ui->graphicsView->fitInView(graphicsScene->sceneRect(), Qt::KeepAspectRatio);
        ui->textBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        ui->graphicsView->show();
    }
    else {
        log("IMG key not found", className);
    }

    QRegularExpression removeRegex("IMG='[^']+'\\s*");
    cardText.remove(removeRegex);

    return cardText;
}
