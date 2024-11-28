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
    QString currentItem = ui->listWidget->currentItem()->text();
    if(ui->listWidget->selectedItems().isEmpty()) {
        emit showToast("You must select a cards list");
    }
    else {
        QString cardsList = "/mnt/onboard/onboard/.flashexam/" + currentItem;
        QString answersList = "/mnt/onboard/onboard/.flashexam/" + currentItem + ".answers";
        if(QFile::exists(answersList)) {
            log("Setting up cards list '" + currentItem + "'", className);
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

    int it = 1;
    for (auto& i : cardsStringList) {
        i.prepend(QString::number(it) + " ");
        it += 1;
    }

    it = 1;
    for (auto& i : answersStringList) {
        i.prepend(QString::number(it) + " ");
        it += 1;
    }

    cardsTotal = it;
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
        QString answerText = answersStringList.at(currentCardNumber);
        answerText.remove(0, 2);
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
    QString cardText;
    if(!existingCardNumber) {
        if(randomize) {
            currentCardNumber = QRandomGenerator::global()->bounded(cardsTotal - 1);
        }
    }
    cardText = cardsStringList.at(currentCardNumber);
    ui->graphicsView->hide();
    cardText.remove(0, 2);
    ui->cardNumberLabel->setText("Card " + QString::number(currentCardNumber + 1));
    ui->textBrowser->setText(cardText);

    ui->revealBtn->setText("Show answer");
    answerShown = false;
}

