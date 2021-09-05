#ifndef SEARCHRESULTSWIDGET_H
#define SEARCHRESULTSWIDGET_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class searchResultsWidget;
}

class searchResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit searchResultsWidget(QWidget *parent = nullptr);
    ~searchResultsWidget();
    void setListViewContents(QStringList searchResults);

private:
    Ui::searchResultsWidget *ui;
};

#endif // SEARCHRESULTSWIDGET_H
