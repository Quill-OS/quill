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
}

void bookOptionsDialog::on_deleteBtn_clicked()
{
    log("Deleting book '" + bookPath + "'", className);
    QFile::remove(bookPath);
    global::localLibrary::bookOptionsDialog::bookDeleted = true;
    QFile::remove(global::localLibrary::databasePath);
    ui->deleteBtn->setEnabled(false);
    ui->deleteBtn->setStyleSheet("padding: 10px; color: lightGrey");
}


void bookOptionsDialog::on_infoBtn_clicked()
{
    // TODO
}

