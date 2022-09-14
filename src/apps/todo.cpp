#include "todo.h"
#include "ui_todo.h"

#include "functions.h"

todo::todo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::todo)
{
    ui->setupUi(this);
    this->setStyleSheet(readFile("/mnt/onboard/.adds/inkbox/eink.qss"));
    this->setFont(QFont("u001"));

    ui->closeBtn->setProperty("type", "borderless");
    ui->newListBtn->setProperty("type", "borderless");
    ui->deleteBtn->setProperty("type", "borderless");
    ui->setupBtn->setProperty("type", "borderless");
    ui->deleteBtn->setStyleSheet("padding: 10px");
    ui->setupBtn->setStyleSheet("padding: 10px");
    ui->closeBtn->setIcon(QIcon(":/resources/close.png"));
    ui->newListBtn->setIcon(QIcon(":/resources/new.png"));
    ui->deleteBtn->setIcon(QIcon(":/resources/x-circle.png"));
    ui->setupBtn->setIcon(QIcon(":/resources/arrow-right.png"));

    refreshList();
}

todo::~todo()
{
    delete ui;
}

void todo::on_closeBtn_clicked()
{
    this->close();
}


void todo::on_newListBtn_clicked()
{
    global::keyboard::embed = false;
    virtualkeyboard * virtualKeyboardWidget = new virtualkeyboard(this);
    virtualKeyboardWidget->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(virtualKeyboardWidget, &virtualkeyboard::enterBtnPressed, this, &todo::createNewList);
    virtualKeyboardWidget->show();
    ui->statusLabel->setText("Enter the list's name");
}

void todo::createNewList(QString listName) {
    log("Initializing new To-Do list with name '" + listName + "'", className);
    QJsonDocument document;
    QJsonArray mainArray;
    QJsonArray listArray;
    QJsonObject object;
    if(QFile::exists(global::localLibrary::todoDatabasePath)) {
        document = readTodoDatabase();
        object = document.object();
        mainArray = object["List"].toArray();
    }
    listArray.append(listName);

    mainArray.push_back(listArray);
    object["List"] = mainArray;

    document.setObject(object);
    writeTodoDatabase(document);

    ui->statusLabel->setText("Select or create a new list");
    refreshList();
}

void todo::refreshList() {
    if(QFile::exists(global::localLibrary::todoDatabasePath)) {
        ui->listWidget->clear();
        QJsonArray array = readTodoDatabase().object()["List"].toArray();
        for(int i = 0; i < array.count(); i++) {
            QString name = array.at(i).toArray().at(0).toString();
            if(!name.isEmpty()) {
                ui->listWidget->addItem(name);
            }
        }
    }
}

void todo::setupList(QString listName) {
    log("Setting up list with name '" + listName + "'", className);
    QJsonArray array = readTodoDatabase().object()["List"].toArray();
    int index;
    for(int i = 0; i < array.count(); i++) {
        QString name = array.at(i).toArray().at(0).toString();
        log("name is " + name);
        if(name == listName) {
            index = i;
            break;
        }
    }
    log("List index is " + QString::number(index), className);
}

void todo::on_setupBtn_clicked()
{
    ui->deleteBtn->setEnabled(false);
    ui->setupBtn->setEnabled(false);
    setupList(ui->listWidget->currentItem()->text());
}


void todo::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->deleteBtn->setEnabled(true);
    ui->setupBtn->setEnabled(true);
}
