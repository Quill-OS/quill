#include "locallibrarywidget.h"
#include "ui_locallibrarywidget.h"

#include <QVBoxLayout>
#include <QSpacerItem>
#include <QScreen>

localLibraryWidget::localLibraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::localLibraryWidget)
{
    ui->setupUi(this);
    this->setFont(QFont("u001"));

    ui->previousPageBtn->setProperty("type", "borderless");
    ui->previousPageBtn->setEnabled(false);
    ui->nextPageBtn->setProperty("type", "borderless");
    ui->previousPageBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextPageBtn->setIcon(QIcon(":/resources/chevron-right.png"));
    ui->pageNumberLabel->setFont(QFont("Source Serif Pro"));
    ui->verticalLayout->setSpacing(4);

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
        buttonsNumber = 4;
    }

    if(global::deviceID == "n873\n") {
        bookTitleTruncateThreshold = 50;
    }
    else {
        bookTitleTruncateThreshold = 40;
    }

    bookIconArray.resize(buttonsNumber);
    bookBtnArray.resize(buttonsNumber);
    horizontalLayoutArray.resize(buttonsNumber);
    lineArray.resize(buttonsNumber);

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    stdIconWidth = sW / 9.5;
    stdIconHeight = sH / 9.5;

    for(int i = 1; i <= buttonsNumber; i++) {
        // Horizontal layout that will contain the book button and its icon
        horizontalLayoutArray[i] = new QHBoxLayout(this);

        // Book icon label
        bookIconArray[i] = new QLabel(this);
        bookIconArray[i]->setStyleSheet("padding: 10px");

        // Book button
        bookBtnArray[i] = new QClickableLabel(this);
        bookBtnArray[i]->setObjectName(QString::number(i));
        connect(bookBtnArray[i], SIGNAL(bookID(int)), SLOT(btnOpenBook(int)));
        bookBtnArray[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        bookBtnArray[i]->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 10px");
        bookBtnArray[i]->setFont(QFont("u001"));

        horizontalLayoutArray[i]->addWidget(bookIconArray[i]);
        horizontalLayoutArray[i]->addWidget(bookBtnArray[i]);
        ui->booksVerticalLayout->addLayout(horizontalLayoutArray[i]);

        // Line
        if(i < buttonsNumber) {
            lineArray[i] = new QFrame(this);
            lineArray[i]->setFrameShape(QFrame::HLine);
            lineArray[i]->setFrameShadow(QFrame::Plain);
            lineArray[i]->setLineWidth(1);
            ui->booksVerticalLayout->addWidget(lineArray[i]);
        }
    }
    setupDatabase();
    setupBooksList(currentPageNumber);
}

localLibraryWidget::~localLibraryWidget()
{
    delete ui;
}

void localLibraryWidget::setupDatabase() {
    QString localLibraryDatabasePath = "/mnt/onboard/onboard/.inkbox/LocalLibrary.db";
    setDefaultWorkDir();
    if(!QFile::exists(localLibraryDatabasePath)) {
        log("Generating database", className);
        QStringList booksList;
        QDir dir("/mnt/onboard/onboard");

        QDirIterator dirIt(dir, QDirIterator::Subdirectories);
        while(dirIt.hasNext()) {
            dirIt.next();
            if(QFileInfo(dirIt.filePath()).isFile()) {
                QString suffix = QFileInfo(dirIt.filePath()).suffix();
                if(suffix == "epub" || suffix == "EPUB") {
                    booksList << dirIt.filePath();
                }
            }
        }

        QString prog("busybox-initrd");
        QStringList args;
        args << "sh" << "explore_local_library.sh" << booksList;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(proc->readAllStandardOutput());
        proc->deleteLater();

        // Write database in compressed form, encoded in Base64 format
        writeFile(localLibraryDatabasePath, qCompress(jsonDocument.toJson()).toBase64());
    }

    // Read library database from file
    log("Reading database", className);
    QFile database(localLibraryDatabasePath);
    QByteArray data;
    if(database.open(QIODevice::ReadOnly)) {
        data = database.readAll();
        database.close();
    }
    else {
        QString function = __func__; log(function + ": Failed to open local library database file for reading at '" + database.fileName() + "'", className);
    }

    // Uncompress data from Base64 encoding
    databaseJsonDocument = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data)));
    // Parse JSON data
    databaseJsonObject = databaseJsonDocument.object();
    databaseJsonArrayList = databaseJsonObject["database"].toArray();
    // Determine maximum page number
    booksNumber = databaseJsonArrayList.size();
    pagesNumber = std::ceil((double)booksNumber / buttonsNumber);
    if(pagesNumber == 1) {
        ui->nextPageBtn->setEnabled(false);
    }
}

void localLibraryWidget::setupBooksList(int pageNumber) {
    idList.clear();
    int in = 1;
    for(int i = (1 * pageNumber * buttonsNumber) - (buttonsNumber - 1); i <= (1 * pageNumber * buttonsNumber); i++) {
        // Read database info for each book and display the corresponding information on each button
        bookIconArray[in]->show();
        bookBtnArray[in]->show();
        if(buttonsNumber - in > 0) {
            lineArray[in]->show();
        }

        QJsonObject jsonObject = databaseJsonArrayList.at(i - 1).toObject();
        QString bookTitle = jsonObject["Title"].toString();
        QString bookAuthor = jsonObject["Author"].toString();
        QString coverPath = jsonObject["CoverPath"].toString();
        QString bookID = jsonObject["BookID"].toString();

        if(!coverPath.isEmpty()) {
            // Display book cover if found
            QPixmap pixmap(coverPath);
            bookIconArray[in]->setPixmap(pixmap);
        }
        else {
            QPixmap pixmap(":/resources/cover_unavailable.png");
            bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
        }

        // Display book title
        idList.append(bookID.toInt());
        if(bookTitle.length() > bookTitleTruncateThreshold) {
            bookTitle.truncate(bookTitleTruncateThreshold);
            bookTitle.append("...");
        }
        if(!bookAuthor.isEmpty()) {
            // Book is likely an ePUB
            bookBtnArray[in]->setText("<font face='Inter'><b>" + bookTitle + "</b></font>" + "<br>" + bookAuthor);
        }
        else {
            // Book is likely a PDF or a picture
            bookBtnArray[in]->setText("<font face='Inter'><b>" + bookTitle + "</b></font>");
        }

        if(!bookID.isEmpty()) {
            in++;
        }
    }
    if(in <= buttonsNumber) {
        for(int i = in; i <= buttonsNumber; i++) {
            bookIconArray[i]->hide();
            bookBtnArray[i]->hide();
            if(i - 1 < buttonsNumber) {
                lineArray[(i - 1)]->hide();
            }
        }
    }
    ui->pageNumberLabel->setText("Page " + QString::number(pageNumber) + " <i>of</i> " + QString::number(pagesNumber));
    // NOTICE: Memory leak?
    // Do it twice, otherwise the layout doesn't show as intended
    for(int i = 0; i <= 1; i++) {
        ui->verticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    }
    QTimer::singleShot(100, this, SLOT(refreshScreenNative()));
}

void localLibraryWidget::on_previousPageBtn_clicked()
{
    currentPageNumber--;
    if(currentPageNumber - 1 < 1) {
        ui->previousPageBtn->setEnabled(false);
        ui->nextPageBtn->setEnabled(true);
    }
    setupBooksList(currentPageNumber);
}


void localLibraryWidget::on_nextPageBtn_clicked()
{
    currentPageNumber++;
    if(currentPageNumber + 1 > pagesNumber) {
        ui->previousPageBtn->setEnabled(true);
        ui->nextPageBtn->setEnabled(false);
    }
    setupBooksList(currentPageNumber);
}

void localLibraryWidget::openBook(int bookID) {
    QJsonObject jsonObject = databaseJsonArrayList.at(bookID - 1).toObject();
    QString bookPath = jsonObject["BookPath"].toString();
    log("Opening book with ID " + QString::number(bookID) + ", path '" + bookPath + "'", className);
    emit openBookSignal(bookPath, false);
}

void localLibraryWidget::btnOpenBook(int buttonNumber) {
    int id = idList.at(buttonNumber - 1);
    openBook(id);
    localLibraryWidget::close();
}

void localLibraryWidget::refreshScreenNative() {
    emit refreshScreen();
}
