#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H

#include <QWidget>
#include <toast.h>

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
    Ui::libraryWidget * ui;
    toast * toastWindow;

private slots:
    void setupView();
    void showToast(QString messageToDisplay);

};

#endif // LIBRARYWIDGET_H
