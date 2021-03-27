#ifndef READER_H
#define READER_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QList>
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include <regex>

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
    int batt_level_int;
    bool menubar_shown = false;
    bool selected_text_lock = false;
    bool nextdefinition_lock = false;
    QString book_1;
    QString book_2;
    QString book_3;
    QString book_4;
    QString ittext;
    QString book_file;
    QString batt_level;
    bool batt_status;
    QString percent = "%";
    QString checkconfig_str_val;
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
    bool checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
    };
    int setup_book(QString book, int i, bool run_parser) {
        // Parse ebook
        // TODO: Use BeautifulSoup with Python to parse ePUBs and display them... somehow (?
        QString mount_prog ("sh");
        QStringList mount_args;
        mount_args << "split.sh";
        QProcess *mount_proc = new QProcess();
        mount_proc->start(mount_prog, mount_args);
        mount_proc->waitForFinished();

        if(epub_file_match(book) == true) {
            // Parsing ePUB with epub2txt, thanks to GitHub:kevinboone

        }
        else {
            // This is likely not an ePUB.
            ;
        }

        // Copying book specified in the function call
        QFile::copy(book, "/inkbox/book/book.txt");

        // Checking if the user has defined an option for the number of words per page; if not, then setting the default.
        QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        string_checkconfig(".config/07-words_number/config");
        if(checkconfig_str_val == "") {
            string_writeconfig(".config/07-words_number/config", "100");
            string_checkconfig(".config/07-words_number/config");
        }

        // Parsing file
        QString parse_prog ("python3");
        QStringList parse_args;
        parse_args << "split.py" << checkconfig_str_val;
        QProcess *parse_proc = new QProcess();
        parse_proc->start(parse_prog, parse_args);
        parse_proc->waitForFinished();

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
    }
    int get_brightness() {
        QFile brightness("/var/run/brightness");
        brightness.open(QIODevice::ReadOnly);
        QString valuestr = brightness.readAll();
        int value = valuestr.toInt();
        brightness.close();
        return value;
    }
    int int_checkconfig(QString file) {
        QFile int_config(file);
        int_config.open(QIODevice::ReadOnly);
        QString valuestr = int_config.readAll();
        int value = valuestr.toInt();
        int_config.close();
        qDebug() << value;
        return value;
    }
    void string_checkconfig_ro(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadOnly);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    void checkwords() {
        QFile words_list(".config/06-words/config");
        words_list.open(QIODevice::ReadWrite);
        QTextStream in (&words_list);
        words = in.readAll();
        words_list.close();
    }
    void set_brightness(int value) {
        ofstream fhandler;
        fhandler.open("/var/run/brightness");
        fhandler << value;
        fhandler.close();
    }
    void brightness_writeconfig(int value) {
        ofstream fhandler;
        fhandler.open(".config/03-brightness/config");
        fhandler << value;
        fhandler.close();
    }
    void string_checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        checkconfig_str_val = in.readAll();
        config.close();
    }
    bool checkconfig_match(QString file, string pattern) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        string contentstr = content.toStdString();

        // Thanks to https://stackoverflow.com/questions/22516463/how-do-i-find-a-complete-word-not-part-of-it-in-a-string-in-c
        std::regex r("\\b" + pattern + "\\b");
        std::smatch m;

        if(std::regex_search(contentstr, m, r)) {
            return true;
        }
        else {
            return false;
        }
        config.close();
    };
    bool epub_file_match(QString file) {
        QString fileExt = file.right(4);

        if(fileExt == "epub" or fileExt=="EPUB") {
            qDebug() << "True.";
            return true;
        }
        else {
            qDebug() << "False.";
            return false;
        }
    };
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }
    void get_battery_level() {
        QFile batt_level_file("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/capacity");
        batt_level_file.open(QIODevice::ReadOnly);
        batt_level = batt_level_file.readAll();
        batt_level = batt_level.trimmed();
        batt_level_int = batt_level.toInt();
        batt_level = batt_level.append("%");
        batt_level_file.close();
    }
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

private:
    Ui::reader *ui;
};

#endif // READER_H
