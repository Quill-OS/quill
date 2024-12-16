#ifndef FLASHEXAM_H
#define FLASHEXAM_H

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
class flashExam;
}

class flashExam : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit flashExam(QWidget *parent = nullptr);
    ~flashExam();

private:
    Ui::flashExam *ui;
    int cardsTotal;
    int currentCardNumber;
    QString listName;
    bool randomize;
    bool nonRedundantRandomization;
    bool saveCardsNotKnownToFile;
    bool brainBruteForceMode;
    bool brainBruteForceLock = false;
    int brainBruteForceCardsThreshold;
    QList<int> cardsAlreadyShown;
    QList<int> cardsNotKnown;
    bool answerShown = false;
    QStringList cardsStringList;
    QStringList answersStringList;
    void setupCardsList();
    void initCardsList(QString cardsList, QString answersList);
    void displayCard(bool existingCardNumber);
    QString displayImage(QString cardText);
    QGraphicsScene * graphicsScene;

signals:
    void showToast(QString messageToDisplay);
private slots:
    void on_closeBtn_clicked();
    void on_startBtn_clicked();
    void on_backBtn_clicked();
    void on_revealBtn_clicked();
    void on_nextBtn_clicked();
    void on_randomizeCheckBox_toggled(bool checked);
    void on_didNotKnowBtn_clicked();
    void on_nonRedundantRandomizationCheckBox_toggled(bool checked);
    void on_brainBruteForceCheckBox_toggled(bool checked);
};

#endif // FLASHEXAM_H
