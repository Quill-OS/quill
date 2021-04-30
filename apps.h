#ifndef APPS_H
#define APPS_H

#include <QWidget>
#include <savedwords.h>
#include <calendarapp.h>
#include <koboxappsdialog.h>
#include <generaldialog.h>

namespace Ui {
class apps;
}

class apps : public QWidget
{
    Q_OBJECT

public:
    explicit apps(QWidget *parent = nullptr);
    ~apps();

private slots:
    void on_scribbleLaunchBtn_clicked();
    void exitSlot();
    void on_lightmapsLaunchBtn_clicked();
    void on_savedWordsLaunchBtn_clicked();
    void on_calendarLaunchBtn_clicked();
    void on_calculatorLaunchBtn_clicked();

    void on_koboxAppsOpenButton_clicked();

private:
    Ui::apps *ui;
    savedwords *savedWordsWindow;
    calendarApp *calendarWindow;
    koboxAppsDialog *koboxAppsDialogWindow;
};

#endif // APPS_H
