#ifndef USERAPP_H
#define USERAPP_H

#include <QWidget>

namespace Ui {
class userapp;
}

class userapp : public QWidget
{
    Q_OBJECT

public:
    explicit userapp(QWidget *parent = nullptr);
    ~userapp();

private:
    Ui::userapp *ui;
};

#endif // USERAPP_H
