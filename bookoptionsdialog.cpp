#include "bookoptionsdialog.h"
#include "ui_bookoptionsdialog.h"

#include "functions.h"

bookOptionsDialog::bookOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookOptionsDialog)
{
    ui->setupUi(this);
    ui->pinBtn->setProperty("type", "borderless");
    ui->deleteBtn->setProperty("type", "borderless");
    ui->infoBtn->setProperty("type", "borderless");

    bookPath = getBookMetadata(global::localLibrary::bookOptionsDialog::bookID)["BookPath"].toString();
}

bookOptionsDialog::~bookOptionsDialog()
{
    delete ui;
}

void bookOptionsDialog::on_pinBtn_clicked()
{
    // TODO
    log("Pinned book with ID " + QString::number(global::localLibrary::bookOptionsDialog::bookID), className);
}

void bookOptionsDialog::on_deleteBtn_clicked()
{
    log("Deleting book '" + bookPath + "'", className);
    global::toast::delay = 3000;
    if(QFile::remove(bookPath)) {
        emit showToast("Book deleted successfully");
        global::localLibrary::bookOptionsDialog::bookDeleted = true;
        QFile::remove(global::localLibrary::databasePath);
    }
    else {
        emit showToast("Failed to delete book");
    }
}

void bookOptionsDialog::on_infoBtn_clicked()
{
    emit openLocalBookInfoDialog();
}
