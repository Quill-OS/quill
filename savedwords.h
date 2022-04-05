#ifndef SAVEDWORDS_H
#define SAVEDWORDS_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace Ui {
class savedwords;
}

class savedwords : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit savedwords(QWidget *parent = nullptr);
    ~savedwords();
    QString words;
    void checkwords();
    void save_word_init();

private slots:
    void on_backBtn_clicked();
    void on_clearBtn_clicked();

private:
    Ui::savedwords *ui;
};

#endif // SAVEDWORDS_H
