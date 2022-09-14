#ifndef TODO_H
#define TODO_H

#include <QWidget>
#include <QListWidgetItem>

#include "virtualkeyboard.h"

namespace Ui {
class todo;
}

class todo : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit todo(QWidget *parent = nullptr);
    ~todo();
    enum class currentView {
        home,
        list
    };
    currentView currentView = currentView::home;
    int listIndex;

private slots:
    void on_closeBtn_clicked();
    void on_newBtn_clicked();
    void createNewList(QString listName);
    void addItem(QString itemName);
    void refreshList();
    void saveCurrentList();
    void setupList(QString listName);
    void on_setupBtn_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_deleteBtn_clicked();

private:
    Ui::todo *ui;
    virtualkeyboard * virtualKeyboardWidget;
};

#endif // TODO_H
