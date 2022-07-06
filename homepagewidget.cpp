#include "homepagewidget.h"
#include "ui_homepagewidget.h"

#include <QScreen>

#include "functions.h"

homePageWidget::homePageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::homePageWidget)
{
    ui->setupUi(this);

    horizontalLayoutArray.resize(global::homePageWidget::recentBooksRowNumber);
    verticalLayoutArray.resize(global::homePageWidget::recentBooksNumber);
    bookBtnArray.resize(global::homePageWidget::recentBooksNumber);
    bookTitleArray.resize(global::homePageWidget::recentBooksNumber);

    if(global::deviceID == "n705\n") {
        bookTitleTruncateThreshold = 20;
    }
    else if(global::deviceID == "n873\n") {
        bookTitleTruncateThreshold = 35;
    }
    else {
        bookTitleTruncateThreshold = 25;
    }

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

    if(!QFile::exists(global::localLibrary::databasePath)) {
        global::toast::modalToast = true;
        global::toast::indefiniteToast = true;
        showToast("Generating database");
        QTimer::singleShot(100, this, SLOT(setupDisplayWithDatabase()));
    }
    else {
        setupDisplay(false);
    }
}

homePageWidget::~homePageWidget()
{
    delete ui;
}

void homePageWidget::openBook(QString bookPath) {
    if(!bookPath.isEmpty()) {
        emit openBookSignal(bookPath, false);
    }
    else {
        showToast("Book not found");
    }
}

void homePageWidget::refreshScreenNative() {
    emit refreshScreen();
}

void homePageWidget::setupDisplay(bool databaseGenerated) {
    if(databaseGenerated == true) {
        toastWindow->close();
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

    log("Reading recent books database", className);
    QFile recentBooksDatabase(global::localLibrary::recentBooksDatabasePath);
    QByteArray recentBooksData;
    if(recentBooksDatabase.open(QIODevice::ReadOnly)) {
        recentBooksData = recentBooksDatabase.readAll();
        recentBooksDatabase.close();
    }
    else {
        QString function = __func__; log(function + ": Failed to open recent books database file for reading at '" + recentBooksDatabase.fileName() + "'", className);
    }

    QJsonObject databaseJsonObject = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data))).object();
    QJsonArray databaseJsonArrayList = databaseJsonObject["database"].toArray();
    int databaseBooksNumber = databaseJsonArrayList.size();
    QJsonObject recentBooksJsonObject = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(recentBooksData))).object();

    log("Setting up home page", className);

    if(!recentBooksJsonObject["Book1"].toObject().value("BookPath").toString().isEmpty()) {
        int in = 0;
        bool newRow = false;
        for(int i = 1; i <= (global::homePageWidget::recentBooksNumber + 1); i++) {
            if(in < global::homePageWidget::recentBooksRowNumber) {
                QString objectName = "Book" + QString::number(i);
                QJsonObject jsonObject = recentBooksJsonObject[objectName].toObject();
                QString bookPath = jsonObject.value("BookPath").toString();
                bookBtnArray[i] = new QClickableLabel(this);
                bookTitleArray[i] = new QToolTipLabel(this);

                // Iterate until we find a book matching the recently opened book's "BookPath" key/value pair
                for(int in = 1; in <= databaseBooksNumber; in++) {
                    QJsonObject bookJsonObject = databaseJsonArrayList.at(in - 1).toObject();
                    if(bookJsonObject["BookPath"].toString() == bookPath) {
                        QByteArray data = qCompress(QJsonDocument(bookJsonObject).toJson()).toBase64();
                        QString dataString = QString(data);
                        bookBtnArray[i]->setObjectName(dataString);
                    }
                }
                QJsonObject uncompressedJsonObject = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(bookBtnArray[i]->objectName().toUtf8()))).object();

                verticalLayoutArray[i] = new QVBoxLayout();

                // Book icon button
                QObject::connect(bookBtnArray[i], &QClickableLabel::bookPath, this, &homePageWidget::openBook);
                bookBtnArray[i]->setAlignment(Qt::AlignCenter);
                bookBtnArray[i]->setFont(QFont("u001"));
                bookBtnArray[i]->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 10px");
                // Book title label
                bookTitleArray[i]->setWordWrap(true);
                bookTitleArray[i]->setAlignment(Qt::AlignCenter);
                bookTitleArray[i]->setFont(QFont("u001"));
                bookTitleArray[i]->setStyleSheet("font-size: 7pt");

                QString bookTitle = uncompressedJsonObject["Title"].toString();
                bookTitleArray[i]->setObjectName(bookTitle);

                int localBookTitleTruncateThreshold;
                if(!bookTitle.contains(" ")) {
                    localBookTitleTruncateThreshold = bookTitleTruncateThreshold - 10;
                }
                else {
                    localBookTitleTruncateThreshold = bookTitleTruncateThreshold;
                }
                if(bookTitle.length() > localBookTitleTruncateThreshold) {
                    bookTitleArray[i]->setProperty("showToolTip", "true");
                    bookTitle.truncate(localBookTitleTruncateThreshold);
                    bookTitle.append("...");
                }
                bookTitleArray[i]->setText(bookTitle);

                QString bookIcon = uncompressedJsonObject["CoverPath"].toString();
                if(QFile::exists(bookIcon)) {
                    bookBtnArray[i]->setPixmap(QPixmap(bookIcon).scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
                }
                else {
                    bookBtnArray[i]->setPixmap(QPixmap(":/resources/cover_unavailable").scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
                }

                verticalLayoutArray[i]->addWidget(bookBtnArray[i]);
                verticalLayoutArray[i]->addWidget(bookTitleArray[i]);
            }

            if(newRow == true) {
                newRow = false;
                horizontalLayoutArray[in] = new QHBoxLayout();
                for(int n = ((i - 1) - (global::homePageWidget::recentBooksNumberPerRow - 1)); n < (((i - 1) - (global::homePageWidget::recentBooksNumberPerRow - 1)) + global::homePageWidget::recentBooksNumberPerRow); n++) {
                    horizontalLayoutArray[in]->addLayout(verticalLayoutArray[n]);
                }
                ui->booksVerticalLayout->addLayout(horizontalLayoutArray[in]);
            }
            if(!(i % global::homePageWidget::recentBooksNumberPerRow)) {
                newRow = true;
                in++;
            }
        }

        // Hiding books that don't exist/are not in the database
        // If you think this is not an efficient way to do it, try to teach the code above not create the button if the book does not exist and arrange layouts so that it looks linear
        for(int i = 1; i <= global::homePageWidget::recentBooksNumber; i++) {
            QString bookPath = recentBooksJsonObject["Book" + QString::number(i)].toObject().value("BookPath").toString();
            if(bookPath.isEmpty()) {
                bookBtnArray[i]->hide();
                bookTitleArray[i]->hide();
                bookBtnArray[i]->deleteLater();
                bookTitleArray[i]->deleteLater();
            }
        }
    }
    else {
        log("No books found in recent books database", className);
        QFrame * line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(1);
        ui->booksVerticalLayout->addWidget(line);
        QLabel * noRecentBooksLabel = new QLabel(this);
        noRecentBooksLabel->setFont(QFont("Chivo"));
        noRecentBooksLabel->setAlignment(Qt::AlignCenter);
        noRecentBooksLabel->setStyleSheet("padding: 30px");
        noRecentBooksLabel->setText("Recently read books will appear here");
        ui->booksVerticalLayout->addWidget(noRecentBooksLabel);
    }
    QTimer::singleShot(500, this, SLOT(refreshScreenNative()));
}

void homePageWidget::setupDisplaySlot() {
    setupDisplay(true);
}

void homePageWidget::setupDisplayWithDatabase() {
    global::localLibrary::headless = true;
    localLibraryWidget * localLibraryWidgetWindow = new localLibraryWidget(this);
    localLibraryWidgetWindow->setAttribute(Qt::WA_DeleteOnClose);
    localLibraryWidgetWindow->hide();
    QObject::connect(localLibraryWidgetWindow, &localLibraryWidget::destroyed, this, &homePageWidget::setupDisplaySlot);
}

void homePageWidget::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    toastWindow->show();
}
