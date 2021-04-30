#ifndef KOBOXAPPSDIALOG_H
#define KOBOXAPPSDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class koboxAppsDialog;
}

class koboxAppsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit koboxAppsDialog(QWidget *parent = nullptr);
    ~koboxAppsDialog();

    void checkApps();

    QString apps;
    QString itemText;
    QModelIndex index;

private slots:
    void on_cancelBtn_clicked();
    void on_launchBtn_clicked();

private:
    Ui::koboxAppsDialog *ui;
};

#endif // KOBOXAPPSDIALOG_H
