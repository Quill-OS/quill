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

private slots:
    void on_closeBtn_clicked();
    void on_newListBtn_clicked();
    void createNewList(QString listName);
    void refreshList();
    void setupList(QString listName);
    void on_setupBtn_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::todo *ui;
    virtualkeyboard * virtualKeyboardWidget;
};

#endif // TODO_H
