#ifndef EGG_H
#define EGG_H

#include <QDialog>
#include <QMessageBox>
#include <QGraphicsScene>

#include "functions.h"

namespace Ui {
class egg;
}

class egg : public QDialog
{
    Q_OBJECT

public:
    explicit egg(QWidget *parent = nullptr);
    ~egg();
    int index = 0;
    const int maximumIndex = 3;
    bool firstRun = true;

private slots:
    void on_previousBtn_clicked();
    void on_nextBtn_clicked();
    void changeIndex(int index);
    void on_quitBtn_clicked();

private:
    Ui::egg *ui;
    QGraphicsScene * graphicsScene;
};

#endif // EGG_H
