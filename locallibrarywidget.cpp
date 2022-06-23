#include "locallibrarywidget.h"
#include "ui_locallibrarywidget.h"

#include <QHBoxLayout>
#include <QSpacerItem>
#include <QScreen>

localLibraryWidget::localLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::localLibraryWidget)
{
    ui->setupUi(this);
    this->setFont(QFont("u001"));

    if(global::deviceID == "n705\n" or global::deviceID == "n905\n") {
        buttonsNumber = 4;
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n") {
        buttonsNumber = 5;
    }
    else if(global::deviceID == "n437\n" or global::deviceID == "n873\n") {
        buttonsNumber = 5;
    }
    else {
        buttonsNumber = 3;
    }

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    if(global::deviceID == "n705\n") {
        stdIconWidth = sW / 16;
        stdIconHeight = sH / 16;
    }
    else if(global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        stdIconWidth = sW / 18;
        stdIconHeight = sH / 18;
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n437\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
        stdIconWidth = sW / 16.5;
        stdIconHeight = sH / 16.5;
    }
    else {
        stdIconWidth = sW / 18;
        stdIconHeight = sH / 18;
    }

    for(int i = 1; i <= buttonsNumber; i++) {
        // Horizontal layout that will contain the book button and its icon
        QHBoxLayout * horizontalLayout = new QHBoxLayout(this);

        // Book icon label
        bookIconArray[i] = new QLabel(this);
        bookIconArray[i]->setStyleSheet("padding: 20px");

        // Book button
        bookBtnArray[i] = new QClickableLabel(this);
        bookBtnArray[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        bookBtnArray[i]->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 20px");
        bookBtnArray[i]->setFont(QFont("u001"));

        horizontalLayout->addWidget(bookIconArray[i]);
        horizontalLayout->addWidget(bookBtnArray[i]);
        ui->booksVerticalLayout->addLayout(horizontalLayout);
    }
    setupBooksList();
}

localLibraryWidget::~localLibraryWidget()
{
    delete ui;
}

void localLibraryWidget::setupBooksList() {
    for(int i = 1; i <= buttonsNumber; i++) {
        QPixmap pixmap(":/resources/starred_star.png");
        bookIconArray[i]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight));
        bookBtnArray[i]->setText("<b>Book " + QString::number(i) + "</b><br>Description");
    }
}
