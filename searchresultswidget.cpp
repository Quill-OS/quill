#include "searchresultswidget.h"
#include "ui_searchresultswidget.h"

#include <QDebug>

searchResultsWidget::searchResultsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::searchResultsWidget)
{
    ui->setupUi(this);
    ui->listView->setStyleSheet("font-size: 9pt");
    ui->backBtn->setProperty("type", "borderless");
    ui->backBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
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
