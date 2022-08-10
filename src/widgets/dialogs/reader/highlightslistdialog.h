#ifndef HIGHLIGHTSLISTDIALOG_H
#define HIGHLIGHTSLISTDIALOG_H

#include <QDialog>

#include "functions.h"

namespace Ui {
class highlightsListDialog;
}

class highlightsListDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit highlightsListDialog(QWidget *parent = nullptr);
    ~highlightsListDialog();

private slots:
    void increaseSize();
    void on_cancelBtn_clicked();
    void on_okBtn_clicked();
    void on_selectAllBtn_clicked();

signals:
    void showToast(QString messageToDisplay);

private:
    Ui::highlightsListDialog *ui;
};

#endif // HIGHLIGHTSLISTDIALOG_H
