#include "koboxappsdialog.h"
#include "ui_koboxappsdialog.h"

#include "functions.h"

#include <QStringListModel>
#include <QScreen>

koboxAppsDialog::koboxAppsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::koboxAppsDialog)
{
    ui->setupUi(this);

    // Preventing outside interaction
    // this->setModal(true);
    // For some obscure reason, this returns a "no member named setModal in 'koboxAppsDialog' error"
    // Instead, I set the modality via GUI in the "Forms" section of Qt Creator

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->launchBtn->setProperty("type", "borderless");
    ui->cancelBtn->setProperty("type", "borderless");
    ui->launchBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->cancelBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");

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
    QFile apps_list("/external_root/opt/X11/extensions/extensions_list");
    apps_list.open(QIODevice::ReadWrite);
    QTextStream in (&apps_list);
    apps = in.readAll();
    apps_list.close();
}

koboxAppsDialog::~koboxAppsDialog()
{
    delete ui;
}
