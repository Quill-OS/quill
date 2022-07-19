#ifndef HOMEPAGEWIDGET_H
#define HOMEPAGEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "qclickablelabel.h"
#include "qtooltiplabel.h"
#include "locallibrarywidget.h"
#include "toast.h"
#include "bookoptionsdialog.h"
#include "bookinfodialog.h"

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
    bool pinnedBooksDatabaseEmpty = true;

signals:
    void openBookSignal(QString bookPath, bool relativePath);
    void refreshScreen();
    void relaunchHomePageWidget();

private slots:
    void openBook(QString bookPath);
    void refreshScreenNative();
    void setupDisplay(bool databaseGenerated);
    void setupDisplaySlot();
    void setupDisplayWithDatabase();
    void showToast(QString messageToDisplay);
    void openBookOptionsDialog(QString bookPath);
    void openLocalBookInfoDialog();
    void handlePossibleBookPin();

private:
    Ui::homePageWidget *ui;
    localLibraryWidget * localLibraryWidgetWindow;
    toast * toastWindow;
    bookOptionsDialog * bookOptionsDialogWindow;
    bookInfoDialog * bookInfoDialogWindow;
    QVector<QToolTipLabel*> bookTitleArray;
    QVector<QHBoxLayout*> horizontalLayoutArray;
    QVector<QVBoxLayout*> verticalLayoutArray;
    QVector<QClickableLabel*> bookBtnArray;
    QVector<QToolTipLabel*> pinnedBooksTitleArray;
    QVector<QHBoxLayout*> pinnedBooksHorizontalLayoutArray;
    QVector<QVBoxLayout*> pinnedBooksVerticalLayoutArray;
    QVector<QClickableLabel*> pinnedBooksBtnArray;
};

#endif // HOMEPAGEWIDGET_H
