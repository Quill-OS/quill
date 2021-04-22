#ifndef READER_H
#define READER_H

#include "functions.h"
#include "alert.h"
#include "generaldialog.h"

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QList>

using namespace std;

namespace Ui {
class reader;
}

class reader : public QWidget
{
    Q_OBJECT

public:
    int split_total;
    int split_files_number;
    int page_number;
    int dictionary_position = 1;
    int pagesTurned = 0;

    // -1 : Never refresh | 0 : Refresh every page | 1 : Refresh every 1 page. And so on...
    // Refresh every 3 pages is the default
    int pageRefreshSetting = 3;

    bool menubar_shown = false;
    bool selected_text_lock = false;
    bool nextdefinition_lock = false;
    bool is_epub = false;
    bool parser_ran = false;
    bool booktostr_ran = false;
    bool neverRefresh = false;
    bool wakeFromSleep = false;
    bool remount = true;
    QString book_1;
    QString book_2;
    QString book_3;
    QString book_4;
    QString ittext;
    QString book_file;
    bool batt_status;
    QString selected_text;
    QString word;
    QString words; // Saved words
    QString letter;
    string selected_text_str;
    QString definition;
    QPixmap scaledChargingPixmap;
    QPixmap scaledHalfPixmap;
    QPixmap scaledFullPixmap;
    QPixmap scaledEmptyPixmap;
    QList<QString> content;
    explicit reader(QWidget *parent = nullptr);
    ~reader();
    int setup_book(QString book, int i, bool run_parser);
    /*int setup_book(QString book, int i, bool run_parser) {
        // Parse ebook
        if(remount != false) {
            QString mount_prog ("sh");
            QStringList mount_args;
            mount_args << "split.sh";
            QProcess *mount_proc = new QProcess();
            mount_proc->start(mount_prog, mount_args);
            mount_proc->waitForFinished();
            remount = false;
        }
        else {
            string_writeconfig("/inkbox/remount", "false");
            QString mount_prog ("sh");
            QStringList mount_args;
            mount_args << "split.sh";
            QProcess *mount_proc = new QProcess();
            mount_proc->start(mount_prog, mount_args);
            mount_proc->waitForFinished();
        }

        if(booktostr_ran != true) {
            if(epub_file_match(book) == true) {
                // Parsing ePUBs with epub2txt, thanks to GitHub:kevinboone
                QString epubProg ("sh");
                QStringList epubArgs;
                epubArgs << "/mnt/onboard/.adds/inkbox/epub.sh" << book;
                QProcess *epubProc = new QProcess();
                epubProc->start(epubProg, epubArgs);
                epubProc->waitForFinished();

                is_epub = true;
                booktostr_ran = true;
            }
            else {
                // This is likely not an ePUB.
                // Copying book specified in the function call
                QFile::copy(book, "/inkbox/book/book.txt");

                is_epub = false;
                booktostr_ran = true;
            }
        }

        // Checking if the user has defined an option for the number of words per page; if not, then setting the default.
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        string_checkconfig(".config/07-words_number/config");
        if(checkconfig_str_val == "") {
            string_writeconfig(".config/07-words_number/config", "100");
            string_checkconfig(".config/07-words_number/config");
        }

        // Parsing file
        if(parser_ran != true) {
            if(is_epub == true) {
                QString parse_prog ("python3");
                QStringList parse_args;
                parse_args << "split.py" << checkconfig_str_val;
                QProcess *parse_proc = new QProcess();
                parse_proc->start(parse_prog, parse_args);
                parse_proc->waitForFinished();
                parser_ran = true;
            }
            else {
                QString parse_prog ("python3");
                QStringList parse_args;
                parse_args << "split-txt.py" << checkconfig_str_val;
                QProcess *parse_proc = new QProcess();
                parse_proc->start(parse_prog, parse_args);
                parse_proc->waitForFinished();
                parser_ran = true;
            }
        }

        // Changing current working directory
        QDir::setCurrent("/inkbox/book");

        // Reading file
        if(run_parser == true) {
            QDirIterator it("/inkbox/book/split");
            while (it.hasNext()) {
                  QFile f(it.next());
                  f.open(QIODevice::ReadOnly);
                  content << f.readAll();
                  f.close();
            }
            return content.size();
            QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        }
        else {
            ittext = content[i];
            QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        }
        return 0;
    }*/
    void checkwords() {
        QFile words_list(".config/06-words/config");
        words_list.open(QIODevice::ReadWrite);
        QTextStream in (&words_list);
        words = in.readAll();
        words_list.close();
    }
    bool epub_file_match(QString file) {
        QString fileExt = file.right(4);

        if(fileExt == "epub" or fileExt == "EPUB") {
            return true;
        }
        else {
            return false;
        }
    };
    void dictionary_lookup(string word, QString first_letter, int position) {
        ofstream fhandler;
        fhandler.open("/inkbox/dictionary/word");
        fhandler << word;
        fhandler.close();

        QDir::setCurrent("dictionary");
        QDir::setCurrent(first_letter);
        QString lookup_prog ("sh");
        QStringList lookup_args;
        QString position_str = QString::number(position);
        lookup_args << "../scripts/lookup.sh" << position_str;
        QProcess *lookup_proc = new QProcess();
        lookup_proc->start(lookup_prog, lookup_args);
        lookup_proc->waitForFinished();

        QFile definition_file("/inkbox/dictionary/definition");
        definition_file.open(QIODevice::ReadWrite);
        QTextStream in (&definition_file);
        definition = in.readAll();
        definition = definition.remove(QRegExp("[\n]"));
        if(definition == "No definition found.") {
            nextdefinition_lock = true;
        }
        else {
            nextdefinition_lock = false;
        }
        definition_file.close();

        QDir::setCurrent("/mnt/onboard/.adds/inkbox");
    }
    void save_word(string word, bool remove) {
        if(remove == false) {
            QFile words(".config/06-words/config");
            words.open(QIODevice::ReadWrite);
            QTextStream in (&words);
            QString words_list = in.readAll();
            string words_list_str = words_list.toStdString();
            words.close();

            ofstream fhandler;
            fhandler.open(".config/06-words/config");
            fhandler << words_list_str << word << "\n";
            fhandler.close();
        }
        else {
            ofstream fhandler;
            fhandler.open(".config/06-words/config");
            fhandler << word;
            fhandler.close();
        }
    }

    void menubar_show();
    void menubar_hide();
    void wordwidget_show();
    void wordwidget_hide();
    void batteryWatchdog();
    void openLowBatteryDialog();
    void openCriticalBatteryAlertWindow();

private slots:
    void on_nextBtn_clicked();
    void on_previousBtn_clicked();
    void on_optionsBtn_clicked();
    void on_hideOptionsBtn_clicked();
    void on_brightnessDecBtn_clicked();
    void on_brightnessIncBtn_clicked();
    void on_aboutBtn_clicked();
    void on_homeBtn_clicked();
    void on_fontChooser_currentIndexChanged(const QString &arg1);
    void on_alignLeftBtn_clicked();
    void on_alignCenterBtn_clicked();
    void on_alignRightBtn_clicked();
    void on_alignJustifyBtn_clicked();
    void on_infoCloseBtn_clicked();
    void on_previousDefinitionBtn_clicked();
    void on_nextDefinitionBtn_clicked();
    void on_saveWordBtn_clicked();
    void on_sizeSlider_valueChanged(int value);
    void writeconfig_pagenumber();
    void quit_restart();

private:
    Ui::reader *ui;
    alert *alertWindow;
    generalDialog *generalDialogWindow;
};

#endif // READER_H
