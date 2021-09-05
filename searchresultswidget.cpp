#include "searchresultswidget.h"
#include "ui_searchresultswidget.h"

#include <QMessageBox>

searchResultsWidget::searchResultsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchResultsWidget)
{
    ui->setupUi(this);
    ui->listView->setStyleSheet("font-size: 10pt");
    ui->backBtn->setProperty("type", "borderless");
    ui->backBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->openBtn->setProperty("type", "borderless");
    ui->openBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
}

searchResultsWidget::~searchResultsWidget()
{
    delete ui;
}

void searchResultsWidget::setListViewContents(QStringList searchResults) {
    QStringListModel * model = new QStringListModel(this);
    model->setStringList(searchResults);
    ui->listView->setModel(model);
}

void searchResultsWidget::on_openBtn_clicked()
{
    index = ui->listView->currentIndex();
    itemText = index.data(Qt::DisplayRole).toString();
    if(!itemText.isEmpty()) {
        emit openBookFile(itemText);
        searchResultsWidget::close();
    }
    else {
        QMessageBox::critical(this, tr("Invalid argument"), tr("Please select a search result."));
    }
}


void searchResultsWidget::on_backBtn_clicked()
{
    searchResultsWidget::close();
}

