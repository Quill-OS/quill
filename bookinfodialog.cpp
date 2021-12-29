#include "bookinfodialog.h"
#include "ui_bookinfodialog.h"
#include "functions.h"

#include <QScreen>

bookInfoDialog::bookInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookInfoDialog)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
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

    if(global::library::isLatestBook == true) {
        QString bookNumberQstr = QString::number(global::library::latestBookNumber);
        QString coverPath = "/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/";
        coverPath = coverPath.append(bookNumberQstr);
        coverPath = coverPath.append("/cover.jpg");

        QPixmap coverPixmap(coverPath);
        QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
        global::library::isLatestBook = false;

        ui->bookTitleLabel->setText(global::library::bookTitle);
        global::library::bookTitle = "";
    }
    else {
        ui->bookTitleLabel->setText(global::library::bookTitle);
        global::library::bookTitle = "";

        QDir dictdir;
        dictdir.mkpath("/inkbox/gutenberg");
        string_writeconfig("/inkbox/gutenberg/bookid", QString::number(global::library::bookId).toStdString());
        string_writeconfig("/opt/ibxd", "gutenberg_get_cover\n");
        while(true) {
            if(QFile::exists("/inkbox/gutenberg/getCoverDone")) {
                if(checkconfig("/inkbox/gutenberg/getCoverDone") == true) {
                    QPixmap coverPixmap("/inkbox/gutenberg/book_cover.jpg");
                    QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
                    ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
                    QFile::remove("/inkbox/gutenberg/getCoverDone");
                    break;
                }
                else {
                    QPixmap coverPixmap(":/resources/cover_unavailable.png");
                    QPixmap scaledCoverPixmap = coverPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
                    ui->bookCoverLabel->setPixmap(scaledCoverPixmap);
                    QFile::remove("/inkbox/gutenberg/getCoverDone");
                    break;
                }
            }
        }
    }

    // Centering dialog
    this->adjustSize();
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
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
    bookInfoDialog::close();
}

