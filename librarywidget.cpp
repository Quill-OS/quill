#include "librarywidget.h"
#include "ui_librarywidget.h"
#include "functions.h"

#include <QProcess>
#include <QDebug>

libraryWidget::libraryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::libraryWidget)
{
    ui->setupUi(this);
    ui->book1Btn->setText("");
    ui->book2Btn->setText("");
    ui->book3Btn->setText("");
    ui->book4Btn->setText("");

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

libraryWidget::~libraryWidget()
{
    delete ui;
}

void libraryWidget::setupView() {

}

void libraryWidget::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    toastWindow->show();
}
