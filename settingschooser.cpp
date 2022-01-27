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

    // UI tweaks
    if(checkconfig("/opt/inkbox_kobox_support") == false) {
        ui->koboxSettingsBtn->hide();
        ui->line_3->hide();
    }

    // Getting the screen's size
    sW = QGuiApplication::screens()[0]->size().width();
    sH = QGuiApplication::screens()[0]->size().height();

    // Defining what the default icon size will be
    string_checkconfig_ro("/opt/inkbox_device");
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

    string_checkconfig_ro("/opt/inkbox_device");
    if(checkconfig_str_val == "n705\n" or checkconfig_str_val == "n905\n") {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 20px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding: 20px; Text-align: left");
    }
    else if(checkconfig_str_val == "n613\n" or checkconfig_str_val == "n236\n") {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 25px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding:25px; Text-align: left");
    }
    else {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
    }

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
    connect(settingsWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
    connect(settingsWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToastNative()));
    settingsWindow->showFullScreen();
}

void settingsChooser::on_koboxSettingsBtn_clicked()
{
    koboxSettingsWindow = new koboxSettings();
    koboxSettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    koboxSettingsWindow->showFullScreen();
}

void settingsChooser::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

void settingsChooser::closeIndefiniteToastNative() {
    emit closeIndefiniteToast();
}
