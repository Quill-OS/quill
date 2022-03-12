#include "reader.h"
#include "ui_reader.h"
#include "functions.h"
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
#include <QFontDatabase>
#include <QDirIterator>
#include <QTextCursor>
#include <QDebug>
#include <QGraphicsScene>

using namespace std;

reader::reader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reader)
{
    // Elements
    graphicsScene = new QGraphicsScene(this);

    // Variables
    global::battery::showLowBatteryDialog = true;
    global::battery::showCriticalBatteryAlert = true;
    if(global::reader::bookIsEpub == true) {
        is_epub = true;
    }
    mupdf::convertRelativeValues = false;
    wordwidgetLock = false;
    goToSavedPageDone = false;

    ui->setupUi(this);
    ui->brightnessStatus->setFont(QFont("u001"));
    ui->fontLabel->setFont(QFont("u001"));
    ui->sizeLabel->setFont(QFont("u001"));
    ui->sizeValueLabel->setFont(QFont("u001"));
    ui->alignmentLabel->setFont(QFont("u001"));
    ui->fontChooser->setFont(QFont("u001"));
    ui->definitionStatusLabel->setFont(QFont("u001"));
    ui->pageProgressBar->setFont(QFont("u001"));

    ui->previousBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
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
    ui->nightModeBtn->setProperty("type", "borderless");
    ui->searchBtn->setProperty("type", "borderless");
    ui->gotoBtn->setProperty("type", "borderless");
    ui->increaseScaleBtn->setProperty("type", "borderless");
    ui->decreaseScaleBtn->setProperty("type", "borderless");
    ui->quitBtn->setProperty("type", "borderless");

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
    ui->brightnessDecBtn->setText("");
    ui->brightnessDecBtn->setIcon(QIcon(":/resources/minus.png"));
    ui->brightnessIncBtn->setText("");
    ui->brightnessIncBtn->setIcon(QIcon(":/resources/plus.png"));
    ui->homeBtn->setText("");
    ui->homeBtn->setIcon(QIcon(":/resources/home.png"));
    ui->aboutBtn->setText("");
    ui->aboutBtn->setIcon(QIcon(":/resources/info.png"));
    ui->searchBtn->setText("");
    ui->searchBtn->setIcon(QIcon(":/resources/search.png"));
    ui->increaseScaleBtn->setText("");
    ui->increaseScaleBtn->setIcon(QIcon(":/resources/zoom-in.png"));
    ui->decreaseScaleBtn->setText("");
    ui->decreaseScaleBtn->setIcon(QIcon(":/resources/zoom-out.png"));
    ui->quitBtn->setText("");
    ui->quitBtn->setIcon(QIcon(":/resources/power.png"));

    // On the Mini with QT_FONT_DPI set to 187 (default for this device), quitBtn makes the UI go beyond the limits of the screen when the menu bar is shown
    if(readFile("/opt/inkbox_device") == "n705\n") {
        ui->quitBtn->hide();
        ui->quitBtn->deleteLater();
        ui->line_19->hide();
        ui->line_19->deleteLater();
    }

    // Style misc.
    ui->bookInfoLabel->setStyleSheet("font-style: italic");

    // Making text selectable
    ui->text->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Font misc.
    int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Italic.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont crimson(family);

    // Stylesheet + misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // We have to get the file's path
    if(global::reader::skipOpenDialog == true) {
        if(checkconfig("/tmp/suspendBook") == true) {
            wakeFromSleep = true;
            // Prevent from opening the Reader framework next time unless the condition is reset
            string_writeconfig("/tmp/suspendBook", "false");
            book_file = "/inkbox/book/book.txt";
        }
        else if(global::reader::bookFile.isEmpty() == false) {
            book_file = global::reader::bookFile;
            global::reader::bookFile = "";
        }
        else {
            if(global::reader::bookNumber == 1) {
                string_checkconfig(".config/08-recent_books/1");
                book_file = checkconfig_str_val;
            }
            if(global::reader::bookNumber == 2) {
                string_checkconfig(".config/08-recent_books/2");
                book_file = checkconfig_str_val;
            }
            if(global::reader::bookNumber == 3) {
                string_checkconfig(".config/08-recent_books/3");
                book_file = checkconfig_str_val;
            }
            if(global::reader::bookNumber == 4) {
                string_checkconfig(".config/08-recent_books/4");
                book_file = checkconfig_str_val;
            }
        }
    }
    else {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            if(checkconfig("/external_root/run/encfs_mounted") == true) {
                QDir::setCurrent("/mnt/onboard/onboard/encfs-decrypted");
            }
            else {
                QDir::setCurrent("/mnt/onboard/onboard");
            }
            QFileDialog *dialog = new QFileDialog(this);
            // https://forum.qt.io/topic/29471/solve-how-to-show-qfiledialog-at-center-position-screen/4
            QDesktopWidget desk;
            QRect screenres = desk.screenGeometry(0); dialog->setGeometry(QRect(screenres.width()/4,screenres.height() /4,screenres.width()/2,screenres.height()/2));
            stylesheetFile.open(QFile::ReadOnly);
            dialog->setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();
            book_file = dialog->getOpenFileName(dialog, tr("Open File"), QDir::currentPath());

            if(book_file != "") {
                setDefaultWorkDir();
            }
            else {
                // User clicked "Cancel" button
                setDefaultWorkDir();
                quit_restart();
            }
        }
        else {
            if(checkconfig("/external_root/run/encfs_mounted") == true) {
                QDir::setCurrent("/mnt/onboard/onboard/encfs-decrypted");
            }
            else {
                QDir::setCurrent("/mnt/onboard/onboard");
            }
            QDir::setCurrent("/mnt/onboard");
            QFileDialog *dialog = new QFileDialog(this);
            // https://forum.qt.io/topic/29471/solve-how-to-show-qfiledialog-at-center-position-screen/4
            QDesktopWidget desk;
            QRect screenres = desk.screenGeometry(0); dialog->setGeometry(QRect(screenres.width()/4,screenres.height() /4,screenres.width()/2,screenres.height()/2));
            stylesheetFile.open(QFile::ReadOnly);
            dialog->setStyleSheet(stylesheetFile.readAll());
            stylesheetFile.close();
            book_file = dialog->getOpenFileName(dialog, tr("Open File"), QDir::currentPath());

            if(book_file != "") {
                setDefaultWorkDir();
            }
            else {
                // User clicked "Cancel" button
                setDefaultWorkDir();
                quit_restart();
            }
        }
    }
    qDebug() << "Reader: Opening file" << book_file;

    // Writing book path to file
    std::string book_file_str = book_file.toStdString();
    string_writeconfig("/tmp/inkboxBookPath", book_file_str);

    // Calling InkBox daemon (ibxd) via FIFO interface to run bookconfig_mount
    if(checkconfig(".config/16-global_reading_settings/config") == false) {
        global::reader::globalReadingSettings = false;
        string_writeconfig("/opt/ibxd", "bookconfig_mount\n");
        // Callback handler to wait until bookconfig_mount has finished execution
        while(true) {
            if(QFile::exists("/inkbox/bookConfigSetUp")) {
                QFile::remove("/inkbox/bookConfigSetUp");
                setupLocalSettingsEnvironment();
                break;
            }
        }
    }
    else {
        global::reader::globalReadingSettings = true;
    }

    // Custom settings
    // Font
    string_checkconfig(".config/04-book/font");
    if(checkconfig_str_val == "") {
        ui->fontChooser->setCurrentText("Source Serif Pro");
        ui->text->setFont(QFont("Source Serif Pro"));
    }
    else {
        if(checkconfig_str_val == "Crimson Pro") {
            setCrimsonProFont();
        }
        else if(checkconfig_str_val == "Bitter") {
            setBitterFont();
        }
        else if(checkconfig_str_val == "Ibarra Real Nova") {
            setIbarraFont();
        }
        else {
            QFont config_font(checkconfig_str_val);
            ui->text->setFont(config_font);
            ui->fontChooser->setCurrentText(checkconfig_str_val);
        }
    }
    // Night mode
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "n306\n") {
        if(checkconfig(".config/10-dark_mode/config") == true) {
            string_writeconfig("/tmp/invertScreen", "y");
            ui->nightModeBtn->setText("");
            ui->nightModeBtn->setIcon(QIcon(":/resources/nightmode-full.png"));
            isNightModeActive = true;
        }
        else {
            string_writeconfig("/tmp/invertScreen", "n");
            ui->nightModeBtn->setText("");
            ui->nightModeBtn->setIcon(QIcon(":/resources/nightmode-empty.png"));
            isNightModeActive = false;
        }
    }
    else {
        ui->line_7->hide();
        ui->line_7->deleteLater();
        ui->nightModeBtn->hide();
        ui->nightModeBtn->deleteLater();
    }

    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n873\n") {
        ui->nextBtn->setStyleSheet("padding: 13.5px");
        ui->previousBtn->setStyleSheet("padding: 13.5px");
        ui->optionsBtn->setStyleSheet("padding: 13.5px");
    }
    ui->sizeValueLabel->setStyleSheet("font-size: 9pt");
    ui->homeBtn->setStyleSheet("font-size: 9pt; padding: 5px");
    ui->aboutBtn->setStyleSheet("font-size: 9pt; padding: 5px");
    ui->fontChooser->setStyleSheet("font-size: 9pt");
    ui->gotoBtn->setStyleSheet("font-size: 9pt; padding: 9px; font-weight: bold; background: lightGrey");
    ui->pageNumberLabel->setFont(QFont("Source Serif Pro"));

    // Hiding the menubar + definition widget + brightness widget + buttons bar widget
    ui->menuWidget->setVisible(false);
    ui->brightnessWidget->setVisible(false);
    ui->menuBarWidget->setVisible(false);
    ui->buttonsBarWidget->setVisible(false);
    ui->pdfScaleWidget->setVisible(false);
    ui->wordWidget->setVisible(false);
    if(checkconfig(".config/11-menubar/sticky") == true) {
        ui->menuWidget->setVisible(true);
        ui->statusBarWidget->setVisible(true);
    }
    else {
        ui->menuWidget->setVisible(false);
        ui->statusBarWidget->setVisible(false);
    }
    ui->pageWidget->hide();
    if(pdf_file_match(book_file) == true) {
        ui->line_4->setLineWidth(2);
    }

    // Topbar widget / book info
    ui->topbarStackedWidget->setVisible(true);
    showTopbarWidget = true;
    ui->bookInfoLabel->setFont(crimson);

    // Getting brightness level
    int brightness_value;
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        brightness_value = get_brightness();
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        brightness_value = brightness_checkconfig(".config/03-brightness/config");
    }
    else {
        brightness_value = get_brightness();
    }
    ui->brightnessStatus->setValue(brightness_value);

    // Defining pixmaps
    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the icons' size will be
    if(checkconfig("/opt/inkbox_genuine") == true) {
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "n306\n" or checkconfig_str_val == "emu\n") {
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

    // Checking if there is a page refresh setting set
    string_checkconfig(".config/04-book/refresh");
    if(checkconfig_str_val == "") {
        // Writing the default, refresh every 3 pages
        string_writeconfig(".config/04-book/refresh", "3");
        string_checkconfig(".config/04-book/refresh");
    }
    else {
        // A config option was set, continuing after the Else statement...
        ;
    }
    pageRefreshSetting = checkconfig_str_val.toInt();
    // Checking if that config option was set to "Never refresh"...
    if(pageRefreshSetting == -1) {
        neverRefresh = true;
    }
    else {
        // Safety measure
        neverRefresh = false;
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

    // Checking if we're waking from sleep and have lockscreen enabled; if so, do nothing there because the book should already have been parsed
    if(wakeFromSleep != true) {
        // Remount tmpfs
        string_writeconfig("/inkbox/remount", "true");

        // Counting number of parsed files
        split_total = setup_book(book_file, 0, true);
        split_files_number = split_total;
        split_total = split_total - 1;

        writeconfig_pagenumber(false);
    }
    else {
        // Retrieve split_total from tmpfs
        string_checkconfig("/tmp/inkboxPageNumber");
        split_total = checkconfig_str_val.toInt();
        setup_book(book_file, 0, true);
    }

    // Get text; no need to do it multiple times for ePUB books
    if(is_epub == false && is_pdf == false && is_image == false) {
        setDefaultWorkDir();
        if(global::reader::globalReadingSettings == false) {
            string_checkconfig_ro(".config/A-page_number/config");
            if(checkconfig_str_val != "") {
                split_total = checkconfig_str_val.toInt();
            }
        }
        setup_book(book_file, split_total, true);
    }

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

    // If needed, show scroll bar when rendering engine isn't doing its job properly
    if(checkconfig(".config/14-reader_scrollbar/config") == true) {
        ui->text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->text->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else {
        ui->text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    // Wheeee!
    if(is_epub == true) {
        ui->graphicsView->hide();
        ui->graphicsView->deleteLater();
        ui->text->setText(epubPageContent);
    }
    else if(is_pdf == true) {
        ui->text->hide();
        ui->text->deleteLater();
        setupPng();
    }
    else if(is_image == true) {
        ui->text->hide();
        ui->text->deleteLater();
        setupPng();
    }
    else {
        ui->text->setText(ittext);
    }

    // Alignment
    string_checkconfig(".config/04-book/alignment");
    if (checkconfig_str_val == "") {
        ;
    }
    else {
        if(checkconfig_str_val == "Left") {
                textAlignment = 0;
                alignText(0);
        }
        if(checkconfig_str_val == "Center") {
                textAlignment = 1;
                alignText(1);
        }
        if(checkconfig_str_val == "Right") {
                textAlignment = 2;
                alignText(2);
        }
        if(checkconfig_str_val == "Justify") {
                textAlignment = 3;
                alignText(3);
        }
    }

    // Topbar info widget
    if(is_epub == true) {
        QString bookCreator = findEpubMetadata(book_file, "creator");
        QString bookTitle = findEpubMetadata(book_file, "title");
        bookCreator = bookCreator.trimmed();
        bookTitle = bookTitle.trimmed();

        QString infoLabelContent = bookCreator;
        infoLabelContent.append(" ― ");
        infoLabelContent.append(bookTitle);
        int infoLabelLength = infoLabelContent.length();
        int infoLabelDefinedLength;
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val == "n705\n") {
            infoLabelDefinedLength = 35;
        }
        else if(checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "n306\n") {
            infoLabelDefinedLength = 50;
        }
        else {
            infoLabelDefinedLength = 50;
        }

        if(infoLabelLength <= infoLabelDefinedLength) {
            ui->bookInfoLabel->setWordWrap(false);
        }
        else {
            ui->bookInfoLabel->setWordWrap(true);
        }
        ui->bookInfoLabel->setText(infoLabelContent);
    }
    else {
        QString bookReadRelativePath;
        if(wakeFromSleep == true) {
            string_checkconfig_ro("/tmp/inkboxBookPath");
            bookReadRelativePath = checkconfig_str_val.split("/").last();
        }
        else {
            bookReadRelativePath = book_file.split("/").last();
        }

        int infoLabelDefinedLength;
        if(checkconfig_str_val == "n705\n") {
            infoLabelDefinedLength = 35;
        }
        else if(checkconfig_str_val == "n905\n" or checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n" or checkconfig_str_val == "n437\n" or checkconfig_str_val == "n306\n") {
            infoLabelDefinedLength = 50;
        }
        else {
            infoLabelDefinedLength = 50;
        }

        if(bookReadRelativePath <= infoLabelDefinedLength) {
            ui->bookInfoLabel->setWordWrap(false);
        }
        else {
            ui->bookInfoLabel->setWordWrap(true);
        }
        ui->bookInfoLabel->setText(bookReadRelativePath);
    }

    // Clean up
    string_writeconfig("/inkbox/remount", "true");

    // Way to tell shell scripts that we're in the Reader framework
    string_writeconfig("/tmp/inkboxReading", "true");

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
    std::string str_book_4 = book_4.toStdString();

    // Don't mess up "Recently read books" with random "book.txt" buttons...
    if(wakeFromSleep == true) {
        string_checkconfig("/tmp/inkboxBookPath");
        book_file_str = checkconfig_str_val.toStdString();
    }
    else {
        book_file_str = book_file.toStdString();
        string_writeconfig("/tmp/inkboxBookPath", book_file_str);
    }

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

    // USB mass storage prompt
    if(global::reader::startUsbmsPrompt == true) {
        QTimer *usbmsPrompt = new QTimer(this);
        usbmsPrompt->setInterval(500);
        connect(usbmsPrompt, &QTimer::timeout, [&]() {
            if(checkconfig("/opt/inkbox_genuine") == true) {
                if(global::usbms::showUsbmsDialog != true) {
                    if(isUsbPluggedIn() != usbmsStatus) {
                        global::usbms::showUsbmsDialog = true;
                    }
                }
                else {
                    usbmsStatus = isUsbPluggedIn();
                    if(usbmsStatus == false) {
                        // Loop again...
                        ;
                    }
                    else {
                        // An USB cable is connected!
                        openUsbmsDialog();
                    }
                }
            }
            else {
                // Do nothing, we're running along with Nickel & friends...
                ;
            }
        } );
        usbmsPrompt->start();
    }

    // Battery watchdog
    if(global::reader::startBatteryWatchdog == true) {
        QTimer *t = new QTimer(this);
        t->setInterval(2000);
        connect(t, &QTimer::timeout, [&]() {
            // Checking if battery level is low
            if(global::battery::showCriticalBatteryAlert != true) {
                ;
            }
            else {
                if(isBatteryCritical() == true) {
                    qDebug() << "Warning! Battery is at a critical charge level!";
                    openCriticalBatteryAlertWindow();
                }
            }

            if(global::battery::showLowBatteryDialog != true) {
                // Do nothing, since a dialog should already have been displayed and (probably) dismissed
                ;
            }
            else {
                if(isBatteryLow() == true) {
                    if(global::battery::batteryAlertLock == true) {
                        ;
                    }
                    else {
                        if(isUsbPluggedIn()) {
                            ;
                        }
                        else {
                            qDebug() << "Warning! Battery is low!";
                            openLowBatteryDialog();
                        }
                    }
                }
            }
        } );
        string_checkconfig_ro("/opt/inkbox_device");
        if(checkconfig_str_val != "emu\n") {
            t->start();
        }
    }

    // Pages number info label
    if(is_pdf == true) {
        getTotalPdfPagesNumber();
    }
    // The total pages number is already known for ePUBs at this point
    setupPageWidget();

    // Reading settings auto-save timer
    if(global::reader::globalReadingSettings == false) {
        QTimer * saveSettingsTimer = new QTimer(this);
        saveSettingsTimer->setInterval(900000); // 15 minutes
        connect(saveSettingsTimer, SIGNAL(timeout()), this, SLOT(saveReadingSettings()));
        saveSettingsTimer->start();
    }
}

reader::~reader()
{
    delete ui;
}

int reader::setup_book(QString book, int i, bool run_parser) {
    // Parse ebook
    if(remount == true) {
        QString mount_prog ("sh");
        QStringList mount_args;
        mount_args << "split.sh";
        QProcess * mount_proc = new QProcess();
        mount_proc->start(mount_prog, mount_args);
        mount_proc->waitForFinished();
        mount_proc->deleteLater();
        remount = false;
    }
    else {
        string_writeconfig("/inkbox/remount", "false");
        QString mount_prog ("sh");
        QStringList mount_args;
        mount_args << "split.sh";
        QProcess * mount_proc = new QProcess();
        mount_proc->start(mount_prog, mount_args);
        mount_proc->waitForFinished();
        mount_proc->deleteLater();
    }

    if(filematch_ran == false) {
        if(epub_file_match(book) == true) {
            QFile::remove("/run/book.epub");
            QFile::copy(book, "/run/book.epub");

            getTotalEpubPagesNumber();

            filematch_ran = true;
            is_epub = true;
        }
        else if(pdf_file_match(book) == true) {
            QString pdfProg("/usr/local/bin/mutool");
            QStringList pdfArgs;
            convertMuPdfVars(1, true);
            pdfArgs << "convert" << "-F" << "png" << "-O" << "width=" + mupdf::pdf::width_qstr + ",height=" + mupdf::pdf::height_qstr << "-o" << "/run/page.png" << book_file << mupdf::pdf::pdfPageNumber_qstr;
            QProcess * pdfProc = new QProcess();
            pdfProc->start(pdfProg, pdfArgs);
            pdfProc->waitForFinished();
            pdfProc->deleteLater();

            filematch_ran = true;
            is_pdf = true;
        }
        else if(image_file_match(book) == true) {
            ui->previousBtn->hide();
            ui->nextBtn->hide();
            ui->line->hide();
            ui->line_3->hide();
            ui->previousBtn->deleteLater();
            ui->nextBtn->deleteLater();
            ui->line->deleteLater();
            ui->line_3->deleteLater();

            string_writeconfig("/tmp/inkboxImagePath", book.toStdString());
            string_writeconfig("/opt/ibxd", "inkbox_convert_image\n");

            while(true) {
                if(QFile::exists("/inkbox/convertImageDone")) {
                    if(checkconfig("/inkbox/convertImageDone") == true) {
                        filematch_ran = true;
                        is_image = true;
                        break;
                    }
                    else {
                        showToast("Failed to display image");
                        break;
                    }
                    QFile::remove("/inkbox/convertImageDone");
                }
            }

            filematch_ran = true;
            is_image = true;
        }
        else {
            // This is likely not an ePUB, a PDF or a picture.
            // Copying book specified in the function call
            QFile::copy(book, "/inkbox/book/book.txt");

            filematch_ran = true;
            is_epub = false;
        }
    }

    // Checking whether the user has defined an option for the number of words per page; if not, then setting the default.
    // NOTE: This is only for plain text files.
    setDefaultWorkDir();
    string_checkconfig(".config/07-words_number/config");
    if(checkconfig_str_val == "") {
        string_writeconfig(".config/07-words_number/config", "100");
        string_checkconfig(".config/07-words_number/config");
    }

    // Parsing file
    if(is_epub == true) {
        if(run_parser == true) {
            if(filematch_ran == true) {
                QString epubProg ("sh");
                QStringList epubArgs;
                convertMuPdfVars(0, false);
                epubArgs << "/mnt/onboard/.adds/inkbox/epub.sh" << "0" << "0" << "0" << mupdf::epub::epubPageNumber_qstr;
                QProcess * epubProc = new QProcess();
                epubProc->start(epubProg, epubArgs);
                epubProc->waitForFinished();
                epubProc->deleteLater();
            }
        }
    }
    else if(is_pdf == true) {
        if(run_parser == true) {
            if(filematch_ran == true) {
                QString pdfProg("/usr/local/bin/mutool");
                QStringList pdfArgs;
                convertMuPdfVars(1, true);
                pdfArgs << "convert" << "-F" << "png" << "-O" << "width=" + mupdf::pdf::width_qstr + ",height=" + mupdf::pdf::height_qstr << "-o" << "/run/page.png" << book_file << mupdf::pdf::pdfPageNumber_qstr;
                QProcess * pdfProc = new QProcess();
                pdfProc->start(pdfProg, pdfArgs);
                pdfProc->waitForFinished();
                pdfProc->deleteLater();
            }
        }
    }
    else if(is_image == true) {
        // Do nothing, since images won't have pages
        ;
    }
    else {
        QString parse_prog ("python3");
        QStringList parse_args;
        parse_args << "split-txt.py" << checkconfig_str_val;
        QProcess * parse_proc = new QProcess();
        parse_proc->start(parse_prog, parse_args);
        parse_proc->waitForFinished();
        parse_proc->deleteLater();
    }

    // Reading files
    if(is_epub == false) {
        if(run_parser == true) {
            QDirIterator it("/inkbox/book/split");
            while (it.hasNext()) {
                  QFile f(it.next());
                  f.open(QIODevice::ReadOnly);
                  content << f.readAll();
                  f.close();
            }
            ittext = content[i];
            return content.size();
        }
        else {
            ittext = content[i];
        }
    }
    else {
        QFile epubPage("/inkbox/book/page");
        epubPage.open(QIODevice::ReadOnly);
        QTextStream in(&epubPage);
        epubPageContent = in.readAll();
        epubPage.close();
    }
    return 0;
}

void reader::checkwords() {
    QFile words_list(".config/06-words/config");
    words_list.open(QIODevice::ReadWrite);
    QTextStream in (&words_list);
    words = in.readAll();
    words_list.close();
}

bool reader::epub_file_match(QString file) {
    QString fileExt = file.right(4);

    if(fileExt == "epub" or fileExt == "EPUB") {
        string_writeconfig("/inkbox/bookIsEpub", "true");
        return true;
    }
    else {
        string_writeconfig("/inkbox/bookIsEpub", "false");
        return false;
    }
}

void reader::dictionary_lookup(string word, QString first_letter, int position) {
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
    QProcess * lookup_proc = new QProcess();
    lookup_proc->start(lookup_prog, lookup_args);
    lookup_proc->waitForFinished();
    lookup_proc->deleteLater();

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

    setDefaultWorkDir();
}

void reader::save_word(string word, bool remove) {
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

void reader::on_nextBtn_clicked()
{
    ui->nextBtn->setEnabled(false);
    if(is_epub == false and is_pdf == false) {
        if(split_total - 1 == 1 or split_total - 1 == 0) {
            showToast("You've reached the end of the document");
        }
        else {
            split_total = split_total - 1;

            setup_book(book_file, split_total, false);
            ui->text->setText("");
            ui->text->setText(ittext);

            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    else if(is_epub == true) {
        if(mupdf::epub::epubPageNumber + 1 > totalPagesInt) {
            showToast("You've reached the end of the document");
        }
        else {
            mupdf::epub::epubPageNumber = mupdf::epub::epubPageNumber + 1;
            setup_book(book_file, mupdf::epub::epubPageNumber, true);
            ui->text->setText("");
            ui->text->setText(epubPageContent);

            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    else if(is_pdf == true) {
        if(mupdf::pdf::pdfPageNumber + 1 > totalPagesInt) {
            showToast("You've reached the end of the document");
        }
        else {
            mupdf::pdf::pdfPageNumber = mupdf::pdf::pdfPageNumber + 1;
            setup_book(book_file, mupdf::pdf::pdfPageNumber, true);
            setupPng();

            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
        }
    }
    setupPageWidget();
    refreshScreen();
    ui->nextBtn->setEnabled(true);
}

void reader::on_previousBtn_clicked()
{
    ui->previousBtn->setEnabled(false);
    if(is_epub == false and is_pdf == false) {
    // Making sure we won't encounter a "List index out of range" error ;)
        if(split_total >= split_files_number - 1) {
            showToast("No previous page");
        }
        else {
            split_total = split_total + 1;
            setup_book(book_file, split_total, false);
            ui->text->setText("");
            ui->text->setText(ittext);

            // We always increment pagesTurned regardless whether we press the Previous or Next button
            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    else if (is_pdf == true) {
        if(mupdf::pdf::pdfPageNumber - 1 <= 0) {
            showToast("No previous page");
        }
        else {
            mupdf::pdf::pdfPageNumber = mupdf::pdf::pdfPageNumber - 1;
            setup_book(book_file, mupdf::pdf::pdfPageNumber, true);
            setupPng();

            // We always increment pagesTurned regardless whether we press the Previous or Next button
            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
        }
    }
    else {
        if(mupdf::epub::epubPageNumber - 1 <= 0) {
            showToast("No previous page");
        }
        else {
            mupdf::epub::epubPageNumber = mupdf::epub::epubPageNumber - 1;
            setup_book(book_file, mupdf::epub::epubPageNumber, true);
            ui->text->setText("");
            ui->text->setText(epubPageContent);

            // We always increment pagesTurned regardless whether we press the Previous or Next button
            pagesTurned = pagesTurned + 1;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    setupPageWidget();
    refreshScreen();
    ui->previousBtn->setEnabled(true);
}

void reader::refreshScreen() {
    if(neverRefresh == true) {
        // Do nothing; "Never refresh" was set
        ;
    }
    else {
        if(pagesTurned >= pageRefreshSetting) {
            // Refreshing the screen
            this->repaint();
            // Reset count
            pagesTurned = 0;
        }
    }
}

void reader::on_optionsBtn_clicked()
{
    if(menubar_shown == true) {
        menubar_hide();
        if(readFile("/opt/inkbox_device") == "n873\n") {
            ui->optionsBtn->setStyleSheet("background: white; color: black; padding: 13.5px");
        }
        else {
            ui->optionsBtn->setStyleSheet("background: white; color: black");
        }
        this->repaint();
        menubar_shown = false;
    }
    else {
        menubar_show();
        if(readFile("/opt/inkbox_device") == "n873\n") {
            ui->optionsBtn->setStyleSheet("background: black; color: white; padding: 13.5px");
        }
        else {
            ui->optionsBtn->setStyleSheet("background: black; color: white");
        }
        this->repaint();
        menubar_shown = true;
    }
}

void reader::on_brightnessDecBtn_clicked()
{
    int bval;
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        bval = get_brightness();
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        bval = brightness_checkconfig(".config/03-brightness/config");
    }
    else {
        bval = get_brightness();
    }
    int set_bval = bval - 1;
    if(set_bval < 0) {
        set_bval = 0;
    }
    pre_set_brightness(set_bval);
    brightness_writeconfig(set_bval);

    ui->brightnessStatus->setValue(set_bval);
}

void reader::on_brightnessIncBtn_clicked()
{
    int bval;
    if(global::isN705 == true or global::isN905C == true or global::isN873 == true) {
        bval = get_brightness();
    }
    else if(global::isN613 == true) {
        setDefaultWorkDir();
        bval = brightness_checkconfig(".config/03-brightness/config");
    }
    else {
        bval = get_brightness();
    }
    int set_bval = bval + 1;
    if(set_bval > 100) {
        set_bval = 100;
    }
    pre_set_brightness(set_bval);
    brightness_writeconfig(set_bval);

    ui->brightnessStatus->setValue(set_bval);
}

void reader::on_aboutBtn_clicked()
{
    if(checkconfig("/opt/inkbox_genuine") == true) {
        QString aboutmsg = "InkBox is an open-source, Qt-based eBook reader. It aims to bring you the latest Qt features while being also fast and responsive.";
        aboutmsg.prepend("<font face='u001'>");
        string_checkconfig_ro("/external_root/opt/isa/version");
        aboutmsg.append("<br><br>InkBox ");
        aboutmsg.append(checkconfig_str_val);
        aboutmsg.append("</font>");
        QMessageBox::information(this, tr("Information"), aboutmsg);
    }
    else {
        QMessageBox::information(this, tr("About"), tr("InkBox is an open-source Qt-based eBook reader. It aims to bring you the latest Qt features while being also fast and responsive."));
    }
}

void reader::on_homeBtn_clicked()
{
    // We're leaving reading mode
    string_writeconfig("/tmp/inkboxReading", "false");
    // Remount tmpfs
    string_writeconfig("/inkbox/remount", "true");

    // Relaunching process
    quit_restart();
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
    if(arg1 == "Crimson Pro") {
        // As adding Crimson Pro to the default fonts bundled along with the Qt libs breaks the general Inter homogeneity, it is incorporated on-demand here.
        setCrimsonProFont();
        string_writeconfig(".config/04-book/font", "Crimson Pro");
    }
    if(arg1 == "Bitter") {
        setBitterFont();
        string_writeconfig(".config/04-book/font", "Bitter");
    }
    if(arg1 == "Ibarra Real Nova") {
        setIbarraFont();
        string_writeconfig(".config/04-book/font", "Ibarra Real Nova");
    }
}

void reader::on_alignLeftBtn_clicked()
{
    if(is_epub != true) {
        ui->text->setAlignment(Qt::AlignLeft);
    }
    else {
        alignText(0);
    }
    string_writeconfig(".config/04-book/alignment", "Left");
}

void reader::on_alignCenterBtn_clicked()
{
    if(is_epub != true) {
        ui->text->setAlignment(Qt::AlignHCenter);
    }
    else {
        alignText(1);
    }
    string_writeconfig(".config/04-book/alignment", "Center");
}

void reader::on_alignRightBtn_clicked()
{
    if(is_epub != true) {
        ui->text->setAlignment(Qt::AlignRight);
    }
    else {
        alignText(2);
    }
    string_writeconfig(".config/04-book/alignment", "Right");
}

void reader::on_alignJustifyBtn_clicked()
{
    if(is_epub != true) {
        ui->text->setAlignment(Qt::AlignJustify);
    }
    else {
        alignText(3);
    }
    string_writeconfig(".config/04-book/alignment", "Justify");
}

void reader::alignText(int alignment) {
    /*
     * 0 - Left
     * 1 - Center
     * 2 - Right
     * 3 - Justify
    */
    textAlignment = alignment;
    if(is_epub == true) {
        if(alignment == 0) {
            QString epubPageContent_alignChange = epubPageContent;
            epubPageContent_alignChange.prepend("<div align='left'>");
            epubPageContent_alignChange.append("</div>");
            ui->text->setText(epubPageContent_alignChange);
        }
        if(alignment == 1) {
            QString epubPageContent_alignChange = epubPageContent;
            epubPageContent_alignChange.prepend("<div align='center'>");
            epubPageContent_alignChange.append("</div>");
            ui->text->setText(epubPageContent_alignChange);
        }
        if(alignment == 2) {
            QString epubPageContent_alignChange = epubPageContent;
            epubPageContent_alignChange.prepend("<div align='right'>");
            epubPageContent_alignChange.append("</div>");
            ui->text->setText(epubPageContent_alignChange);
        }
        if(alignment == 3) {
            QString epubPageContent_alignChange = epubPageContent;
            epubPageContent_alignChange.prepend("<div align='justify'>");
            epubPageContent_alignChange.append("</div>");
            ui->text->setText(epubPageContent_alignChange);
        }
    }
    else {
        if(alignment == 0) {
            QString ittext_alignChange = ittext;
            ittext_alignChange.prepend("<div align='left'>");
            ittext_alignChange.append("</div>");
            ui->text->setText(ittext_alignChange);
        }
        if(alignment == 1) {
            QString ittext_alignChange = ittext;
            ittext_alignChange.prepend("<div align='center'>");
            ittext_alignChange.append("</div>");
            ui->text->setText(ittext_alignChange);
        }
        if(alignment == 2) {
            QString ittext_alignChange = ittext;
            ittext_alignChange.prepend("<div align='right'>");
            ittext_alignChange.append("</div>");
            ui->text->setText(ittext_alignChange);
        }
        if(alignment == 3) {
            QString ittext_alignChange = ittext;
            ittext_alignChange.prepend("<div align='justify'>");
            ittext_alignChange.append("</div>");
            ui->text->setText(ittext_alignChange);
        }
    }
}

void reader::menubar_show() {
    // Checking battery level and status, then displaying the relevant icon on batteryIconLabel
    if(isUsbPluggedIn() == true) {
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

    ui->menuWidget->setVisible(true);
    if(is_pdf == false && is_image == false) {
        ui->menuBarWidget->setVisible(true);
    }
    else {
        if(is_pdf == true) {
            ui->pdfScaleWidget->setVisible(true);
        }
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    ui->buttonsBarWidget->setVisible(true);
    ui->statusBarWidget->setVisible(true);
    if(is_image == true) {
        ui->pageWidget->setVisible(false);
    }
    else {
        ui->pageWidget->setVisible(true);
    }

    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        ;
    }
    else {
        ui->brightnessWidget->setVisible(true);
    }

    menubar_shown = true;
}

void reader::menubar_hide() {
    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        ui->brightnessWidget->setVisible(false);
    }
    else {
        // Safety measure
        ui->brightnessWidget->setVisible(false);
    }

    if(is_pdf == false && is_image == false) {
        ui->menuBarWidget->setVisible(false);
    }
    else {
        ui->pdfScaleWidget->setVisible(false);
        if(checkconfig(".config/14-reader_scrollbar/config") == true) {
            ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else {
            ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    ui->buttonsBarWidget->setVisible(false);
    ui->pageWidget->setVisible(false);
    if(checkconfig(".config/11-menubar/sticky") == true) {
        ui->statusBarWidget->setVisible(true);
    }
    else {
        ui->statusBarWidget->setVisible(false);
        ui->menuWidget->setVisible(false);
    }
    menubar_shown = false;
}

void reader::wordwidget_show() {
    if(menubar_shown == true) {
        menubar_hide();
        ui->optionsBtn->hide();
        ui->line->hide();
        ui->wordWidget->setVisible(true);
    }
    else {
        ui->optionsBtn->hide();
        ui->line->hide();
        ui->wordWidget->setVisible(true);
    }
}

void reader::wordwidget_hide() {
    ui->wordWidget->setVisible(false);
    ui->optionsBtn->setStyleSheet("background: white; color: black");
    ui->optionsBtn->show();
    ui->line->show();
    wordwidgetLock = false;
}

void reader::on_infoCloseBtn_clicked()
{
    wordwidget_hide();
    dictionary_position = 1;
    QString dictionary_position_str = QString::number(dictionary_position);
    ui->definitionStatusLabel->setText(dictionary_position_str);

    QTextCursor cursor = ui->text->textCursor();
    cursor.clearSelection();
    ui->text->setTextCursor(cursor);
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
    if(checkconfig_match("/opt/inkbox_device", "n905\n") == true) {
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

void reader::writeconfig_pagenumber(bool persistent) {
    // Saving the page number in tmpfs and in persistent storage if requested
    if(is_epub == true) {
        std::string epubPageNumber_str = std::to_string(mupdf::epub::epubPageNumber);
        string_writeconfig("/tmp/inkboxPageNumber", epubPageNumber_str);
        if(persistent == true) {
            epubPageNumber_str.append("\n");
            string_writeconfig(".config/A-page_number/config", epubPageNumber_str);
        }
    }
    else if(is_pdf == true) {
        std::string pdfPageNumber_str = std::to_string(mupdf::pdf::pdfPageNumber);
        string_writeconfig("/tmp/inkboxPageNumber", pdfPageNumber_str);
        if(persistent == true) {
            pdfPageNumber_str.append("\n");
            string_writeconfig(".config/A-page_number/config", pdfPageNumber_str);
        }
    }
    else {
        std::string split_total_str = std::to_string(split_total);
        string_writeconfig("/tmp/inkboxPageNumber", split_total_str);
        if(persistent == true) {
            split_total_str.append("\n");
            string_writeconfig(".config/A-page_number/config", split_total_str);
        }
    }
}

void reader::quit_restart() {
    // Saving current page number
    saveReadingSettings();

    // Cleaning bookconfig_mount mountpoint
    string_writeconfig("/opt/ibxd", "bookconfig_unmount\n");

    // Restarting InkBox
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void reader::openLowBatteryDialog() {
    global::mainwindow::lowBatteryDialog = true;
    global::battery::batteryAlertLock = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QApplication::processEvents();
}

void reader::openCriticalBatteryAlertWindow() {
    global::battery::showCriticalBatteryAlert = true;
    global::battery::showLowBatteryDialog = false;

    alertWindow = new alert();
    alertWindow->setAttribute(Qt::WA_DeleteOnClose);
    alertWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    alertWindow->show();
}

void reader::convertMuPdfVars(int fileType, bool convertAll) {
    /* fileType can be:
     * 0: ePUB
     * 1: PDF
     * 2: Image
    */
    if(fileType == 0) {
        if(convertAll == 1) {
            setPageStyle(0);
            mupdf::epub::fontSize = 12;
            mupdf::epub::fontSize_qstr = QString::number(mupdf::epub::fontSize);
            mupdf::epub::width_qstr = QString::number(mupdf::epub::width);
            mupdf::epub::height_qstr = QString::number(mupdf::epub::height);
        }
        if(global::reader::globalReadingSettings == false) {
            if(goToSavedPageDone == false) {
                mupdf::epub::epubPageNumber = readFile(".config/A-page_number/config").toInt();
                goToSavedPageDone = true;
            }
        }
        if(mupdf::epub::epubPageNumber <= 0) {
            mupdf::epub::epubPageNumber = 1;
        }
        mupdf::epub::epubPageNumber_qstr = QString::number(mupdf::epub::epubPageNumber);
    }
    else if(fileType == 1) {
        setPageStyle(1);
        mupdf::pdf::width = defaultPdfPageWidth;
        mupdf::pdf::height = defaultPdfPageHeight;
        if(mupdf::convertRelativeValues == true) {
            // For scaling
            mupdf::pdf::width_qstr = QString::number(mupdf::pdf::relativeWidth);
            mupdf::pdf::height_qstr = QString::number(mupdf::pdf::relativeHeight);
        }
        else {
            // Default
            mupdf::pdf::width_qstr = QString::number(mupdf::pdf::width);
            mupdf::pdf::height_qstr = QString::number(mupdf::pdf::height);
        }
        if(global::reader::globalReadingSettings == false) {
            if(goToSavedPageDone == false) {
                string_checkconfig_ro(".config/A-page_number/config");
                mupdf::pdf::pdfPageNumber = checkconfig_str_val.toInt();
                goToSavedPageDone = true;
            }
        }
        if(mupdf::pdf::pdfPageNumber <= 0) {
            mupdf::pdf::pdfPageNumber = 1;
        }
        mupdf::pdf::pdfPageNumber_qstr = QString::number(mupdf::pdf::pdfPageNumber);
    }
    else if(fileType == 2) {
        ;
    }
    if(mupdf::convertRelativeValues == true) {
        mupdf::convertRelativeValues = false;
    }
}

void reader::setPageStyle(int fileType) {
    /* fileType can be:
     * 0: ePUB
     * 1: PDF
     * 2: Image
    */
    if(fileType == 0) {
        // General page size
        defineDefaultPageSize(0);

        string_checkconfig_ro(".config/13-epub_page_size/width");
        if(checkconfig_str_val != "") {
            ;
        }
        else {
            std::string pageWidth = std::to_string(defaultEpubPageWidth);
            string_writeconfig(".config/13-epub_page_size/width", pageWidth);
            string_writeconfig(".config/13-epub_page_size/set", "true");
            string_checkconfig_ro(".config/13-epub_page_size/width");
        }
        mupdf::epub::width = checkconfig_str_val.toInt();

        string_checkconfig_ro(".config/13-epub_page_size/height");
        if(checkconfig_str_val != "") {
            ;
        }
        else {
            std::string pageHeight = std::to_string(defaultEpubPageHeight);
            string_writeconfig(".config/13-epub_page_size/height", pageHeight);
            string_writeconfig(".config/13-epub_page_size/set", "true");
            string_checkconfig_ro(".config/13-epub_page_size/height");
        }
        mupdf::epub::height = checkconfig_str_val.toInt();
    }
    else if(fileType == 1) {
        defineDefaultPageSize(1);
    }
}

void reader::delay(int mseconds) {
    // https://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
    QTime dieTime= QTime::currentTime().addMSecs(mseconds);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void reader::on_text_selectionChanged() {
    delay(100);
    if(wordwidgetLock != true) {
        QTextCursor cursor = ui->text->textCursor();
        selected_text = cursor.selectedText();
        if(selected_text != "") {
            QString dictionary_position_str = QString::number(dictionary_position);
            ui->definitionStatusLabel->setText(dictionary_position_str);

            selected_text = selected_text.toLower();
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
            wordwidgetLock = true;
            wordwidget_show();
        }
        else {
            ;
        }
    }
}

void reader::on_nightModeBtn_clicked()
{
    if(isNightModeActive == true) {
        // Disabling night/dark mode
        string_writeconfig("/tmp/invertScreen", "n");
        string_writeconfig(".config/10-dark_mode/config", "false");
        ui->nightModeBtn->setIcon(QIcon(":/resources/nightmode-empty.png"));
        isNightModeActive = false;
    }
    else {
        // Enabling night/dark mode
        string_writeconfig("/tmp/invertScreen", "y");
        string_writeconfig(".config/10-dark_mode/config", "true");
        ui->nightModeBtn->setIcon(QIcon(":/resources/nightmode-full.png"));
        isNightModeActive = true;
    }
}

void reader::openUsbmsDialog() {
    global::usbms::showUsbmsDialog = false;
    global::usbms::usbmsDialog = true;

    generalDialogWindow = new generalDialog(this);
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    QApplication::processEvents();
}

QString reader::setPageNumberLabelContent() {
    QString content;
    if(is_epub == true) {
        QString pageNumber;
        QString totalPages;
        pageNumberInt = mupdf::epub::epubPageNumber;
        pageNumber = QString::number(pageNumberInt);
        totalPages = QString::number(totalPagesInt);
        content.append(pageNumber);
        content.append(" <i>of</i> ");
        content.append(totalPages);
    }
    else if(is_pdf == true) {
        QString pageNumber;
        QString totalPages;
        pageNumberInt = mupdf::pdf::pdfPageNumber;
        pageNumber = QString::number(pageNumberInt);
        totalPages = QString::number(totalPagesInt);
        content.append(pageNumber);
        content.append(" <i>of</i> ");
        content.append(totalPages);
    }
    else {
        QString pageNumber;
        QString totalPages;
        pageNumberInt = split_files_number - split_total;
        totalPagesInt = split_files_number - 2;
        pageNumber = QString::number(pageNumberInt);
        totalPages = QString::number(totalPagesInt);
        content.append(pageNumber);
        content.append(" <i>of</i> ");
        content.append(totalPages);
    }
    return content;
}

void reader::setupPageWidget() {
    QString pageNumberInfoLabelContent = setPageNumberLabelContent();
    ui->pageNumberLabel->setText(pageNumberInfoLabelContent);
    ui->pageProgressBar->setMaximum(totalPagesInt);
    ui->pageProgressBar->setMinimum(1);
    ui->pageProgressBar->setValue(pageNumberInt);
}

void reader::getTotalEpubPagesNumber() {
    QString epubProg ("sh");
    QStringList epubArgs;
    convertMuPdfVars(0, true);
    epubArgs << "/mnt/onboard/.adds/inkbox/epub.sh" << mupdf::epub::fontSize_qstr << mupdf::epub::width_qstr << mupdf::epub::height_qstr << mupdf::epub::epubPageNumber_qstr << "get_pages_number";
    QProcess *epubProc = new QProcess();
    epubProc->start(epubProg, epubArgs);
    epubProc->waitForFinished();
    epubProc->deleteLater();

    string_checkconfig_ro("/run/epub_total_pages_number");
    totalPagesInt = checkconfig_str_val.toInt();
    QFile::remove("/run/epub_total_pages_number");
}

void reader::on_gotoBtn_clicked()
{
    global::keyboard::keypadDialog = true;
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(gotoPageSelected(int)), SLOT(gotoPage(int)));
}

void reader::gotoPage(int pageNumber) {
    if(is_epub == true) {
        if(pageNumber > totalPagesInt or pageNumber < 1) {
            showToast("Request is beyond page range");
        }
        else {
            mupdf::epub::epubPageNumber = pageNumber;
            setup_book(book_file, mupdf::epub::epubPageNumber, true);
            ui->text->setText("");
            ui->text->setText(epubPageContent);

            pagesTurned = 0;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    else if(is_pdf == true) {
        if(pageNumber > totalPagesInt or pageNumber < 1) {
            showToast("Request is beyond page range");
        }
        else {
            mupdf::pdf::pdfPageNumber = pageNumber;
            setup_book(book_file, mupdf::pdf::pdfPageNumber, true);
            setupPng();

            pagesTurned = 0;
            writeconfig_pagenumber(false);
        }
    }
    else {
        if(split_files_number - pageNumber < 2 or split_files_number - pageNumber > split_files_number - 1) {
            showToast("You've reached the end of the document");
        }
        else {
            split_total = split_files_number - pageNumber;

            setup_book(book_file, split_total, false);
            ui->text->setText("");
            ui->text->setText(ittext);

            pagesTurned = 0;
            writeconfig_pagenumber(false);
            alignText(textAlignment);
        }
    }
    setupPageWidget();
    refreshScreen();
}

void reader::on_searchBtn_clicked()
{
    global::forbidOpenSearchDialog = false;
    setupSearchDialog();
}

void reader::setupSearchDialog() {
    if(global::forbidOpenSearchDialog == false) {
        global::keyboard::keyboardDialog = true;
        global::keyboard::searchDialog = true;
        global::keyboard::keyboardText = "";
        generalDialogWindow = new generalDialog();
        generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(searchRefreshScreen()));
        connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupSearchDialog()));
        connect(generalDialogWindow, SIGNAL(openBookFile(QString, bool)), SLOT(openBookFileNative(QString, bool)));
        connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
        connect(generalDialogWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToast()));
        generalDialogWindow->show();
    }
    else {
        ;
    }
}

void reader::setBitterFont() {
    QString family;
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/Bitter-Medium.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/Bitter-MediumItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/Bitter-Bold.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/Bitter-BoldItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    QFont bitter(family);
    ui->text->setFont(bitter);
    ui->fontChooser->setCurrentText("Bitter");
}

void reader::setCrimsonProFont() {
    QString family;
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Regular.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Italic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Bold.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-BoldItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    QFont crimson(family);
    ui->text->setFont(crimson);
    ui->fontChooser->setCurrentText("Crimson Pro");
}

void reader::setIbarraFont() {
    QString family;
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/IbarraRealNova-Medium.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/IbarraRealNova-MediumItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/IbarraRealNova-Bold.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    {
        int id = QFontDatabase::addApplicationFont(":/resources/fonts/IbarraRealNova-BoldItalic.ttf");
        family = QFontDatabase::applicationFontFamilies(id).at(0);
    }
    QFont ibarra(family);
    ui->text->setFont(ibarra);
    ui->fontChooser->setCurrentText("Ibarra Real Nova");
}

void reader::searchRefreshScreen() {
    this->repaint();
}

void reader::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    toastWindow->show();
}

void reader::saveReadingSettings() {
    writeconfig_pagenumber(true);
}

void reader::setupLocalSettingsEnvironment() {
    QString pageNumberDirPath = ".config/A-page_number";
    QDir pageNumberDir;
    pageNumberDir.mkpath(pageNumberDirPath);
}

void reader::setupPng() {
    if(is_pdf == true) {
        // Note: Output file is supposed to be '/run/page.png', but somehow mutool puts it in '/run/page1.png'
        QPixmap pixmap("/run/page1.png");
        QFile::remove("/run/page1.png");
        // Initialized above
        graphicsScene->clear();
        graphicsScene->addPixmap(pixmap);
        // Shrinking scene if item is smaller than previous one
        QRectF rect = graphicsScene->itemsBoundingRect();
        graphicsScene->setSceneRect(rect);
        ui->graphicsView->items().clear();
        ui->graphicsView->setScene(graphicsScene);
    }
    else if(is_image == true) {
        QPixmap pixmap("/run/image.png");
        QFile::remove("/run/image.png");
        // Initialized above
        graphicsScene->clear();
        graphicsScene->addPixmap(pixmap);
        // Shrinking scene if item is smaller than previous one
        QRectF rect = graphicsScene->itemsBoundingRect();
        graphicsScene->setSceneRect(rect);
        ui->graphicsView->items().clear();
        ui->graphicsView->setScene(graphicsScene);
    }
}

bool reader::pdf_file_match(QString file) {
    QString fileExt = file.right(3);

    if(fileExt == "pdf" or fileExt == "PDF") {
        string_writeconfig("/inkbox/bookIsPdf", "true");
        return true;
    }
    else {
        string_writeconfig("/inkbox/bookIsPdf", "false");
        return false;
    }
}

bool reader::image_file_match(QString file) {
    if(file.right(3) == "png" or file.right(3) == "PNG" or file.right(3) == "jpg" or file.right(3) == "JPG" or file.right(3) == "bmp" or file.right(3) == "BMP" or file.right(3) == "tif" or file.right(3) == "TIF") {
        string_writeconfig("/inkbox/bookIsImage", "true");
        return true;
    }
    else if(file.right(4) == "jpeg" or file.right(4) == "JPEG" or file.right(4) == "tiff" or file.right(4) == "TIFF") {
        string_writeconfig("/inkbox/bookIsImage", "true");
        return true;
    }
    else {
        string_writeconfig("/inkbox/bookIsImage", "false");
        return false;
    }
}

void reader::getTotalPdfPagesNumber() {
    QString epubProg ("sh");
    QStringList epubArgs;
    convertMuPdfVars(0, true);
    epubArgs << "/mnt/onboard/.adds/inkbox/pdf_get_total_pages_number.sh" << book_file;
    QProcess * epubProc = new QProcess();
    epubProc->start(epubProg, epubArgs);
    epubProc->waitForFinished();
    epubProc->deleteLater();

    string_checkconfig_ro("/run/pdf_total_pages_number");
    totalPagesInt = checkconfig_str_val.toInt();
    QFile::remove("/run/pdf_total_pages_number");
}

void reader::on_pdfScaleSlider_valueChanged(int value)
{
    if(value == 1) {
        mupdf::pdf::relativeWidth = 1 * mupdf::pdf::width;
        mupdf::pdf::relativeHeight = 1 * mupdf::pdf::height;
    }
    else if(value == 2) {
        mupdf::pdf::relativeWidth = 1.50 * mupdf::pdf::width;
        mupdf::pdf::relativeHeight = 1.50 * mupdf::pdf::height;
    }
    else if(value == 3) {
        mupdf::pdf::relativeWidth = 2 * mupdf::pdf::width;
        mupdf::pdf::relativeHeight = 2 * mupdf::pdf::height;
    }
    else if(value == 4) {
        mupdf::pdf::relativeWidth = 2.50 * mupdf::pdf::width;
        mupdf::pdf::relativeHeight = 2.50 * mupdf::pdf::height;
    }

    if(value != 1) {
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else {
        if(checkconfig(".config/14-reader_scrollbar/config") == true) {
            ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else {
            ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    mupdf::convertRelativeValues = true;
    setup_book(book_file, mupdf::pdf::pdfPageNumber, true);
    setupPng();
}

void reader::on_decreaseScaleBtn_clicked()
{
    int sliderCurrentValue;
    int sliderWantedValue;
    sliderCurrentValue = ui->pdfScaleSlider->value();
    sliderWantedValue = sliderCurrentValue - 1;
    if(sliderWantedValue < ui->pdfScaleSlider->QAbstractSlider::minimum()) {
        showToast("Minimum scale reached");
    }
    else {
        ui->pdfScaleSlider->setValue(sliderWantedValue);
    }
}

void reader::on_increaseScaleBtn_clicked()
{
    int sliderCurrentValue;
    int sliderWantedValue;
    sliderCurrentValue = ui->pdfScaleSlider->value();
    sliderWantedValue = sliderCurrentValue + 1;
    if(sliderWantedValue > ui->pdfScaleSlider->QAbstractSlider::maximum()) {
        showToast("Maximum scale reached");
    }
    else {
        ui->pdfScaleSlider->setValue(sliderWantedValue);
    }
}

void reader::openBookFileNative(QString book, bool relativePath) {
    if(global::runningInstanceIsReaderOnly == false) {
        emit openBookFile(book, relativePath);
    }
    else {
        showToast("Not supported");
    }
}

void reader::on_quitBtn_clicked()
{
    quitWindow = new quit();
    quitWindow->setAttribute(Qt::WA_DeleteOnClose);
    quitWindow->showFullScreen();
}
