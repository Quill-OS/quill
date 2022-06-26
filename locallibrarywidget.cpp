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

    ui->previousPageBtn->setProperty("type", "borderless");
    ui->nextPageBtn->setProperty("type", "borderless");
    ui->previousPageBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextPageBtn->setIcon(QIcon(":/resources/chevron-right.png"));
    ui->pageNumberLabel->setFont(QFont("Source Serif Pro"));

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
        bookIconArray[i]->setStyleSheet("padding: 10px");

        // Book button
        bookBtnArray[i] = new QClickableLabel(this);
        bookBtnArray[i]->setObjectName(QString::number(i));
        connect(bookBtnArray[i], SIGNAL(bookID(int)), SLOT(btnOpenBook(int)));
        bookBtnArray[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        bookBtnArray[i]->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding: 10px");
        bookBtnArray[i]->setFont(QFont("u001"));

        horizontalLayout->addWidget(bookIconArray[i]);
        horizontalLayout->addWidget(bookBtnArray[i]);
        ui->booksVerticalLayout->addLayout(horizontalLayout);
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

        QString prog("sh");
        QStringList args;
        args << "explore_local_library.sh" << booksList;
        QProcess *proc = new QProcess();
        proc->setEnvironment((QStringList() << "EXTRACT_COVER=1"));
        proc->start(prog, args);
        proc->waitForFinished();
        QJsonDocument jsonDocument = QJsonDocument::fromJson(proc->readAllStandardOutput());
        proc->deleteLater();

        // Write database in compressed form, encoded in Base64 format
        writeFile(localLibraryDatabasePath, qCompress(jsonDocument.toJson()).toBase64());
    }

    // Read library database from file
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
}

void localLibraryWidget::setupBooksList(int pageNumber) {
    idList->clear();
    int in = 1;
    for(int i = (1 * pageNumber * buttonsNumber) - (buttonsNumber - 1); i <= (1 * pageNumber * buttonsNumber); i++) {
        // Read database info for each book and display the corresponding information on each button
        QJsonObject jsonObject = databaseJsonArrayList.at(i - 1).toObject();
        QString bookTitle = jsonObject["Title"].toString();
        QString bookAuthor = jsonObject["Author"].toString();
        QString coverPath = jsonObject["CoverPath"].toString();
        QString bookID = jsonObject["BookID"].toString();
        if(!coverPath.isEmpty()) {
            QPixmap pixmap(coverPath);
            bookIconArray[in]->setPixmap(pixmap);
        }
        idList->append(bookID.toInt());
        bookBtnArray[in]->setText("<b>" + bookTitle + "</b>" + "<br>" + bookAuthor);
        in++;
    }
    ui->pageNumberLabel->setText("Page " + QString::number(pageNumber) + " <i>of</i> " + "10");
}

void localLibraryWidget::on_previousPageBtn_clicked()
{
    currentPageNumber--;
    setupBooksList(currentPageNumber);
}


void localLibraryWidget::on_nextPageBtn_clicked()
{
    currentPageNumber++;
    setupBooksList(currentPageNumber);
}

void localLibraryWidget::openBook(int bookID) {
    QJsonObject jsonObject = databaseJsonArrayList.at(bookID - 1).toObject();
    QString bookPath = jsonObject["BookPath"].toString();
    log("Opening book with ID " + QString::number(bookID) + ", path '" + bookPath + "'", className);
    emit openBookSignal(bookPath, false);
}

void localLibraryWidget::btnOpenBook(int buttonNumber) {
    int id = idList->at(buttonNumber - 1);
    openBook(id);
    // localLibraryWidget::close();
}
