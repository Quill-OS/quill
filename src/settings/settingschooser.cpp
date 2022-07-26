#include "settingschooser.h"
#include "ui_settingschooser.h"
#include "functions.h"
#include "powerdaemonsettings.h"

#include <QFile>
#include <QScreen>

settingsChooser::settingsChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsChooser)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile("/mnt/onboard/.adds/inkbox/eink.qss");
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
    if(global::deviceID == "n705\n") {
        homeIconWidth = sW / 18;
        homeIconHeight = sW / 18;
    }
    else {
        homeIconWidth = sW / 20;
        homeIconHeight = sW / 20;
    }

    ui->inkboxSettingsBtn->setProperty("type", "borderless");
    ui->koboxSettingsBtn->setProperty("type", "borderless");
    ui->powerSettingsBtn->setProperty("type", "borderless");

    if(global::deviceID == "n705\n" or global::deviceID == "n905\n" or global::deviceID == "kt\n") {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 20px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding: 20px; Text-align: left");
        ui->powerSettingsBtn->setStyleSheet("padding: 20px; Text-align: left");
    }
    else if(global::deviceID == "n613\n" or global::deviceID == "n236\n" or global::deviceID == "n306\n") {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 25px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding:25px; Text-align: left");
        ui->powerSettingsBtn->setStyleSheet("padding:25px; Text-align: left");
    }
    else {
        ui->inkboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
        ui->koboxSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
        ui->powerSettingsBtn->setStyleSheet("padding: 40px; Text-align: left");
    }

    ui->inkboxSettingsBtn->setText("\t\t\tInkBox settings");
    ui->koboxSettingsBtn->setText("\t\t\tKoBox settings");
    ui->powerSettingsBtn->setText("\t\t\tPower settings");

    ui->inkboxSettingsBtn->setIcon(QIcon(":/resources/settings.png"));
    ui->inkboxSettingsBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));
    ui->koboxSettingsBtn->setIcon(QIcon(":/resources/X11.png"));
    ui->koboxSettingsBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));
    ui->powerSettingsBtn->setIcon(QIcon(":/resources/power.png"));
    ui->powerSettingsBtn->setIconSize(QSize(homeIconWidth, homeIconHeight));
}

settingsChooser::~settingsChooser()
{
    delete ui;
}

void settingsChooser::on_inkboxSettingsBtn_clicked()
{
    log("Launching InkBox Settings", className);
    settingsWindow = new settings();
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(settingsWindow, SIGNAL(showToast(QString)), SLOT(showToastNative(QString)));
    connect(settingsWindow, SIGNAL(closeIndefiniteToast()), SLOT(closeIndefiniteToastNative()));
    settingsWindow->showFullScreen();
}

void settingsChooser::on_koboxSettingsBtn_clicked()
{
    log("Launching KoBox Settings", className);
    koboxSettingsWindow = new koboxSettings();
    koboxSettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    koboxSettingsWindow->showFullScreen();
}

void settingsChooser::on_powerSettingsBtn_clicked()
{
    log("Launching Power Daemon Settings", className);
    powerDaemonSettings * powerSettingsWindow = new powerDaemonSettings();
    powerSettingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    powerSettingsWindow->showFullScreen();
}

void settingsChooser::showToastNative(QString messageToDisplay) {
    emit showToast(messageToDisplay);
}

void settingsChooser::closeIndefiniteToastNative() {
    emit closeIndefiniteToast();
}
