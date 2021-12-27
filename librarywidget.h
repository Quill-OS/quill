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
    float sH;
    float sW;
    float stdIconHeight;
    float stdIconWidth;

private:
    Ui::libraryWidget * ui;
    toast * toastWindow;

private slots:
    void syncCatalog();
    void setupView();
    void showToast(QString messageToDisplay);

    void on_previousBtn_clicked();
    void on_nextBtn_clicked();
};

#endif // LIBRARYWIDGET_H
