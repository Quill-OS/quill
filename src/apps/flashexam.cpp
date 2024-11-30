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
    ui->didNotKnowBtn->setProperty("type", "borderless");
    ui->startBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none }");
    ui->closeBtn->setStyleSheet("QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->revealBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->nextBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->didNotKnowBtn->setStyleSheet("padding: 20px; QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->backBtn->setStyleSheet("QPushButton[type='borderless']:pressed { background: black; color: white; border: none}");
    ui->textBrowser->setStyleSheet("font-size: 12pt");
    ui->closeBtn->setIcon(QIcon(":/resources/close.png"));
    ui->backBtn->setIcon(QIcon(":/resources/arrow-left.png"));
    ui->didNotKnowBtn->setIcon(QIcon(":/resources/close.png"));
    ui->nextBtn->setIcon(QIcon(":/resources/check.png"));
    ui->nonRedundantRandomizationCheckBox->setDisabled(true);
    ui->randomizeCheckBox->click();
    ui->randomizeCheckBox->setDisabled(true);

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
    ui->listWidget->clear();
    ui->cardNumberLabel->show();
    for (const QString &filename : dir.entryList(QDir::Files)) {
        if(!filename.contains(".answers")) {
            ui->listWidget->addItem(filename);
        }
    }
}

void flashExam::on_startBtn_clicked()
{
    if(ui->listWidget->selectedItems().isEmpty()) {
        emit showToast("You must select a cards list");
    }
    else {
        listName = ui->listWidget->currentItem()->text();
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
    cardsStringList.clear();
    answersStringList.clear();
    cardsStringList = readFile(cardsList).split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
    answersStringList = readFile(answersList).split(QRegExp("(\\r\\n)|(\\n\\r)|\\r|\\n"), QString::SkipEmptyParts);
    randomize = ui->randomizeCheckBox->isChecked();
    nonRedundantRandomization = ui->nonRedundantRandomizationCheckBox->isChecked();
    cardsAlreadyShown.clear();
    cardsNotKnown.clear();
    ui->nonRedundantRandomizationCheckBox->setChecked(false);
    cardsTotal = cardsStringList.count() + 1;
    displayCard(false);
    ui->stackedWidget->setCurrentIndex(1);
}

void flashExam::on_backBtn_clicked()
{
    this->setDisabled(false);
    setupCardsList();
    ui->stackedWidget->setCurrentIndex(0);
}


void flashExam::on_revealBtn_clicked()
{
    if(answerShown) {
        displayCard(true);
    }
    else {
        QString answerText = displayImage(answersStringList.at(currentCardNumber));
        ui->textBrowser->clear();
        ui->textBrowser->setText("<i>" + answerText + "</i>");
        answerShown = true;
        ui->revealBtn->setText("Hide answer");
        ui->continueWidget->show();
    }
}


void flashExam::on_nextBtn_clicked()
{
    displayCard(false);
}

void flashExam::displayCard(bool existingCardNumber) {
    if(nonRedundantRandomization) {
        float cardsTotalFloat = cardsTotal * 1.0;
        float cardsAlreadyShownNumber = cardsAlreadyShown.count() * 1.0;
        float cardsNotKnownNumber = cardsNotKnown.count() * 1.0;
        ui->cardNumberLabel->setText(QString::number(cardsAlreadyShownNumber / cardsTotalFloat * 100, 'f', 1) + "% done, " + QString::number(cardsNotKnownNumber / cardsTotalFloat * 100, 'f', 1) + "% forgotten");
    }
    else {
        ui->cardNumberLabel->hide();
    }

    if(!existingCardNumber) {
        if(randomize) {
            while(true) {
                currentCardNumber = QRandomGenerator::global()->bounded(cardsTotal - 1);
                if(nonRedundantRandomization) {
                    if(!cardsAlreadyShown.contains(currentCardNumber)) {
                        cardsAlreadyShown.append(currentCardNumber);
                        break;
                    }
                    else {
                        if(cardsAlreadyShown.count() != cardsTotal - 1) {
                            log("cardsAlreadyShown already contains random card number chosen, choosing another one", className);
                        }
                        else {
                            emit showToast("No more cards to display");
                            QTimer::singleShot(5000, this, SLOT(on_backBtn_clicked()));
                            this->setDisabled(true);
                            break;
                        }
                    }
                }
                else {
                    break;
                }
            }
        }
    }
    QString cardText = displayImage(cardsStringList.at(currentCardNumber));

    ui->textBrowser->clear();
    ui->textBrowser->setText(cardText);

    ui->revealBtn->setText("Show answer");
    answerShown = false;
    ui->continueWidget->hide();
}

QString flashExam::displayImage(QString cardText) {
    ui->textBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->graphicsView->hide();
    QRegularExpression imageRegex("IMG='([^']+)'");
    QRegularExpressionMatch match = imageRegex.match(cardText);

    if(match.hasMatch()) {
        QString imageFile = match.captured(1); // Captured group 1 is the value of IMG
        QString imagePath = "/mnt/onboard/onboard/.flashexam/resources/" + imageFile;
        log("Displaying image '" + imagePath + "'", className);
        if(QFile::exists(imagePath)) {
            ui->graphicsView->items().clear();
            graphicsScene->clear();
            QPixmap pixmap(imagePath);
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
            log("Image does not exist", className);
        }
    }

    QRegularExpression removeRegex("IMG='[^']+'\\s*");
    cardText.remove(removeRegex);

    return cardText;
}

void flashExam::on_randomizeCheckBox_toggled(bool checked)
{
    if(checked) {
        ui->nonRedundantRandomizationCheckBox->setDisabled(false);
    }
    else {
        ui->nonRedundantRandomizationCheckBox->setDisabled(true);
    }
}


void flashExam::on_didNotKnowBtn_clicked()
{
    cardsNotKnown.append(currentCardNumber);
    on_nextBtn_clicked();
}

