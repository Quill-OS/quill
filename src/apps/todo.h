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
    bool selectItemsMode = false;
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
    void switchViewHome();
    void switchItemsSelectionMode(bool selectionMode);
    void setDefaultHomePageStatusText();
    void setDefaultListPageStatusText();
    void on_setupBtn_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_deleteBtn_clicked();
    void on_newItemBtn_clicked();
    void on_selectItemsModeBtn_clicked();
    void on_deleteItemBtn_clicked();
    void on_selectAllItemsBtn_clicked();
    void on_deselectAllItemsBtn_clicked();
    void on_saveCurrentListViewBtn_clicked();

signals:
    void showToast(QString messageToDisplay);

private:
    Ui::todo *ui;
    virtualkeyboard * virtualKeyboardWidget;
};

#endif // TODO_H
