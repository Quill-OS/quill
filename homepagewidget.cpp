#include "homepagewidget.h"
#include "ui_homepagewidget.h"

#include <QScreen>

#include "functions.h"

homePageWidget::homePageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homePageWidget)
{
    ui->setupUi(this);

    verticalLayoutArray.resize(global::homePageWidget::recentBooksNumber);
    bookBtnArray.resize(global::homePageWidget::recentBooksNumber);
    bookTitleArray.resize(global::homePageWidget::recentBooksNumber);

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        stdIconWidthDivider = 8;
        stdIconHeightDivider = 8;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n") {
        stdIconWidthDivider = 7;
        stdIconHeightDivider = 7;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconWidth = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n437\n") {
        stdIconWidthDivider = 6.5;
        stdIconHeightDivider = 6.5;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n873\n") {
        stdIconWidthDivider = 6;
        stdIconHeightDivider = 6;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else {
        stdIconWidthDivider = 8;
        stdIconHeightDivider = 8;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }

    log("Reading database", className);
    QFile database(global::localLibrary::databasePath);
    QByteArray data;
    if(database.open(QIODevice::ReadOnly)) {
        data = database.readAll();
        database.close();
    }
    else {
        QString function = __func__; log(function + ": Failed to open local library database file for reading at '" + database.fileName() + "'", className);
    }

    QJsonObject databaseJsonObject = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data))).object();
    QJsonArray databaseJsonArrayList = databaseJsonObject["database"].toArray();
    int databaseBooksNumber = databaseJsonArrayList.size();
    QJsonObject recentBooksJsonObject = QJsonDocument::fromJson(readFile(global::localLibrary::recentBooksDatabasePath).toUtf8()).object();

    log("Setting up home page", className);

    for(int i = 1; i <= global::homePageWidget::recentBooksNumber; i++) {
        QString objectName = "Book" + QString::number(i);
        QJsonObject jsonObject = recentBooksJsonObject[objectName].toObject();
        QString bookPath = jsonObject.value("BookPath").toString();
        bookBtnArray[i] = new QClickableLabel(this);

        // Iterate until we find a book matching the recently opened book's "BookPath" key/value pair
        for(int in = i; in <= databaseBooksNumber; in++) {
            QJsonObject bookJsonObject = databaseJsonArrayList.at(in - 1).toObject();
            if(bookJsonObject["BookPath"] == bookPath) {
                bookBtnArray[i]->setObjectName(QJsonDocument(bookJsonObject).toJson());
            }
        }

        verticalLayoutArray[i] = new QVBoxLayout(this);

        QObject::connect(bookBtnArray[i], &QClickableLabel::bookPath, this, &homePageWidget::openBook);
        bookBtnArray[i]->setAlignment(Qt::AlignCenter);
        bookBtnArray[i]->setFont(QFont("u001"));
        bookBtnArray[i]->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 10px");

        QString bookIcon = QJsonDocument::fromJson(bookBtnArray[i]->objectName().toUtf8()).object()["CoverPath"].toString();
        if(QFile::exists(bookIcon)) {
            bookBtnArray[i]->setPixmap(QPixmap(bookIcon).scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
        }
        else {
            bookBtnArray[i]->setPixmap(QPixmap(":/resources/cover_unavailable").scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
        }

        verticalLayoutArray[i]->addWidget(bookBtnArray[i]);
        ui->horizontalLayout->addLayout(verticalLayoutArray[i]);
    }
    QTimer::singleShot(500, this, SLOT(refreshScreenNative()));
}

homePageWidget::~homePageWidget()
{
    delete ui;
}

void homePageWidget::openBook(QString bookPath) {
    emit openBookSignal(bookPath, false);
}

void homePageWidget::refreshScreenNative() {
    emit refreshScreen();
}
