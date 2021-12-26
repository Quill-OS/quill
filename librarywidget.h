#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H

#include <QWidget>

namespace Ui {
class libraryWidget;
}

class libraryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit libraryWidget(QWidget *parent = nullptr);
    ~libraryWidget();

private:
    Ui::libraryWidget *ui;
};

#endif // LIBRARYWIDGET_H
