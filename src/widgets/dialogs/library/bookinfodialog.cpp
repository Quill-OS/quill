#include "bookinfodialog.h"
#include "ui_bookinfodialog.h"
#include "functions.h"

#include <QScreen>

bookInfoDialog::bookInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookInfoDialog)
{
    ui->setupUi(this);

    // Preventing outside interaction
    this->setModal(true);

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->closeBtn->setProperty("type", "borderless");
    ui->closeBtn->setText("");
    ui->closeBtn->setIcon(QIcon(":/resources/close.png"));
    ui->bookCoverLabel->setText("");
    ui->getBtn->setStyleSheet("background: lightGrey; font-size: 9pt; padding: 8px");

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Setting icons up
    stdIconWidth = sW / 4;
    stdIconHeight = sH / 4;

    if(global::bookInfoDialog::localInfoDialog == false) {
        if(global::library::isLatestBook == true) {
            QString bookNumberQstr = QString::number(global::library::latestBookNumber);

            QString coverPath = "/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/";
            coverPath = coverPath.append(bookNumberQstr);
            coverPath = coverPath.append("/cover.jpg");

            QString idPath = "/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/";
            idPath = idPath.append(bookNumberQstr);
            idPath = idPath.append("/id");
            global::library::bookId = readFile(idPath).toULong();

            QPixmap coverPixmap(coverPath);
            QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
            global::library::isLatestBook = false;

            ui->bookTitleLabel->setText(global::library::bookTitle);
        }
        else {
            ui->bookTitleLabel->setText(global::library::bookTitle);

            QDir gutenbergDir;
            gutenbergDir.mkpath("/inkbox/gutenberg");
            writeFile("/inkbox/gutenberg/bookid", QString::number(global::library::bookId));
            writeFile("/opt/ibxd", "gutenberg_get_cover\n");
            if(waitForStatusFile("/inkbox/gutenberg/getCoverDone")) {
                QPixmap coverPixmap("/inkbox/gutenberg/book_cover.jpg");
                QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
            }
            else {
                QPixmap coverPixmap(":/resources/cover_unavailable.png");
                QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
            }
        }

        log("Setting up book info dialog, ID: " + QString::number(global::library::bookId) + ", title: " + global::library::bookTitle, className);
    }
    else {
        if(global::localLibrary::bookOptionsDialog::bookID != global::localLibrary::folderID) {
            QJsonObject bookJsonObject = getBookMetadata(global::localLibrary::bookOptionsDialog::bookID);
            QString bookInfo;
            QString title = bookJsonObject["Title"].toString();
            QString author = bookJsonObject["Author"].toString();
            QString publicationDate = bookJsonObject["PublicationDate"].toString();
            QString path = bookJsonObject["BookPath"].toString();
            if(!title.isEmpty()) {
                bookInfo.append("<b>Title:</b> " + title + "<br>");
            }
            if(!author.isEmpty()) {
                bookInfo.append("<b>Author:</b> " + author + "<br>");
            }
            if(!publicationDate.isEmpty()) {
                bookInfo.append("<b>Publication date:</b> " + publicationDate + "<br>");
            }
            if(!path.isEmpty()) {
                bookInfo.append("<b>Path:</b> " + path + "<br>");
            }
            global::text::textBrowserContents = bookInfo;
        }
        else {
            QString bookInfo = "<b>Path:</b> " + global::localLibrary::bookOptionsDialog::folderPath;
            global::text::textBrowserContents = bookInfo;
        }
        textwidget * textwidgetWindow = new textwidget(this);
        ui->stackedWidget->insertWidget(1, textwidgetWindow);
        ui->stackedWidget->setCurrentIndex(1);
    }

    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    {
        int wx = screenGeometry.width();

        int x = wx - 25;
        int y = this->height() * determineYIncrease();
        this->setFixedWidth(x);
        this->setFixedHeight(y);

        this->adjustSize();
    }

    // Centering dialog
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

bookInfoDialog::~bookInfoDialog()
{
    delete ui;
}

void bookInfoDialog::on_closeBtn_clicked()
{
    global::library::bookTitle = "";
    global::bookInfoDialog::localInfoDialog = false;
    bookInfoDialog::close();
}


void bookInfoDialog::on_getBtn_clicked()
{
    QDir gutenbergDir;
    gutenbergDir.mkpath("/inkbox/gutenberg");
    writeFile("/inkbox/gutenberg/bookid", QString::number(global::library::bookId));
    writeFile("/inkbox/gutenberg/booktitle", global::library::bookTitle);
    writeFile("/opt/ibxd", "gutenberg_get_book\n");

    global::toast::modalToast = true;
    global::toast::indefiniteToast = true;
    log("Downloading book, ID: " + QString::number(global::library::bookId) + ", title: " + global::library::bookTitle, className);
    emit showToast("Downloading");

    QTimer::singleShot(500, this, SLOT(waitForBookFetch()));
}

void bookInfoDialog::waitForBookFetch() {
    if(waitForStatusFile("/inkbox/gutenberg/getBookDone")) {
        emit closeIndefiniteToast();
        QString function = __func__; log(function + ": Download successful", className);
        emit showToast("Download successful");
        QFile::remove(global::localLibrary::databasePath);
    }
    else {
        emit closeIndefiniteToast();
        QString function = __func__; log(function + ": Download failed", className);
        emit showToast("Download failed");
    }
}
