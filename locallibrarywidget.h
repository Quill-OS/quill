#ifndef LOCALLIBRARYWIDGET_H
#define LOCALLIBRARYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

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
    QJsonDocument databaseJsonDocument;
    QJsonObject databaseJsonObject;
    QJsonArray databaseJsonArrayList;
    int currentPageNumber = 1;
    int pagesNumber;
    int booksNumber;
    QList<int> idList[4];

private slots:
    void setupDatabase();
    void setupBooksList(int pageNumber);
    void on_previousPageBtn_clicked();
    void on_nextPageBtn_clicked();
    void openBook(int id);
    void btnOpenBook(int buttonNumber);

private:
    Ui::localLibraryWidget * ui;
    QHBoxLayout * horizontalLayoutArray[4];
    QLabel * bookIconArray[4];
    QClickableLabel * bookBtnArray[4];
    QFrame * lineArray[3];

signals:
    void openBookSignal(QString bookFile, bool relativePath);
};

#endif // LOCALLIBRARYWIDGET_H
