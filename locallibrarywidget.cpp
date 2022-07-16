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
    QObject::connect(ui->pageNumberLabel, &QClickableLabel::clicked, this, &localLibraryWidget::openGoToPageDialog);

    ui->previousPageBtn->setProperty("type", "borderless");
    ui->previousPageBtn->setEnabled(false);
    ui->nextPageBtn->setProperty("type", "borderless");
    ui->previousPageBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextPageBtn->setIcon(QIcon(":/resources/chevron-right.png"));
    ui->pageNumberLabel->setFont(QFont("Source Serif Pro"));
    ui->pageNumberLabel->setStyleSheet("color: black; background-color: white; border-radius: 10px; padding-left: 10px; padding-right: 10px");
    ui->verticalLayout->setSpacing(4);

    if(global::deviceID == "n705\n") {
        buttonsNumber = 3;
    }
    else if(global::deviceID == "n873\n") {
        buttonsNumber = 5;
    }
    else {
        buttonsNumber = 4;
    }

    if(global::deviceID == "n705\n") {
        bookTitleTruncateThreshold = 30;
    }
    else if(global::deviceID == "n873\n") {
        bookTitleTruncateThreshold = 45;
    }
    else {
        bookTitleTruncateThreshold = 35;
    }

    bookIconArray.resize(buttonsNumber);
    bookBtnArray.resize(buttonsNumber);
    horizontalLayoutArray.resize(buttonsNumber);
    lineArray.resize(buttonsNumber);

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        stdIconWidthDivider = 9.5;
        stdIconHeightDivider = 9.5;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n") {
        stdIconWidthDivider = 8.5;
        stdIconHeightDivider = 8.5;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconWidth = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n437\n") {
        stdIconWidthDivider = 8;
        stdIconHeightDivider = 8;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else if(global::deviceID == "n873\n") {
        stdIconWidthDivider = 9.7;
        stdIconHeightDivider = 9.7;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }
    else {
        stdIconWidthDivider = 9.5;
        stdIconHeightDivider = 9.5;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
    }

    for(int i = 1; i <= buttonsNumber; i++) {
        // Horizontal layout that will contain the book button and its icon
        horizontalLayoutArray[i] = new QHBoxLayout(this);

        // Book icon label
        bookIconArray[i] = new QLabel(this);
        bookIconArray[i]->setStyleSheet("padding: 10px");

        // Book button
        bookBtnArray[i] = new QClickableLabel(this);
        bookBtnArray[i]->setObjectName(QString::number(i));
        QObject::connect(bookBtnArray[i], &QClickableLabel::bookID, this, &localLibraryWidget::btnOpenBook);
        QObject::connect(bookBtnArray[i], &QClickableLabel::longPress, this, &localLibraryWidget::openBookOptionsDialog);
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

    if(!QFile::exists(global::localLibrary::databasePath)) {
        global::toast::modalToast = true;
        global::toast::indefiniteToast = true;
        showToast("Generating database");
    }
    QTimer::singleShot(100, this, SLOT(setupDisplay()));
}

localLibraryWidget::~localLibraryWidget()
{
    delete ui;
}

void localLibraryWidget::setupDatabase() {
    setDefaultWorkDir();
    if(!QFile::exists(global::localLibrary::databasePath)) {
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
        args << "env" << "icon_width_divider=" + QString::number(stdIconWidthDivider - 1.5) << "icon_height_divider=" + QString::number(stdIconHeightDivider - 1.5) << "./explore_local_library.sh" << booksList;
        QProcess *proc = new QProcess();
        proc->start(prog, args);
        proc->waitForFinished(-1);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(readFile(global::localLibrary::rawDatabasePath).toUtf8());
        QFile::remove(global::localLibrary::rawDatabasePath);
        proc->deleteLater();

        // Write database in compressed form, encoded in Base64 format
        writeFile(global::localLibrary::databasePath, qCompress(jsonDocument.toJson()).toBase64());
        toastWindow->close();
    }

    // Read library database from file
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

    // Uncompress data from Base64 encoding
    databaseJsonDocument = QJsonDocument::fromJson(qUncompress(QByteArray::fromBase64(data)));
    // Parse JSON data
    databaseJsonObject = databaseJsonDocument.object();
    databaseJsonArrayList = databaseJsonObject["database"].toArray();
    // Determine maximum page number
    booksNumber = databaseJsonArrayList.size();
    pagesNumber = std::ceil((double)booksNumber / buttonsNumber);
    if(databaseJsonArrayList.isEmpty()) {
        log("Database is empty", className);
        noBooksInDatabase = true;
    }
    else {
        noBooksInDatabase = false;
        if(pagesNumber == 1) {
            ui->nextPageBtn->setEnabled(false);
        }
    }

    if(global::localLibrary::headless == true) {
        global::localLibrary::headless = false;
        localLibraryWidget::close();
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
            bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));
        }
        else {
            QPixmap pixmap(":/resources/cover_unavailable.png");
            bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight));
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
    ui->pageNumberLabel->setText(QString::number(pageNumber) + " <i>of</i> " + QString::number(pagesNumber));
    // NOTICE: Memory leak?
    // Do it twice, otherwise the layout doesn't show as intended
    for(int i = 0; i <= 1; i++) {
        ui->verticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    }

    // Set boundaries for 'Previous'/'Next' page turn buttons
    currentPageNumber = pageNumber;
    ui->previousPageBtn->setEnabled(true);
    ui->nextPageBtn->setEnabled(true);
    if(currentPageNumber - 1 < 1) {
        ui->previousPageBtn->setEnabled(false);
        ui->nextPageBtn->setEnabled(true);
    }
    if(currentPageNumber + 1 > pagesNumber) {
        ui->previousPageBtn->setEnabled(true);
        ui->nextPageBtn->setEnabled(false);
    }
    if(currentPageNumber - 1 < 1 and currentPageNumber + 1 > pagesNumber) {
        ui->previousPageBtn->setEnabled(false);
        ui->nextPageBtn->setEnabled(false);
    }
}

void localLibraryWidget::on_previousPageBtn_clicked()
{
    currentPageNumber--;
    setupBooksList(currentPageNumber);

    pagesTurned = pagesTurned + 1;
    if(pagesTurned >= 3) {
        emit refreshScreen();
        pagesTurned = 0;
    }
}


void localLibraryWidget::on_nextPageBtn_clicked()
{
    currentPageNumber++;
    setupBooksList(currentPageNumber);

    pagesTurned = pagesTurned + 1;
    if(pagesTurned >= 3) {
        emit refreshScreen();
        pagesTurned = 0;
    }
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

void localLibraryWidget::openGoToPageDialog() {
    log("Showing 'Go to page' dialog", className);
    global::keyboard::keypadDialog = true;
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(generalDialogWindow, &generalDialog::gotoPageSelected, this, &localLibraryWidget::goToPage);
}

void localLibraryWidget::goToPage(int page) {
    if(page > pagesNumber or page <= 0) {
        log("Page number specified (" + QString::number(page) + ") is out of range", className);
        showToast("Request is beyond page range");
    }
    else {
        log("Going to page " + QString::number(page), className);
        setupBooksList(page);
        emit refreshScreen();
    }
}

void localLibraryWidget::refreshScreenNative() {
    emit refreshScreen();
}

void localLibraryWidget::setupDisplay() {
    setupDatabase();
    if(noBooksInDatabase == false) {
        setupBooksList(currentPageNumber);
    }
    else {
        ui->previousPageBtn->setEnabled(false);
        ui->nextPageBtn->setEnabled(false);
        ui->pageNumberLabel->setText("1 <i>of</i> 1");
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void localLibraryWidget::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    toastWindow->show();
}

void localLibraryWidget::openBookOptionsDialog(int pseudoBookID) {
    // Determine book ID from the book's button number
    int bookID = ((currentPageNumber * buttonsNumber) - (buttonsNumber - 1)) + (pseudoBookID - 1);

    log("Opening book options dialog for book with pseudo-ID " + QString::number(pseudoBookID) + ", ID " + QString::number(bookID), className);
    global::localLibrary::bookOptionsDialog::bookID = bookID;
    bookOptionsDialog * bookOptionsDialogWindow = new bookOptionsDialog(this);
    QObject::connect(bookOptionsDialogWindow, &bookOptionsDialog::openLocalBookInfoDialog, this, &localLibraryWidget::openLocalBookInfoDialog);
    QObject::connect(bookOptionsDialogWindow, &bookOptionsDialog::showToast, this, &localLibraryWidget::showToast);
    QObject::connect(bookOptionsDialogWindow, &bookOptionsDialog::destroyed, this, &localLibraryWidget::handlePossibleBookDeletion);
    bookOptionsDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    bookOptionsDialogWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    bookOptionsDialogWindow->show();
}

void localLibraryWidget::handlePossibleBookDeletion() {
    if(global::localLibrary::bookOptionsDialog::bookDeleted == true) {
        QTimer::singleShot(3100, this, [&]() {
            global::localLibrary::bookOptionsDialog::bookDeleted = false;
            global::toast::modalToast = true;
            global::toast::indefiniteToast = true;
            showToast("Generating database");
            QTimer::singleShot(100, this, SLOT(setupDisplay()));
        });
    }
}

void localLibraryWidget::openLocalBookInfoDialog() {
    global::bookInfoDialog::localInfoDialog = true;
    bookInfoDialog * bookInfoDialogWindow = new bookInfoDialog(this);
    bookInfoDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    bookInfoDialogWindow->show();
}
