#include "librarywidget.h"
#include "ui_librarywidget.h"
#include "functions.h"

#include <QProcess>
#include <QDebug>
#include <QDateTime>
#include <QScreen>
#include <QFontDatabase>

libraryWidget::libraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::libraryWidget)
{
    ui->setupUi(this);
    ui->booksStackedWidget->hide();

    QString family;
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Regular.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Italic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Bold.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-BoldItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    QFont crimson(family);

    ui->previousBtn->setText("");
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setText("");
    ui->nextBtn->setProperty("type", "borderless");
    ui->previousBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextBtn->setIcon(QIcon(":/resources/chevron-right.png"));
    ui->pageLabel->setText("1 <i>of</i> 2");
    ui->pageLabel->setFont(QFont("Source Serif Pro"));

    ui->book1Btn->setText("");
    ui->book2Btn->setText("");
    ui->book3Btn->setText("");
    ui->book4Btn->setText("");
    ui->book5Btn->setText("");
    ui->book6Btn->setText("");
    ui->book7Btn->setText("");
    ui->book8Btn->setText("");
    ui->book9Btn->setText("");
    ui->book10Btn->setText("");
    ui->book11Btn->setText("");
    ui->book12Btn->setText("");
    ui->book13Btn->setText("");
    ui->book14Btn->setText("");
    ui->book15Btn->setText("");
    ui->book16Btn->setText("");
    ui->book1Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book2Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book3Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book4Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book5Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book6Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book7Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book8Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book9Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book10Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book11Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book12Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book13Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book14Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book15Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book16Label->setStyleSheet("font-size: 8.5pt; font-style: italic");
    ui->book1Label->setFont(crimson);
    ui->book2Label->setFont(crimson);
    ui->book3Label->setFont(crimson);
    ui->book4Label->setFont(crimson);
    ui->book5Label->setFont(crimson);
    ui->book6Label->setFont(crimson);
    ui->book7Label->setFont(crimson);
    ui->book8Label->setFont(crimson);
    ui->book9Label->setFont(crimson);
    ui->book10Label->setFont(crimson);
    ui->book11Label->setFont(crimson);
    ui->book12Label->setFont(crimson);
    ui->book13Label->setFont(crimson);
    ui->book14Label->setFont(crimson);
    ui->book15Label->setFont(crimson);
    ui->book16Label->setFont(crimson);

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Prevent abusive sync
    string_checkconfig_ro("/external_root/opt/storage/gutenberg/last_sync");
    if(!checkconfig_str_val.isEmpty()) {
        unsigned long currentEpoch = QDateTime::currentSecsSinceEpoch();
        unsigned long syncEpoch = checkconfig_str_val.toULong();
        unsigned long allowSyncEpoch = syncEpoch + 86400;
        if(currentEpoch > allowSyncEpoch) {
            syncCatalog();
        }
        else {
            setupView();
        }
    }
    else {
        syncCatalog();
    }
}

libraryWidget::~libraryWidget()
{
    delete ui;
}

void libraryWidget::setupView() {
    ui->booksStackedWidget->show();

    stdIconWidth = sW / 5.5;
    stdIconHeight = sH / 5.5;

    ui->book1Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/1/cover.jpg"));
    ui->book1Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book2Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/2/cover.jpg"));
    ui->book2Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book3Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/3/cover.jpg"));
    ui->book3Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book4Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/4/cover.jpg"));
    ui->book4Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book5Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/5/cover.jpg"));
    ui->book5Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book6Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/6/cover.jpg"));
    ui->book6Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book7Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/7/cover.jpg"));
    ui->book7Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book8Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/8/cover.jpg"));
    ui->book8Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book9Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/9/cover.jpg"));
    ui->book9Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book10Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/10/cover.jpg"));
    ui->book10Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book11Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/11/cover.jpg"));
    ui->book11Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book12Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/12/cover.jpg"));
    ui->book12Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book13Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/13/cover.jpg"));
    ui->book13Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book14Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/14/cover.jpg"));
    ui->book14Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book15Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/15/cover.jpg"));
    ui->book15Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));
    ui->book16Btn->setIcon(QIcon("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/16/cover.jpg"));
    ui->book16Btn->setIconSize(QSize(stdIconWidth, stdIconHeight));

    ui->book1Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/1/title"));
    ui->book2Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/2/title"));
    ui->book3Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/3/title"));
    ui->book4Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/4/title"));
    ui->book5Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/5/title"));
    ui->book6Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/6/title"));
    ui->book7Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/7/title"));
    ui->book8Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/8/title"));
    ui->book9Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/9/title"));
    ui->book10Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/10/title"));
    ui->book11Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/11/title"));
    ui->book12Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/12/title"));
    ui->book13Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/13/title"));
    ui->book14Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/14/title"));
    ui->book15Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/15/title"));
    ui->book16Label->setText(readFile("/mnt/onboard/onboard/.inkbox/gutenberg-data/latest-books/16/title"));
}

void libraryWidget::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    toastWindow->show();
}

void libraryWidget::syncCatalog() {
    global::toast::modalToast = true;
    global::toast::indefiniteToast = true;
    showToast("Sync in progress");

    string_writeconfig("/opt/ibxd", "gutenberg_sync\n");
    QTimer * syncCheckTimer = new QTimer(this);
    syncCheckTimer->setInterval(100);
    connect(syncCheckTimer, &QTimer::timeout, [&]() {
        if(QFile::exists("/inkbox/gutenbergSyncDone") == true) {
            if(checkconfig("/inkbox/gutenbergSyncDone") == true) {
                qDebug() << "Gutenberg sync successfully completed";
                toastWindow->close();
                setupView();
            }
            else {
                qDebug() << "Gutenberg sync encountered an error";
                toastWindow->close();
                showToast("Error");
                libraryWidget::close();
            }
            QFile::remove("/inkbox/gutenbergSyncDone");
        }
    } );
    syncCheckTimer->start();
}

void libraryWidget::on_previousBtn_clicked()
{
    ui->booksStackedWidget->setCurrentIndex(0);
    ui->pageLabel->setText("1 <i>of</i> 2");
}


void libraryWidget::on_nextBtn_clicked()
{
    ui->booksStackedWidget->setCurrentIndex(1);
    ui->pageLabel->setText("2 <i>of</i> 2");
}

