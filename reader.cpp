#include "reader.h"
#include "ui_reader.h"
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QFont>
#include <QIcon>
#include <QSize>
#include <QDesktopWidget>
#include <QScreen>

using namespace std;

reader::reader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reader)
{
    ui->setupUi(this);
    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->hideOptionsBtn->setProperty("type", "borderless");
    ui->optionsBtn->setProperty("type", "borderless");
    ui->brightnessDecBtn->setProperty("type", "borderless");
    ui->brightnessIncBtn->setProperty("type", "borderless");
    ui->homeBtn->setProperty("type", "borderless");
    ui->aboutBtn->setProperty("type", "borderless");
    ui->alignLeftBtn->setProperty("type", "borderless");
    ui->alignRightBtn->setProperty("type", "borderless");
    ui->alignCenterBtn->setProperty("type", "borderless");
    ui->alignLeftBtn->setProperty("type", "borderless");
    ui->alignJustifyBtn->setProperty("type", "borderless");
    ui->infoCloseBtn->setProperty("type", "borderless");
    ui->saveWordBtn->setProperty("type", "borderless");
    ui->previousDefinitionBtn->setProperty("type", "borderless");
    ui->nextDefinitionBtn->setProperty("type", "borderless");

    // Icons
    ui->alignLeftBtn->setText("");
    ui->alignLeftBtn->setIcon(QIcon(":/resources/align-left.png"));
    ui->alignRightBtn->setText("");
    ui->alignRightBtn->setIcon(QIcon(":/resources/align-right.png"));
    ui->alignCenterBtn->setText("");
    ui->alignCenterBtn->setIcon(QIcon(":/resources/align-center.png"));
    ui->alignJustifyBtn->setText("");
    ui->alignJustifyBtn->setIcon(QIcon(":/resources/align-justify.png"));
    ui->infoCloseBtn->setText("");
    ui->infoCloseBtn->setIcon(QIcon(":/resources/close.png"));
    ui->saveWordBtn->setText("");
    ui->saveWordBtn->setIcon(QIcon(":/resources/star.png"));
    ui->previousDefinitionBtn->setText("");
    ui->previousDefinitionBtn->setIcon(QIcon(":/resources/chevron-left.png"));
    ui->nextDefinitionBtn->setText("");
    ui->nextDefinitionBtn->setIcon(QIcon(":/resources/chevron-right.png"));


    // Making text selectable
    ui->text->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Custom settings
    // Font
    string_checkconfig(".config/04-book/font");
    if(checkconfig_str_val == "") {
        ;
    }
    else {
        QFont config_font(checkconfig_str_val);
        ui->text->setFont(config_font);
        ui->fontChooser->setCurrentText(checkconfig_str_val);
    }

    // Alignment
    string_checkconfig(".config/04-book/alignment");
    if (checkconfig_str_val == "") {
        ;
    }
    else {
        if(checkconfig_str_val == "Left") {
            ui->text->setAlignment(Qt::AlignLeft);
        }
        if(checkconfig_str_val == "Center") {
            ui->text->setAlignment(Qt::AlignHCenter);
        }
        if(checkconfig_str_val == "Right") {
            ui->text->setAlignment(Qt::AlignRight);
        }
        if(checkconfig_str_val == "Justify") {
            ui->text->setAlignment(Qt::AlignJustify);
        }
    }

    // Stylesheet + misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->sizeValueLabel->setStyleSheet("font-size: 9pt");

    // Hiding the menubar + definition widget
    ui->inkboxLabel->hide();
    ui->hideOptionsBtn->hide();
    ui->line_4->hide();
    ui->line_5->hide();
    ui->line_6->hide();
    ui->line_7->hide();
    ui->line_8->hide();
    ui->line_9->hide();
    ui->line_10->hide();
    ui->line_11->hide();
    ui->line_12->hide();
    ui->line_13->hide();
    ui->line_14->hide();
    ui->timeLabel->hide();
    ui->brightnessDecBtn->hide();
    ui->brightnessIncBtn->hide();
    ui->brightnessLabel->hide();
    ui->brightnessStatus->hide();
    ui->homeBtn->hide();
    ui->aboutBtn->hide();
    ui->batteryLabel->hide();
    ui->batteryIconLabel->hide();
    ui->fontChooser->hide();
    ui->fontLabel->hide();
    ui->styleLabel->hide();
    ui->alignmentLabel->hide();
    ui->alignJustifyBtn->hide();
    ui->alignCenterBtn->hide();
    ui->alignLeftBtn->hide();
    ui->alignRightBtn->hide();
    ui->sizeLabel->hide();
    ui->sizeSlider->hide();
    ui->sizeValueLabel->hide();
    ui->infoCloseBtn->hide();
    ui->saveWordBtn->hide();
    ui->previousDefinitionBtn->hide();
    ui->nextDefinitionBtn->hide();
    ui->definitionLabel->hide();
    ui->wordSearchLabel->hide();
    ui->wordLabel->hide();
    ui->definitionStatusLabel->hide();

    // Getting brightness level
    int brightness_value = get_brightness();
    ui->brightnessStatus->setValue(brightness_value);

    // Defining pixmaps
    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the icons' size will be
    if(checkconfig("/opt/inkbox_genuine") == true) {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            float stdIconWidth = sW / 16;
            float stdIconHeight = sW / 16;
            QPixmap chargingPixmap(":/resources/battery_charging.png");
            scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap fullPixmap(":/resources/battery_full.png");
            scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap halfPixmap(":/resources/battery_half.png");
            scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap emptyPixmap(":/resources/battery_empty.png");
            scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        }
        else {
            float stdIconWidth = sW / 19;
            float stdIconHeight = sH / 19;
            QPixmap chargingPixmap(":/resources/battery_charging.png");
            scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap fullPixmap(":/resources/battery_full.png");
            scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap halfPixmap(":/resources/battery_half.png");
            scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
            QPixmap emptyPixmap(":/resources/battery_empty.png");
            scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        }
    }
    else {
        float stdIconWidth = sW / 19;
        float stdIconHeight = sH / 19;
        QPixmap chargingPixmap(":/resources/battery_charging.png");
        scaledChargingPixmap = chargingPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap fullPixmap(":/resources/battery_full.png");
        scaledFullPixmap = fullPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap halfPixmap(":/resources/battery_half.png");
        scaledHalfPixmap = halfPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        QPixmap emptyPixmap(":/resources/battery_empty.png");
        scaledEmptyPixmap = emptyPixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
    }

    // Clock setting to show seconds + battery level
    if(checkconfig(".config/02-clock/config") == true) {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm:ss");
           get_battery_level();
           ui->batteryLabel->setText(batt_level);
           ui->timeLabel->setText(time);
        } );
        t->start();
    }
    else {
        QTimer *t = new QTimer(this);
        t->setInterval(500);
        connect(t, &QTimer::timeout, [&]() {
           QString time = QTime::currentTime().toString("hh:mm");
           get_battery_level();
           ui->batteryLabel->setText(batt_level);
           ui->timeLabel->setText(time);
        } );
        t->start();
    }

    // Word selection & dictionary lookup feature
    QString dictionary_position_str = QString::number(dictionary_position);
    ui->definitionStatusLabel->setText(dictionary_position_str);
    QTimer *select_t = new QTimer(this);
    select_t->setInterval(100);
    connect(select_t, &QTimer::timeout, [&]() {
        selected_text = ui->text->selectedText();
        if(ui->text->hasSelectedText() == true) {
            if(selected_text_lock == false) {
                selected_text_lock = true;
                QStringList parts = selected_text.split(' ', QString::SkipEmptyParts);
                for (int i = 0; i < parts.size(); ++i)
                    parts[i].replace(0, 1, parts[i][0].toUpper());
                word = parts.join(" ");
                letter = word.left(1);
                selected_text_str = word.toStdString();
                dictionary_lookup(selected_text_str, letter, dictionary_position);
                ui->wordLabel->setText(word);
                ui->definitionLabel->setText(definition);
                if(checkconfig_match(".config/06-words/config", selected_text_str) == true) {
                    ui->saveWordBtn->setText("");
                    ui->saveWordBtn->setIcon(QIcon(":/resources/starred_star.png"));
                }
                else {
                    ui->saveWordBtn->setText("");
                    ui->saveWordBtn->setIcon(QIcon(":/resources/star.png"));
                }
                wordwidget_show();
            }
            else {
                ;
            }
        }
        else {
            ;
        }
    } );
    select_t->start();

    // We have to get the file's path
    if(checkconfig("/inkbox/skip_opendialog") == true) {
        string_checkconfig("/inkbox/book_number");
        if(checkconfig_str_val == "1") {
            string_checkconfig(".config/08-recent_books/1");
            book_file = checkconfig_str_val;
        }
        if(checkconfig_str_val == "2") {
            string_checkconfig(".config/08-recent_books/2");
            book_file = checkconfig_str_val;
        }
        if(checkconfig_str_val == "3") {
            string_checkconfig(".config/08-recent_books/3");
            book_file = checkconfig_str_val;
        }
        if(checkconfig_str_val == "4") {
            string_checkconfig(".config/08-recent_books/4");
            book_file = checkconfig_str_val;
        }
    }
    else {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            QDir::setCurrent("/mnt/onboard/onboard");
            QFileDialog *dialog = new QFileDialog(this);
            // https://forum.qt.io/topic/29471/solve-how-to-show-qfiledialog-at-center-position-screen/4
            QDesktopWidget desk;
            QRect screenres = desk.screenGeometry(0); dialog->setGeometry(QRect(screenres.width()/4,screenres.height() /4,screenres.width()/2,screenres.height()/2));
            stylesheetFile.open(QFile::ReadOnly);
            dialog->setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();
            book_file = dialog->getOpenFileName(dialog, tr("Open File"), QDir::currentPath());
            QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        }
        else {
            QDir::setCurrent("/mnt/onboard");
            QFileDialog *dialog = new QFileDialog(this);
            // https://forum.qt.io/topic/29471/solve-how-to-show-qfiledialog-at-center-position-screen/4
            QDesktopWidget desk;
            QRect screenres = desk.screenGeometry(0); dialog->setGeometry(QRect(screenres.width()/4,screenres.height() /4,screenres.width()/2,screenres.height()/2));
            stylesheetFile.open(QFile::ReadOnly);
            dialog->setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();
            book_file = dialog->getOpenFileName(dialog, tr("Open File"), QDir::currentPath());
            QDir::setCurrent("/mnt/onboard/.adds/inkbox");
        }
    }

    // Counting number of parsed files
    split_total = setup_book(book_file, 0, true);
    split_total = split_total - 1;
    split_files_number = setup_book(book_file, 0, true);
    split_files_number = split_files_number / 2;

    // Get text
    QDir::setCurrent("/mnt/onboard/.adds/inkbox");
    setup_book(book_file, split_total, false);

    // Display text
    // Checking saved font size if any
    string_checkconfig(".config/04-book/size");
    if(checkconfig_str_val == "0") {
        checkconfig_str_val = "6";
        ui->sizeSlider->setValue(0);
        ui->sizeValueLabel->setText("1");
    }
    if(checkconfig_str_val == "1") {
        checkconfig_str_val = "10";
        ui->sizeSlider->setValue(1);
        ui->sizeValueLabel->setText("2");
    }
    if(checkconfig_str_val == "2") {
        checkconfig_str_val = "14";
        ui->sizeSlider->setValue(2);
        ui->sizeValueLabel->setText("3");
    }
    if(checkconfig_str_val == "3") {
        checkconfig_str_val = "18";
        ui->sizeSlider->setValue(3);
        ui->sizeValueLabel->setText("4");
    }
    if(checkconfig_str_val == "4") {
        checkconfig_str_val = "22";
        ui->sizeSlider->setValue(4);
        ui->sizeValueLabel->setText("5");
    }
    if(checkconfig_str_val == "") {
        checkconfig_str_val = "10";
        ui->sizeSlider->setValue(1);
        ui->sizeValueLabel->setText("2");
    }
    QString font_size = "font-size: ";
    font_size = font_size.append(checkconfig_str_val);
    font_size = font_size.append("pt");
    ui->text->setStyleSheet(font_size);

    // Wheeee!
    ui->text->setText(ittext);

    // Saving the book opened in the favorites list
    string_checkconfig(".config/08-recent_books/1");
    book_1 = checkconfig_str_val;
    string str_book_1 = book_1.toStdString();
    string_checkconfig(".config/08-recent_books/2");
    book_2 = checkconfig_str_val;
    string str_book_2 = book_2.toStdString();
    string_checkconfig(".config/08-recent_books/3");
    book_3 = checkconfig_str_val;
    string str_book_3 = book_3.toStdString();
    string_checkconfig(".config/08-recent_books/4");
    book_4 = checkconfig_str_val;
    string str_book_4 = book_4.toStdString();

    string book_file_str = book_file.toStdString();

    if(book_1 == book_file) {
        ;
    }
    else {
        // Moves old items to the right and puts the new one at the left side
        string_writeconfig(".config/08-recent_books/1", book_file_str);
        string_writeconfig(".config/08-recent_books/2", str_book_1);
        string_writeconfig(".config/08-recent_books/3", str_book_2);
        string_writeconfig(".config/08-recent_books/4", str_book_3);
    }
}

reader::~reader()
{
    delete ui;
}

void reader::on_nextBtn_clicked()
{
    if(split_total - 1 == 0) {
        QMessageBox::critical(this, tr("Invalid argument"), tr("You've reached the end of the document."));
    }
    else {
        split_total = split_total - 1;
        setup_book(book_file, split_total, false);
        ui->text->setText("");
        ui->text->setText(ittext);
    }
}

void reader::on_previousBtn_clicked()
{
    // Making sure we won't encounter a "List index out of range" error ;)
    if(split_total >= split_files_number) {
        QMessageBox::critical(this, tr("Invalid argument"), tr("No previous page."));
    }
    else {
        split_total = split_total + 1;
        setup_book(book_file, split_total, false);
        ui->text->setText("");
        ui->text->setText(ittext);
    }
}

void reader::on_optionsBtn_clicked()
{
    menubar_show();
}

void reader::on_hideOptionsBtn_clicked()
{
    menubar_hide();
    this->repaint();
}

void reader::on_brightnessDecBtn_clicked()
{
    int bval = get_brightness();
    int set_bval = bval - 1;
    set_brightness(set_bval);
    brightness_writeconfig(set_bval);

    bval = get_brightness();
    ui->brightnessStatus->setValue(bval);
}

void reader::on_brightnessIncBtn_clicked()
{
    int bval = get_brightness();
    int set_bval = bval + 1;
    set_brightness(set_bval);
    brightness_writeconfig(set_bval);

    bval = get_brightness();
    ui->brightnessStatus->setValue(bval);
}

void reader::on_aboutBtn_clicked()
{
    if(checkconfig("/opt/inkbox_genuine") == true) {
        QString aboutmsg = "InkBox is an open-source Qt-based eBook reader. It brings you the latest Qt features while being also fast and responsive.";
        string_checkconfig_ro("/external_root/opt/isa/version");
        aboutmsg.append("\n\nInkBox v");
        aboutmsg.append(checkconfig_str_val);
        QMessageBox::information(this, tr("Information"), aboutmsg);
    }
    else {
        QMessageBox::information(this, tr("About"), tr("InkBox is an open-source Qt-based eBook reader. It brings you the latest Qt features while being also fast and responsive."));
    }
}

void reader::on_homeBtn_clicked()
{
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void reader::on_fontChooser_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Roboto") {
        QFont roboto("Roboto");
        ui->text->setFont(roboto);
        string_writeconfig(".config/04-book/font", "Roboto");
    }
    if(arg1 == "Inter") {
        QFont inter("Inter");
        ui->text->setFont(inter);
        string_writeconfig(".config/04-book/font", "Inter");
    }
    if(arg1 == "Source Serif Pro") {
        QFont sourceserif("Source Serif Pro");
        ui->text->setFont(sourceserif);
        string_writeconfig(".config/04-book/font", "Source Serif Pro");
    }
    if(arg1 == "Libre Baskerville") {
        QFont librebaskerville("Libre Baskerville");
        ui->text->setFont(librebaskerville);
        string_writeconfig(".config/04-book/font", "Libre Baskerville");
    }
    if(arg1 == "Noto Mono") {
        QFont notomono("Noto Mono");
        ui->text->setFont(notomono);
        string_writeconfig(".config/04-book/font", "Noto Mono");
    }
    if(arg1 == "Libertinus Serif") {
        QFont libertinus("Libertinus Serif");
        ui->text->setFont(libertinus);
        string_writeconfig(".config/04-book/font", "Libertinus Serif");
    }
}

void reader::on_alignLeftBtn_clicked()
{
    ui->text->setAlignment(Qt::AlignLeft);
    string_writeconfig(".config/04-book/alignment", "Left");
}

void reader::on_alignCenterBtn_clicked()
{
    ui->text->setAlignment(Qt::AlignHCenter);
    string_writeconfig(".config/04-book/alignment", "Center");
}

void reader::on_alignRightBtn_clicked()
{
    ui->text->setAlignment(Qt::AlignRight);
    string_writeconfig(".config/04-book/alignment", "Right");
}

void reader::on_alignJustifyBtn_clicked()
{
    ui->text->setAlignment(Qt::AlignJustify);
    string_writeconfig(".config/04-book/alignment", "Justify");
}

void reader::menubar_show() {
    // Checking battery level and status, then displaying the relevant icon on batteryIconLabel
    string_checkconfig_ro("/sys/devices/platform/pmic_battery.1/power_supply/mc13892_bat/status");
    if(checkconfig_str_val == "Charging\n") {
        ui->batteryIconLabel->setPixmap(scaledChargingPixmap);
    }
    else {
        get_battery_level();
        if(batt_level_int >= 75 && batt_level_int <= 100) {
            ui->batteryIconLabel->setPixmap(scaledFullPixmap);
        }
        if(batt_level_int >= 25 && batt_level_int <= 74) {
            ui->batteryIconLabel->setPixmap(scaledHalfPixmap);
        }
        if(batt_level_int >= 0 && batt_level_int <= 24) {
            ui->batteryIconLabel->setPixmap(scaledEmptyPixmap);
        }
    }

    ui->inkboxLabel->show();
    ui->hideOptionsBtn->show();
    ui->optionsBtn->hide();
    ui->line_4->show();
    ui->line_5->show();
    ui->line_6->show();
    ui->line_7->show();
    ui->line_8->show();
    ui->line_9->show();
    ui->line_10->show();
    ui->line_11->show();
    ui->timeLabel->show();
    ui->brightnessDecBtn->show();
    ui->brightnessIncBtn->show();
    ui->brightnessLabel->show();
    ui->brightnessStatus->show();
    ui->aboutBtn->show();
    ui->homeBtn->show();
    ui->batteryLabel->show();
    ui->batteryIconLabel->show();
    ui->fontChooser->show();
    ui->fontLabel->show();
    ui->styleLabel->show();
    ui->alignmentLabel->show();
    ui->alignJustifyBtn->show();
    ui->alignCenterBtn->show();
    ui->alignLeftBtn->show();
    ui->alignRightBtn->show();
    ui->sizeLabel->show();
    ui->sizeSlider->show();
    ui->sizeValueLabel->show();
    menubar_shown = true;
}

void reader::menubar_hide() {
    ui->inkboxLabel->hide();
    ui->hideOptionsBtn->hide();
    ui->line_4->hide();
    ui->line_5->hide();
    ui->line_6->hide();
    ui->line_7->hide();
    ui->line_8->hide();
    ui->line_9->hide();
    ui->line_10->hide();
    ui->line_11->hide();
    ui->optionsBtn->show();
    ui->timeLabel->hide();
    ui->brightnessDecBtn->hide();
    ui->brightnessIncBtn->hide();
    ui->brightnessLabel->hide();
    ui->brightnessStatus->hide();
    ui->aboutBtn->hide();
    ui->homeBtn->hide();
    ui->batteryLabel->hide();
    ui->batteryIconLabel->hide();
    ui->fontChooser->hide();
    ui->fontLabel->hide();
    ui->styleLabel->hide();
    ui->alignmentLabel->hide();
    ui->alignJustifyBtn->hide();
    ui->alignCenterBtn->hide();
    ui->alignLeftBtn->hide();
    ui->alignRightBtn->hide();
    ui->sizeLabel->hide();
    ui->sizeSlider->hide();
    ui->sizeValueLabel->hide();
    menubar_shown = false;
}

void reader::wordwidget_show() {
    if(menubar_shown == true) {
        menubar_hide();
        ui->optionsBtn->hide();
        ui->hideOptionsBtn->hide();
        ui->infoCloseBtn->show();
        ui->saveWordBtn->show();
        ui->previousDefinitionBtn->show();
        ui->nextDefinitionBtn->show();
        ui->definitionLabel->show();
        ui->wordSearchLabel->show();
        ui->wordLabel->show();
        ui->definitionStatusLabel->show();
        ui->line->hide();
        ui->line_12->show();
        ui->line_13->show();
        ui->line_14->show();
    }
    else {
        ui->optionsBtn->hide();
        ui->hideOptionsBtn->hide();
        ui->infoCloseBtn->show();
        ui->saveWordBtn->show();
        ui->definitionLabel->show();
        ui->definitionStatusLabel->show();
        ui->previousDefinitionBtn->show();
        ui->nextDefinitionBtn->show();
        ui->wordSearchLabel->show();
        ui->wordLabel->show();
        ui->line_13->show();
        ui->line_12->show();
        ui->line->hide();
        ui->line_14->show();
    }
}

void reader::wordwidget_hide() {
    ui->infoCloseBtn->hide();
    ui->saveWordBtn->hide();
    ui->definitionLabel->hide();
    ui->wordSearchLabel->hide();
    ui->wordLabel->hide();
    ui->line_12->hide();
    ui->line_13->hide();
    ui->line_14->hide();
    ui->line->show();
    ui->optionsBtn->show();
    ui->previousDefinitionBtn->hide();
    ui->nextDefinitionBtn->hide();
    ui->definitionStatusLabel->hide();
    selected_text_lock = false;
}

void reader::on_infoCloseBtn_clicked()
{
    wordwidget_hide();
    dictionary_position = 1;
    QString dictionary_position_str = QString::number(dictionary_position);
    ui->definitionStatusLabel->setText(dictionary_position_str);
}

void reader::on_previousDefinitionBtn_clicked()
{
    dictionary_position = dictionary_position - 1;
    if(dictionary_position <= 0) {
        dictionary_position = 1;
    }
    else {
        dictionary_lookup(selected_text_str, letter, dictionary_position);
        ui->definitionLabel->setText(definition);
        QString dictionary_position_str = QString::number(dictionary_position);
        ui->definitionStatusLabel->setText(dictionary_position_str);
    }
}

void reader::on_nextDefinitionBtn_clicked()
{
    dictionary_position = dictionary_position + 1;
    dictionary_lookup(selected_text_str, letter, dictionary_position);
    if(nextdefinition_lock == true) {
        dictionary_position = dictionary_position - 1;
    }
    else {
        ui->definitionLabel->setText(definition);
        QString dictionary_position_str = QString::number(dictionary_position);
        ui->definitionStatusLabel->setText(dictionary_position_str);
    }
}

void reader::on_saveWordBtn_clicked()
{
    if(checkconfig_match(".config/06-words/config", selected_text_str) == true) {
        checkwords();
        word = word.append("\n");
        words = words.replace(word, "");
        string words_std_string = words.toStdString();
        save_word(words_std_string, true);
        ui->saveWordBtn->setText("");
        ui->saveWordBtn->setIcon(QIcon(":/resources/star.png"));
    }
    else {
        save_word(selected_text_str, false);
        ui->saveWordBtn->setText("");
        ui->saveWordBtn->setIcon(QIcon(":/resources/starred_star.png"));
    }
}

void reader::on_sizeSlider_valueChanged(int value)
{
    // Font size
    string value_str = to_string(value);
    string_writeconfig(".config/04-book/size", value_str);

    // Future improvement?
    if(checkconfig_match("/opt/inkbox_device", "n705\n") == true) {
        if(value == 0) {
            ui->text->setStyleSheet("font-size: 6pt");
            ui->sizeValueLabel->setText("1");
        }
        if(value == 1) {
            ui->text->setStyleSheet("font-size: 10pt");
            ui->sizeValueLabel->setText("2");
        }
        if(value == 2) {
            ui->text->setStyleSheet("font-size: 14pt");
            ui->sizeValueLabel->setText("3");
        }
        if(value == 3) {
            ui->text->setStyleSheet("font-size: 18pt");
            ui->sizeValueLabel->setText("4");
        }
        if(value == 4) {
            ui->text->setStyleSheet("font-size: 22pt");
            ui->sizeValueLabel->setText("5");
        }
    }
    else {
        if(value == 0) {
            ui->text->setStyleSheet("font-size: 6pt");
            ui->sizeValueLabel->setText("1");
        }
        if(value == 1) {
            ui->text->setStyleSheet("font-size: 10pt");
            ui->sizeValueLabel->setText("2");
        }
        if(value == 2) {
            ui->text->setStyleSheet("font-size: 14pt");
            ui->sizeValueLabel->setText("3");
        }
        if(value == 3) {
            ui->text->setStyleSheet("font-size: 18pt");
            ui->sizeValueLabel->setText("4");
        }
        if(value == 4) {
            ui->text->setStyleSheet("font-size: 22pt");
            ui->sizeValueLabel->setText("5");
        }
    }
}
