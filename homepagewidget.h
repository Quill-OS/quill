#ifndef HOMEPAGEWIDGET_H
#define HOMEPAGEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "qclickablelabel.h"
#include "qtooltiplabel.h"

namespace Ui {
class homePageWidget;
}

class homePageWidget : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit homePageWidget(QWidget *parent = nullptr);
    ~homePageWidget();
    int sW;
    int sH;
    int stdIconWidth;
    int stdIconHeight;
    float stdIconWidthDivider;
    float stdIconHeightDivider;
    int bookTitleTruncateThreshold;

signals:
    void openBookSignal(QString bookPath, bool relativePath);
    void refreshScreen();

private slots:
    void openBook(QString bookPath);
    void refreshScreenNative();

private:
    Ui::homePageWidget *ui;
    QVector<QToolTipLabel*> bookTitleArray;
    QVector<QHBoxLayout*> horizontalLayoutArray;
    QVector<QVBoxLayout*> verticalLayoutArray;
    QVector<QClickableLabel*> bookBtnArray;
};

#endif // HOMEPAGEWIDGET_H
