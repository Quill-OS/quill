#ifndef LOCALLIBRARYWIDGET_H
#define LOCALLIBRARYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "functions.h"
#include "qclickablelabel.h"
#include "generaldialog.h"
#include "toast.h"
#include "bookoptionsdialog.h"
#include "bookinfodialog.h"

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
    float stdIconWidthDivider;
    float stdIconHeightDivider;
    int stdIconWidth;
    int stdIconHeight;
    QJsonDocument databaseJsonDocument;
    QJsonObject databaseJsonObject;
    QJsonArray databaseJsonArrayList;
    int currentPageNumber = 1;
    int pagesNumber;
    int pagesTurned = 0;
    int booksNumber;
    int bookTitleTruncateThreshold;
    QVector<int> idList;
    bool noBooksInDatabase = true;

private slots:
    void setupDatabase();
    void setupBooksList(int pageNumber);
    void on_previousPageBtn_clicked();
    void on_nextPageBtn_clicked();
    void openBook(int id);
    void btnOpenBook(int buttonNumber);
    void refreshScreenNative();
    void openGoToPageDialog();
    void goToPage(int page);
    void setupDisplay();
    void showToast(QString messageToDisplay);
    void openBookOptionsDialog(int pseudoBookID);
    void handlePossibleBookDeletion();
    void openLocalBookInfoDialog();

    void setupBooksListFolders(int pageNumber);
    void chooseBookListSetup(int pageNumber);
    void calculateMaxPagesForFolders();

private:
    Ui::localLibraryWidget * ui;
    generalDialog * generalDialogWindow;
    bookOptionsDialog * bookOptionsDialogWindow;
    toast * toastWindow;
    bookInfoDialog * bookInfoDialogWindow;
    QVector<QHBoxLayout*> horizontalLayoutArray;
    QVector<QLabel*> bookIconArray;
    QVector<QClickableLabel*> bookBtnArray;
    QVector<QFrame*> lineArray;

    QString pathForFolders = "/mnt/onboard/onboard/";
    bool folderFeatureEnabled = true;
    // For folder feature
    int firstPageForBooks;

signals:
    void openBookSignal(QString bookFile, bool relativePath);
    void refreshScreen();
};

#endif // LOCALLIBRARYWIDGET_H
