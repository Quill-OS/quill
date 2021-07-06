#include "generaldialog.h"
#include "ui_generaldialog.h"
#include "functions.h"
#include "reader.h"
#include "mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QProcess>
#include <QScreen>
#include <QTimer>
#include <QMessageBox>

generalDialog::generalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::generalDialog)
{
    ui->setupUi(this);

    // Preventing outside interaction
    this->setModal(true);

    // Stylesheet, style & misc.
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->okBtn->setProperty("type", "borderless");
    ui->cancelBtn->setProperty("type", "borderless");
    ui->acceptBtn->setProperty("type", "borderless");

    ui->okBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->cancelBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->acceptBtn->setStyleSheet("font-size: 9pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->bodyLabel->setStyleSheet("font-size: 9pt");
    ui->searchComboBox->setStyleSheet("font-size: 9pt");

    if(checkconfig("/inkbox/resetDialog") == true) {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            resetDialog = true;
            ui->okBtn->setText("Proceed");
            ui->cancelBtn->setText("Go back");
            ui->bodyLabel->setText("This will erase any books you have on the device. Settings will be reset.");
            ui->headerLabel->setText("Warning");
            this->adjustSize();
            string_writeconfig("/inkbox/resetDialog", "false");
        }
        else {
            resetDialog = true;
            ui->okBtn->setText("Proceed");
            ui->cancelBtn->setText("Go back");
            ui->bodyLabel->setText("Settings will be reset.");
            ui->headerLabel->setText("Warning");
            this->adjustSize();
            string_writeconfig("/inkbox/resetDialog", "false");
        }
    }
    else if(checkconfig("/inkbox/updateDialog") == true) {
        updateDialog = true;
        ui->okBtn->setText("Update");
        ui->cancelBtn->setText("Not now");
        ui->bodyLabel->setText("Do you want to update InkBox now?");
        ui->headerLabel->setText("Update available");
        this->adjustSize();
        string_writeconfig("/inkbox/updateDialog", "false");
    }
    else if(global::settings::settingsRebootDialog == true) {
        settingsRebootDialog = true;
        ui->stackedWidget->setCurrentIndex(1);
        if(global::kobox::koboxSettingsRebootDialog == true) {
            koboxSettingsRebootDialog = true;
            ui->bodyLabel->setText("The device will reboot now, since the settings you chose require it to work properly.");
        }
        else {
            ui->bodyLabel->setText("The settings you chose might require a complete reboot of the device for them to work properly.");
        }
        ui->headerLabel->setText("Information");
        this->adjustSize();
    }
    else if(global::mainwindow::lowBatteryDialog == true) {
        lowBatteryDialog = true;
        ui->stackedWidget->setCurrentIndex(1);
        get_battery_level();
        QString message = "The battery's level is low. Please charge your eReader.\nCurrent level: ";
        message.append(batt_level);
        ui->bodyLabel->setText(message);
        ui->headerLabel->setText("Low battery");
        this->adjustSize();
        string_writeconfig("/inkbox/lowBatteryDialog", "false");
    }
    else if(global::usbms::usbmsDialog == true) {
        usbmsDialog = true;
        ui->okBtn->setText("Connect");
        ui->cancelBtn->setText("Cancel");
        ui->bodyLabel->setText("Do you want to connect your device to a computer to manage books?");
        ui->headerLabel->setText("USB cable connected");
        this->adjustSize();
    }
    else if(global::text::textBrowserDialog == true) {
        textBrowserDialog = true;
        textwidgetWindow = new textwidget();
        ui->headerLabel->setText("Information");
        ui->stackedWidget->setCurrentIndex(1);
        ui->mainStackedWidget->insertWidget(1, textwidgetWindow);
        ui->mainStackedWidget->setCurrentIndex(1);
        this->adjustSize();
    }
    else if(global::kobox::resetKoboxDialog == true) {
        resetKoboxDialog = true;
        ui->headerLabel->setText("Warning");
        ui->okBtn->setText("Proceed");
        ui->cancelBtn->setText("Go back");
        ui->bodyLabel->setText("This will erase all KoBox user data. KoBox settings will be reset.");
        this->adjustSize();
    }
    else if(global::keyboard::keyboardDialog == true) {
        setupKeyboardDialog();
    }
    else if(global::keyboard::keypadDialog == true) {
        keypadDialog = true;
        keypadWidget = new virtualkeypad();
        ui->headerLabel->setText("Enter a number");
        ui->okBtn->setText("OK");
        ui->cancelBtn->setText("Cancel");
        ui->mainStackedWidget->insertWidget(1, keypadWidget);
        ui->mainStackedWidget->setCurrentIndex(1);
        this->adjustSize();
    }
    else {
        // We shouldn't be there ;)
        ;
    }

    // Centering dialog
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
}

generalDialog::~generalDialog()
{
    delete ui;
}

void generalDialog::on_cancelBtn_clicked()
{
    if(updateDialog == true) {
        string_writeconfig("/tmp/cancelUpdateDialog", "true");
        generalDialog::close();
    }
    else if(usbmsDialog == true) {
        global::usbms::usbmsDialog = false;
        generalDialog::close();
    }
    else {
        if(global::keyboard::searchDialog == true) {
            global::keyboard::searchDialog = false;
            global::forbidOpenSearchDialog = true;
            global::keyboard::keyboardDialog = false;
            global::keyboard::keyboardText = "";
        }
        generalDialog::close();
    }
}

void generalDialog::on_okBtn_clicked()
{
    if(resetDialog == true) {
        if(checkconfig("/opt/inkbox_genuine") == true) {
            // Soft-reset the device
            // We set a custom boot flag and reboot silently in Diagnostics
            string_writeconfig("/external_root/boot/flags/DIAGS_BOOT", "true");
            string_writeconfig("/external_root/boot/flags/DO_SOFT_RESET", "true");
            reboot(false);
        }
        else {
            // Restore default settings, we're not on InkBox OS
            QString prog ("sh");
            QStringList args;
            args << "reset-config.sh";
            QProcess *proc = new QProcess();
            proc->start(prog, args);
            proc->waitForFinished();

            // Relaunching InkBox
            QProcess process;
            process.startDetached("inkbox.sh", QStringList());
            qApp->quit();
        }
    }
    if(updateDialog == true) {
        string_writeconfig("/mnt/onboard/onboard/.inkbox/can_really_update", "true");
        string_writeconfig("/external_root/opt/update/will_update", "true");
        string_writeconfig("/external_root/boot/flags/WILL_UPDATE", "true");
        reboot(true);
    }
    if(usbmsDialog == true) {
        global::usbms::usbmsDialog = false;
        global::usbms::launchUsbms = true;

        usbmsWindow = new usbms_splash();
        usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
        usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
        usbmsWindow->show();
    }
    if(resetKoboxDialog == true) {
        resetKoboxUserData();
    }
    if(keypadDialog == true) {
        int number = global::keyboard::keypadText.toInt();
        emit gotoPageSelected(number);
        global::keyboard::keypadDialog = false;
        generalDialog::close();
    }
    if(keyboardDialog == true) {
        if(global::keyboard::searchDialog == true) {
            if(global::keyboard::keyboardText != "") {
                if(ui->searchComboBox->currentText() == "Dictionary") {
                    for(int i = ui->mainStackedWidget->count(); i >= 0; i--) {
                        QWidget * widget = ui->mainStackedWidget->widget(i);
                        ui->mainStackedWidget->removeWidget(widget);
                        widget->deleteLater();
                    }
                    ui->topStackedWidget->setVisible(false);
                    ui->stackedWidget->setVisible(false);
                    dictionaryWidgetWindow = new dictionaryWidget();
                    dictionaryWidgetWindow->setAttribute(Qt::WA_DeleteOnClose);
                    connect(dictionaryWidgetWindow, SIGNAL(refreshScreen()), SLOT(refreshScreenNative()));
                    connect(dictionaryWidgetWindow, SIGNAL(destroyed(QObject*)), SLOT(restartSearchDialog()));
                    ui->mainStackedWidget->insertWidget(1, dictionaryWidgetWindow);
                }
                else {
                    ;
                }
            }
            else {
                QMessageBox::critical(this, tr("Invalid argument"), tr("Please type in a search term."));
            }
        }
        else if(global::keyboard::vncDialog == true) {
            if(global::keyboard::keyboardText != "") {
                if(vncServerSet != true) {
                    vncServerAddress = global::keyboard::keyboardText;
                    vncServerSet = true;
                    keyboardWidget->clearLineEdit();
                    ui->headerLabel->setText("Enter the server's password");
                }
                else if(vncPasswordSet != true) {
                    vncServerPassword = global::keyboard::keyboardText;
                    vncPasswordSet = true;
                    keyboardWidget->clearLineEdit();
                    ui->headerLabel->setText("Enter the server's port");
                }
                else {
                    vncServerPort = global::keyboard::keyboardText;
                    global::keyboard::vncDialog = false;
                    global::keyboard::keyboardDialog = false;
                    startVNC(vncServerAddress, vncServerPassword, vncServerPort);
                    generalDialog::close();
                }
            }
            else {
                QMessageBox::critical(this, tr("Invalid argument"), tr("Please type in the required argument."));
            }
        }
        else {
            global::keyboard::keyboardDialog = false;
            generalDialog::close();
        }
    }
}
void generalDialog::on_acceptBtn_clicked()
{
    if(lowBatteryDialog == true) {
        global::mainwindow::lowBatteryDialog = false;
        global::battery::batteryAlertLock = false;
        global::battery::showLowBatteryDialog = false;
    }

    if(settingsRebootDialog == true) {
        if(koboxSettingsRebootDialog == true) {
            reboot(true);
        }
        else {
            QProcess process;
            process.startDetached("inkbox.sh", QStringList());
            qApp->quit();
        }
    }
    if(textBrowserDialog == true) {
        global::text::textBrowserContents = "";
        global::text::textBrowserDialog = false;
    }

    // We don't have any other option ;p
    generalDialog::close();
}

void generalDialog::adjust_size() {
    this->adjustSize();
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    emit refreshScreen();
}

void generalDialog::restartSearchDialog() {
    generalDialog::close();
}

void generalDialog::setupKeyboardDialog() {
    keyboardDialog = true;
    ui->stackedWidget->setVisible(true);
    if(global::keyboard::searchDialog == true) {
        ui->topStackedWidget->setCurrentIndex(1);
        ui->searchHeaderLabel->setText("Search");
        ui->okBtn->setText("Search");
        ui->cancelBtn->setText("Close");
    }
    else if(global::keyboard::vncDialog == true) {
        ui->headerLabel->setText("Enter the server's IP address");
        ui->okBtn->setText("OK");
        ui->cancelBtn->setText("Cancel");
    }
    else {
        ui->headerLabel->setText("Enter a string");
        ui->okBtn->setText("OK");
        ui->cancelBtn->setText("Cancel");
    }
    keyboardWidget = new virtualkeyboard();
    connect(keyboardWidget, SIGNAL(adjust_size()), SLOT(adjust_size()));
    ui->mainStackedWidget->insertWidget(1, keyboardWidget);
    ui->mainStackedWidget->setCurrentIndex(1);
    QTimer::singleShot(1000, this, SLOT(adjust_size()));
}

void generalDialog::refreshScreenNative() {
    emit refreshScreen();
}

void generalDialog::startVNC(QString server, QString password, QString port) {
    std::string server_str = server.toStdString();
    std::string password_str = password.toStdString();
    std::string port_str = port.toStdString();
    string_writeconfig("/external_root/tmp/app_vnc_server", server_str);
    string_writeconfig("/external_root/tmp/app_vnc_password", password_str);
    string_writeconfig("/external_root/tmp/app_vnc_port", port_str);
    string_writeconfig("/opt/ibxd", "app_start_vnc");
    qApp->quit();
}
