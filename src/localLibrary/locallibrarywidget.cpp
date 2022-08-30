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
        // test
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
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n" or global::deviceID == "emu\n") {
        stdIconWidthDivider = 8.5;
        stdIconHeightDivider = 8.5;
        stdIconWidth = sW / stdIconWidthDivider;
        stdIconHeight = sH / stdIconHeightDivider;
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
        QObject::connect(bookBtnArray[i], &QClickableLabel::longPressInt, this, &localLibraryWidget::openBookOptionsDialog);
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
    qDebug() << databaseJsonDocument;

    // Parse JSON data
    databaseJsonObject = databaseJsonDocument.object();
    databaseJsonArrayList = databaseJsonObject["database"].toArray();
    // Determine maximum page number
    booksNumber = databaseJsonArrayList.size();

    if(folderFeatureEnabled == false) {
        pagesNumber = std::ceil((double)booksNumber / buttonsNumber);
    }
    else {
        calculateMaxPagesForFolders();
    }

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
            bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    // This varies if books are there
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
    log("Clicked on previous button", className);

    if(folderFeatureEnabled == true) {
        log("Decreasing bookIndexVector by buttonsNumber", className);
        bookIndexVector = (bookIndexVector - goBackInIndex) - buttonsNumber;
        if(bookIndexVector < 0) {
            log("ERROR: Shouldn't be possible to get here", className);
            bookIndexVector = 0;
        }
    }

    currentPageNumber--;
    chooseBookListSetup(currentPageNumber);

    pagesTurned = pagesTurned + 1;
    if(pagesTurned >= 3) {
        emit refreshScreen();
        pagesTurned = 0;
    }
}


void localLibraryWidget::on_nextPageBtn_clicked()
{
    currentPageNumber++;
    chooseBookListSetup(currentPageNumber);

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
    log("Received a click on buttons vector, buttonNumber is: " + QString::number(buttonNumber), className);
    int id = idList.at(buttonNumber - 1);
    if(id == 99990) {
        if(folderFeatureEnabled == true) {
            log("Clicked a folder", className);
            QString dirToGo = bookBtnArray[buttonNumber]->text();
            log("Dir to go is: " + dirToGo, className);
            changePathAndRefresh(dirToGo);
        }
        else {
            log("Error: how", className);
        }
    }
    else {
        log("Clicked a book", className);
        openBook(id);
        localLibraryWidget::close();
    }
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
        if(folderFeatureEnabled == true) {
            calcIndexForPage(page);
        }
        log("Going to page " + QString::number(page), className);
        chooseBookListSetup(page);
        emit refreshScreen();
    }
}

void localLibraryWidget::refreshScreenNative() {
    emit refreshScreen();
}

void localLibraryWidget::setupDisplay() {
    setupDatabase();
    if(noBooksInDatabase == false) {
        // Prevent segmentation fault if a book was the last of its page
        if(currentPageNumber > pagesNumber) {
            chooseBookListSetup(currentPageNumber - 1);
        }
        else {
            chooseBookListSetup(currentPageNumber);
        }
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

void localLibraryWidget::chooseBookListSetup(int pageNumber) {
    log("Choosed folders setup", className);
    setupBooksListFolders(pageNumber);
}

void localLibraryWidget::setupBooksListFolders(int pageNumber) {
    log("Showing local library but with folders", className);

    QStringList dirList = QDir(pathForFolders).entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    log("Dir list full: "+ dirList.join(","), className);

    // This part is calculating which folders to show per this page
    QStringList dirListFront = dirList;
    QStringList dirListBack = dirList;
    int pageNumberAbove = pageNumber;
    while(pageNumberAbove != 1) {
        for (int i = 0; i < buttonsNumber; ++i) {
            if(dirListFront.isEmpty() == false) {
                dirListFront.removeFirst();
            }
        }
        pageNumberAbove = pageNumberAbove - 1;
    }
    log("Dir list front: " + dirListFront.join(","), className);

    int aboveRemove = pageNumber * buttonsNumber;
    if(dirListBack.count() > aboveRemove) {
        while(dirListBack.count() > aboveRemove) {
            if(dirListBack.isEmpty() == false) {
                dirListBack.removeLast();
            }
        }
    }
    log("Dir list back: " + dirListBack.join(","), className);

    QStringList dirListPure;
    for(QString dir: dirListFront) {
        if(dirListBack.contains(dir) == true) {
            dirListPure.append(dir);
        }
    }
    log("Final pure dir list:" + dirListPure.join(","), className);

    idList.clear();
    int in = 1;
    int dirCount = 0; // I want to start at 0
    goBackInIndex = 0;
    for(int i = (1 * pageNumber * buttonsNumber) - (buttonsNumber - 1); i <= (1 * pageNumber * buttonsNumber); i++) {
        if(dirListPure.count() != dirCount) {
            // Insert a folder if here
            log("Showing a folder for index: " + QString::number(i), className);

            // Show it, its maybe hided
            bookIconArray[in]->show();
            bookBtnArray[in]->show();
            if(buttonsNumber - in > 0) {
                lineArray[in]->show();
            }

            bookBtnArray[in]->setText(dirListPure.at(dirCount));
            QPixmap pixmap("://resources/folder.png");
            bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio));

            // Id for indicating its a folder in btnOpenBook
            idList.append(99990);
            dirCount = dirCount + 1;
            in++;
        }
        else {
            // Show a book otherwise
            log("bookIndexVector used: " + QString::number(bookIndexVector), className);


            log("Showing a book for index: " + QString::number(i), className);
            // Read database info for each book and display the corresponding information on each button
            bookIconArray[in]->show();
            bookBtnArray[in]->show();
            if(buttonsNumber - in > 0) {
                lineArray[in]->show();
            }

            // If a book is missing, its propably because of >= idk
            if(bookIndexVector >= bookListForPathIndex.count()) {
                continue;
            }

            QJsonObject jsonObject = databaseJsonArrayList.at(bookListForPathIndex.at(bookIndexVector)).toObject();
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
                bookIconArray[in]->setPixmap(pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
            bookIndexVector = bookIndexVector + 1;
        }
        goBackInIndex = goBackInIndex + 1;
    }

    if(in <= buttonsNumber) {
        for(int i = in; i <= buttonsNumber; i++) {
            log("Hiding items in LocalLibrary", className);
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

// Sets pagesNumber for folders feature, and everything else
void localLibraryWidget::calculateMaxPagesForFolders() {
    log("Main path is: " + pathForFolders, className);

    fileListCount = QDir(pathForFolders).entryList(QDir::Files, QDir::Name).count();
    log("Files count in dir: " + QString::number(fileListCount), className);
    dirListCount = QDir(pathForFolders).entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name).count();
    log("Dirs count in dir: " + QString::number(dirListCount), className);
    completeListOfItems = fileListCount + dirListCount;
    log("All items: " + QString::number(completeListOfItems), className);

    pagesNumber = std::ceil((double)completeListOfItems / buttonsNumber);

    log("Total pages: " + QString::number(pagesNumber), className);

    // This is the last page full of folders
    firstPageForBooks = (QString::number(float(dirListCount) / buttonsNumber).split(".").first()).toInt();
    log("There are so many pages with folders: " + QString::number(firstPageForBooks), className);

    // This indicates how much folders are after firstPageForBooks. its always less than buttonsNumber
    lastPageFolderCount = dirListCount;
    while(lastPageFolderCount >= buttonsNumber) {
        lastPageFolderCount = lastPageFolderCount - buttonsNumber;
    }
    log("Start after item on the last page: " + QString::number(lastPageFolderCount), className);

    // Look for books in this path
    bookListForPathIndex.clear();
    int count = 0;
    for(QJsonValue object: databaseJsonArrayList) {
        // This syntax...
        QString bookPath = object.toObject()["BookPath"].toString();
        QString bookDirPath = QFileInfo(bookPath).absoluteDir().path();

        if(bookDirPath.at(bookDirPath.count() - 1) == "/") {
            //log("bookDirPath last character is /, removing it");
            bookDirPath = bookDirPath.remove(bookDirPath.count() - 1, 1);
        }
        //log("bookDirPath is: " + bookDirPath, className);

        QString temporaryPathForFolders = pathForFolders;
        if(temporaryPathForFolders.at(temporaryPathForFolders.count() - 1) == "/") {
            log("temporaryPathForFolders last character is /, removing it");
            temporaryPathForFolders = temporaryPathForFolders.remove(temporaryPathForFolders.count() - 1, 1);
        }
        //log("temporaryPathForFolders is: " + temporaryPathForFolders, className);

        if(bookDirPath == temporaryPathForFolders) {
            //log("Found book in folder, adding it to vector", className);
            bookListForPathIndex.append(count);
        }
        count = count + 1;
    }
    QStringList list;
    foreach (int number, bookListForPathIndex) {
        list.append(QString::number(number));
    }
    log("bookListForPathIndex is: " + list.join(","), className);

    // Sorting the vector if needed should be done here
}

// Don't touch this code. This is a warning.
// If this code changes, and something with changing pages by keypad, or changing dirs breaks, I won't help debug it. good luck
// ~Szybet
void localLibraryWidget::calcIndexForPage(int pageNumber) {
    if(pageNumber == firstPageForBooks + 1) {
        log("Variable firstPageForBooks is 0", className);
        bookIndexVector = 0;
        return void();
    }

    if(pageNumber == firstPageForBooks + 2) {
        log("The page is just after firstPageForBooks", className);
        bookIndexVector = buttonsNumber - lastPageFolderCount;
        return void();
    }
    else if(pageNumber > firstPageForBooks + 2) {
        bookIndexVector = buttonsNumber - lastPageFolderCount;
        int pageTillGoal = 1;
        int pageDiffrence = pageNumber - (firstPageForBooks + 2);
        log("Variable firstPageForBooks is: " + QString::number(firstPageForBooks), className);
        log("Variable pageDiffrence is: " + QString::number(pageDiffrence), className);
        if(pageDiffrence != 0) {
            while(pageTillGoal != pageDiffrence) {
                log("Looping for pageTillGoal");
                bookIndexVector = bookIndexVector + buttonsNumber;
                pageTillGoal = pageTillGoal + 1;
            }
        }
        bookIndexVector = bookIndexVector + buttonsNumber;
        log("Calculated bookIndexVector is: " + QString::number(bookIndexVector), className);
    }
    else {
        log("Variable pageNumber isin't Higher than firstPageForBooks", className);
        bookIndexVector = 0;
    }
}

void localLibraryWidget::changePathAndRefresh(QString directory) {
    log("Changing path", className);
    QString temporaryPathForFolders = pathForFolders + directory + "/";
    if(QDir(temporaryPathForFolders).isEmpty() == false) {
        pathForFolders = temporaryPathForFolders;
        calculateMaxPagesForFolders();
        bookIndexVector = 0;
        goToPage(1);
    }
    else {
        showToast("Directory is empty");
    }
}
