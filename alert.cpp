#include "alert.h"
#include "ui_alert.h"

#include <QPixmap>
#include <QScreen>
#include <QFile>
#include <QProcess>

alert::alert(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::alert)
{
    ui->setupUi(this);
    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    // Defining what the "Alert" icon size will be
    float stdIconWidth = sW / 1.7;
    float stdIconHeight = sH / 1.7;

    QPixmap pixmap(":/resources/alert.png");
    QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
    ui->alertIconLabel->setPixmap(scaledPixmap);

    // General stylesheet
    QFile stylesheetFile(":/resources/eink_dark.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    // Checking if the update signature was bad
    if(checkconfig("/external_root/boot/flags/ALERT_SIGN") == true) {
        ui->securityLabel->setText("Failed to install update.");
        ui->messageLabel->setText("The update's digital signature is untrusted. For security reasons, it is not possible to install it.");
        ui->stackedWidget->setCurrentIndex(1);
    }

    ui->warningLabel->setStyleSheet("QLabel { background-color : black; color : white; font-size: 16pt}");
    ui->messageLabel->setStyleSheet("QLabel { background-color : black; color : white; font-size: 9pt}");
    ui->securityLabel->setStyleSheet("QLabel { background-color : black; color : white; font-size: 11pt}");
    ui->continueBtn->setProperty("type", "borderless");
    ui->resetBtn->setProperty("type", "borderless");
    ui->continueBtn->setStyleSheet("padding: 20px");
    ui->resetBtn->setStyleSheet("padding: 20px");
    ui->continue2Btn->setProperty("type", "borderless");
    ui->continue2Btn->setStyleSheet("padding: 20px");
}

alert::~alert()
{
    delete ui;
}

void alert::on_continueBtn_clicked()
{
    // We continue anyway and re-set the ALERT flag
    string_writeconfig("/external_root/boot/flags/ALERT", "false");
    string_writeconfig("/external_root/boot/flags/ALERT_SIGN", "false");
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void alert::on_resetBtn_clicked()
{
    // We set the DO_FACTORY_RESET flag and we restart the Kobo
    string_writeconfig("/external_root/boot/flags/DO_FACTORY_RESET", "true");
    string_writeconfig("/external_root/boot/flags/DIAGS_BOOT", "true");
    QString reboot_prog ("busybox");
    QStringList reboot_args;
    reboot_args << "reboot";
    QProcess *reboot_proc = new QProcess();
    reboot_proc->start(reboot_prog, reboot_args);
    reboot_proc->waitForFinished();
}

void alert::on_continue2Btn_clicked()
{
    // We continue anyway and re-set the ALERT flag
    string_writeconfig("/external_root/boot/flags/ALERT", "false");
    string_writeconfig("/external_root/boot/flags/ALERT_SIGN", "false");
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}
