#ifndef LOCALLIBRARYWIDGET_H
#define LOCALLIBRARYWIDGET_H

#include <QWidget>

namespace Ui {
class localLibraryWidget;
}

class localLibraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit localLibraryWidget(QWidget *parent = nullptr);
    ~localLibraryWidget();

private:
    Ui::localLibraryWidget *ui;
};

#endif // LOCALLIBRARYWIDGET_H
