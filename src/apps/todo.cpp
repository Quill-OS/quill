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
    ui->closeBtn->setIcon(QIcon(":/resources/close.png"));
    ui->newListBtn->setIcon(QIcon(":/resources/new.png"));

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
    int index = 0;
    if(QFile::exists(global::localLibrary::todoDatabasePath)) {
        document = readTodoDatabase();
        QJsonObject object = document.object();
        index = object.length();
        if(!(index <= object[QString::number(index)].toObject().value("Name").toInt())) {
            // TODO
            index++;
        }
    }
    QJsonObject mainObject = document.object();
    QJsonObject listObject;

    listObject.insert("Name", listName);
    mainObject.insert(QString::number(index), listObject);

    document.setObject(mainObject);
    writeTodoDatabase(document);

    ui->statusLabel->setText("Select or create a new list");
    refreshList();
}

void todo::refreshList() {
    if(QFile::exists(global::localLibrary::todoDatabasePath)) {
        ui->listWidget->clear();
        QJsonObject mainObject = readTodoDatabase().object();
        for(int i = 0; i < mainObject.length(); i++) {
            QString name = mainObject[QString::number(i)].toObject().value("Name").toString();
            if(!name.isEmpty()) {
                ui->listWidget->addItem(name);
            }
        }
    }
}
