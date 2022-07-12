#ifndef BOOKOPTIONSDIALOG_H
#define BOOKOPTIONSDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class bookOptionsDialog;
}

class bookOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit bookOptionsDialog(QWidget *parent = nullptr);
    ~bookOptionsDialog();
    QString bookPath;

private slots:
    void on_pinBtn_clicked();
    void on_deleteBtn_clicked();
    void on_infoBtn_clicked();

signals:
    void openLocalBookInfoDialog();

private:
    Ui::bookOptionsDialog *ui;
};

#endif // BOOKOPTIONSDIALOG_H
