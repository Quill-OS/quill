#include "savedwords.h"
#include "ui_savedwords.h"
#include <QStringListModel>
#include <QFile>
#include <QProcess>

savedwords::savedwords(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::savedwords)
{
    ui->setupUi(this);
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();
    ui->backBtn->setProperty("type", "borderless");
    ui->clearBtn->setProperty("type", "borderless");

    checkwords();

    QStringListModel * model = new QStringListModel(this);
    QStringList list = words.split("\n", QString::SkipEmptyParts);
    model->setStringList(list);
    ui->wordsList->setModel(model);
    ui->wordsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

savedwords::~savedwords()
{
    delete ui;
}

void savedwords::on_backBtn_clicked()
{
    // Workaround for the random strange memory corruption error
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void savedwords::on_clearBtn_clicked()
{
    // Warning: possible memory leak here. Though, usually, when you press the "Clear" button and all clears up, you don't have to press it again ;)
    save_word_init();
    checkwords();
    QStringListModel* model = new QStringListModel(this);
    QStringList list = words.split("\n", QString::SkipEmptyParts);
    model->setStringList(list);
    ui->wordsList->setModel(model);
    ui->wordsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void savedwords::checkwords() {
    QFile words_list(".config/06-words/config");
    words_list.open(QIODevice::ReadWrite);
    QTextStream in (&words_list);
    words = in.readAll();
    words_list.close();
}

void savedwords::save_word_init() {
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
