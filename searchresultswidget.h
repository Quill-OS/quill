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
    QModelIndex index;
    QString itemText;

private slots:
    void on_openBtn_clicked();
    void on_backBtn_clicked();

private:
    Ui::searchResultsWidget *ui;

signals:
    void openBookFile(QString book, bool relativePath);
};

#endif // SEARCHRESULTSWIDGET_H
