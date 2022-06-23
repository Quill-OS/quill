#ifndef LOCALLIBRARYWIDGET_H
#define LOCALLIBRARYWIDGET_H

#include <QWidget>
#include <QLabel>

#include "functions.h"
#include "qclickablelabel.h"

namespace Ui {
class localLibraryWidget;
}

class localLibraryWidget : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit localLibraryWidget(QWidget *parent = nullptr);
    ~localLibraryWidget();
    int buttonsNumber;
    int sW;
    int sH;
    int stdIconWidth;
    int stdIconHeight;

private slots:
    void setupBooksList();

private:
    Ui::localLibraryWidget * ui;
    QLabel * bookIconArray[10];
    QClickableLabel * bookBtnArray[10];
};

#endif // LOCALLIBRARYWIDGET_H
