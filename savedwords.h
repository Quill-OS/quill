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
    explicit savedwords(QWidget *parent = nullptr);
    ~savedwords();
    QString words;
    void checkwords() {
        QFile words_list(".config/06-words/config");
        words_list.open(QIODevice::ReadWrite);
        QTextStream in (&words_list);
        words = in.readAll();
        words_list.close();
    }
    void save_word_init(string word) {
        QFile words(".config/06-words/config");
        words.open(QIODevice::ReadWrite);
        QTextStream in (&words);
        QString words_list = in.readAll();
        string words_list_str = words_list.toStdString();
        words.close();

        ofstream fhandler;
        fhandler.open(".config/06-words/config");
        fhandler << "";
        fhandler.close();
    }

private slots:
    void on_backBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::savedwords *ui;
};

#endif // SAVEDWORDS_H
