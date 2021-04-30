#ifndef KOBOXAPPSDIALOG_H
#define KOBOXAPPSDIALOG_H

#include <QWidget>

namespace Ui {
class koboxAppsDialog;
}

class koboxAppsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit koboxAppsDialog(QWidget *parent = nullptr);
    ~koboxAppsDialog();

    void checkApps();

    QString apps;

private:
    Ui::koboxAppsDialog *ui;
};

#endif // KOBOXAPPSDIALOG_H
