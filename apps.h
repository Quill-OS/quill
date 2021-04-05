#ifndef APPS_H
#define APPS_H

#include <QWidget>
#include <savedwords.h>
#include <calendarapp.h>

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

private:
    Ui::apps *ui;
    savedwords *savedWordsWindow;
    calendarApp *calendarWindow;
};

#endif // APPS_H
