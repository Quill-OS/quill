#include "settingschooser.h"
#include "ui_settingschooser.h"
#include "functions.h"

#include <QFile>
#include <QScreen>

settingsChooser::settingsChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsChooser)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    string_checkconfig("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n") {
        homeIconWidth = sW / 18;
        homeIconHeight = sW / 18;
    }
    else {
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
    }

    ui->inkboxSettingsBtn->setProperty("type", "borderless");
    ui->koboxSettingsBtn->setProperty("type", "borderless");

    ui->inkboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
    ui->koboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
    ui->inkboxSettingsBtn->setText("\t\t\tInkBox settings");
    ui->koboxSettingsBtn->setText("\t\t\tKoBox settings");

    ui->inkboxSettingsBtn->setIcon(QIcon(":/resources/settings.png"));
    ui->inkboxSettingsBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));
    ui->koboxSettingsBtn->setIcon(QIcon(":/resources/X11.png"));
    ui->koboxSettingsBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));
}

settingsChooser::~settingsChooser()
{
    delete ui;
}

void settingsChooser::on_inkboxSettingsBtn_clicked()
{
        settingsWindow = new settings();
        settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
        settingsWindow->showFullScreen();
}
