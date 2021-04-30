#include "koboxappsdialog.h"
#include "ui_koboxappsdialog.h"

#include "functions.h"

#include <QStringListModel>
#include <QScreen>
#include <QDebug>
#include <QFontDatabase>
#include <QMessageBox>

koboxAppsDialog::koboxAppsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::koboxAppsDialog)
{
    ui->setupUi(this);

    // Preventing outside interaction
    this->setModal(true);

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->launchBtn->setProperty("type", "borderless");
    ui->cancelBtn->setProperty("type", "borderless");
    ui->launchBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->cancelBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->appsList->setStyleSheet("font-size: 9pt");
    ui->headerLabel->setStyleSheet("font-weight: bold");

    // UI fonts
    int id = QFontDatabase::addApplicationFont(":/resources/fonts/CrimsonPro-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont crimson(family);
    ui->definitionLabel->setFont(QFont(crimson));

    this->adjustSize();
    // Centering dialog
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    checkApps();
    QStringListModel* model = new QStringListModel(this);
    QStringList list = apps.split("\n", QString::SkipEmptyParts);
    model->setStringList(list);
    ui->appsList->setModel(model);
    ui->appsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void koboxAppsDialog::checkApps() {
    QFile apps_list("/external_root/opt/X11/extensions_list");
    apps_list.open(QIODevice::ReadWrite);
    QTextStream in (&apps_list);
    apps = in.readAll();
    apps_list.close();
}

koboxAppsDialog::~koboxAppsDialog()
{
    delete ui;
}

void koboxAppsDialog::on_cancelBtn_clicked()
{
    // Emergency TODO: Fix this giving a "free(): invalid next size(fast)..." error
    koboxAppsDialog::close();
}

void koboxAppsDialog::on_launchBtn_clicked()
{
    index = ui->appsList->currentIndex();
    itemText = index.data(Qt::DisplayRole).toString();
    if(itemText == "") {
        QMessageBox::critical(this, tr("Invalid argument"), tr("Please select an application."));
    }
    else {
        qDebug() << itemText;
    }
}
