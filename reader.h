#ifndef READER_H
#define READER_H

#include "functions.h"
#include "alert.h"
#include "generaldialog.h"

#include <QWidget>
#include <QTextDocument>

using namespace std;

namespace Ui {
class reader;
}

class reader : public QWidget
{
    Q_OBJECT

public:
    explicit reader(QWidget *parent = nullptr);
    ~reader();

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
    QString content;
    int setup_book(QString book);
    void checkwords();
    bool epub_file_match(QString file);
    void dictionary_lookup(string word, QString first_letter, int position);
    void save_word(string word, bool remove);
    void menubar_show();
    void menubar_hide();
    void wordwidget_show();
    void wordwidget_hide();
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
    QTextDocument *textDocument;
};

#endif // READER_H
