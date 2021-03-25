#ifndef CALENDARAPP_H
#define CALENDARAPP_H

#include <QWidget>

namespace Ui {
class calendarApp;
}

class calendarApp : public QWidget
{
    Q_OBJECT

public:
    explicit calendarApp(QWidget *parent = nullptr);
    ~calendarApp();

private slots:
    void on_backBtn_clicked();

private:
    Ui::calendarApp *ui;
};

#endif // CALENDARAPP_H
