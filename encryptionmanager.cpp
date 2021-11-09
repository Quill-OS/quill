#include "encryptionmanager.h"
#include "ui_encryptionmanager.h"

#include <QFile>
#include <QScreen>
#include <QDebug>
#include <QDir>
#include <QTimer>
#include <QMessageBox>
#include <QDateTime>
#include "usbms_splash.h"
#include "functions.h"

encryptionManager::encryptionManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::encryptionManager)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(":/resources/eink.qss");
    stylesheetFile.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheetFile.readAll());
    stylesheetFile.close();

    ui->encryptionSetupLabel->setStyleSheet("font-size: 15pt");
    ui->descriptionLabel->setStyleSheet("font-size: 9pt");
    ui->successLabel->setStyleSheet("font-size: 15pt");
    ui->successDescriptionLabel->setStyleSheet("font-size: 9pt");
    ui->failureLabel->setStyleSheet("font-size: 15pt");
    ui->failureDescriptionLabel->setStyleSheet("font-size: 9pt");
    ui->setupContinueBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->setupAbortBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->exitSuccessBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->failureContinueBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->acceptBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->usbmsBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->warningLabel->setStyleSheet("font-size: 15pt");
    ui->warningDescriptionLabel->setStyleSheet("font-size: 9pt");

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    float stdIconWidth;
    float stdIconHeight;
    {
        stdIconWidth = sW / 1.50;
        stdIconHeight = sH / 1.50;
        QPixmap pixmap(":/resources/encryption.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->encryptionImageLabel->setPixmap(scaledPixmap);
    }
    {
        stdIconWidth = sW / 1.65;
        stdIconHeight = sH / 1.65;
        QPixmap pixmap(":/resources/check-display.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->checkImageLabel->setPixmap(scaledPixmap);
    }
    {
        stdIconWidth = sW / 1.65;
        stdIconHeight = sH / 1.65;
        QPixmap pixmap(":/resources/error.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->failureImageLabel->setPixmap(scaledPixmap);
    }
    {
        stdIconWidth = sW / 1.50;
        stdIconHeight = sH / 1.50;
        QPixmap pixmap(":/resources/alert-triangle.png");
        QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
        ui->warningImageLabel->setPixmap(scaledPixmap);
    }

    hourglassAnimationWidgetWindow = new hourglassAnimationWidget();
    ui->hourglassWidget->insertWidget(0, hourglassAnimationWidgetWindow);
    ui->hourglassWidget->setCurrentIndex(0);

    setDefaultWorkDir();
    if(checkconfig(".config/18-encrypted_storage/initial_setup_done") == true) {
        ui->activityWidget->hide();
        setupPassphraseDialogMode = 1;
        QTimer::singleShot(500, this, SLOT(setupPassphraseDialog()));
    }
    else {
        QDir dir("/mnt/onboard/onboard/encfs-dropbox");
        if(dir.isEmpty()) {
            ui->activityWidget->setCurrentIndex(4);
            mkEncfsDirs();
        }
    }
}

encryptionManager::~encryptionManager()
{
    delete ui;
}

void encryptionManager::on_setupContinueBtn_clicked()
{
    setupPassphraseDialogMode = 0;
    setupPassphraseDialog();
}

void encryptionManager::setupPassphraseDialog() {
    /*
     * setupPassphraseDialogMode can be:
     * 0: Initial setup
     * 1: Normal behavior
    */
    ui->activityWidget->hide();
    this->setStyleSheet("background-color: black");
    global::keyboard::keyboardDialog = true;
    global::keyboard::encfsDialog = true;
    global::keyboard::keyboardText = "";
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    if(setupPassphraseDialogMode == 0) {
        connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupEncryptedStorage()));
    }
    else {
        connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(unlockEncryptedStorage()));
    }
    connect(generalDialogWindow, SIGNAL(showToast(QString)), SLOT(showToast(QString)));
    generalDialogWindow->show();
}


void encryptionManager::on_setupAbortBtn_clicked()
{
    setDefaultWorkDir();
    string_writeconfig(".config/18-encrypted_storage/status", "false");
    quit_restart();
}

void encryptionManager::quit_restart() {
    QProcess process;
    process.startDetached("inkbox", QStringList());
    qApp->quit();
}

void encryptionManager::refreshScreen() {
    this->repaint();
}

void encryptionManager::showToast(QString messageToDisplay) {
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(toastWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    toastWindow->show();
}

void encryptionManager::setupEncryptedStorage() {
    this->setStyleSheet("background-color: white");
    ui->activityWidget->show();
    if(global::encfs::cancelSetup == true) {
        global::encfs::cancelSetup = false;
        ui->activityWidget->setCurrentIndex(0);
    }
    else {
        mkEncfsDirs();
        std::string bootstrapPassphrase = global::encfs::passphrase.toStdString();
        global::encfs::passphrase = "";
        string_writeconfig("/external_root/run/encfs/encrypted_storage_create", "true");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_files_location", "/data/onboard/encfs-dropbox");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_archive_location", "/data/onboard/data.encfs");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_passphrase", bootstrapPassphrase);
        setDefaultWorkDir();
        string_writeconfig(".config/18-encrypted_storage/storage_list", "/data/onboard/encfs-decrypted");
        string_writeconfig("/opt/ibxd", "encfs_restart\n");
        bool exitStatus;
        ui->activityWidget->setCurrentIndex(3);
        QTimer * t = new QTimer(this);
        t->setInterval(1000);
        connect(t, &QTimer::timeout, [&]() {
            if(QFile::exists("/external_root/run/encrypted_storage_bootstrap_setup")) {
                exitStatus = checkconfig("/external_root/run/encrypted_storage_bootstrap_setup");
                QFile::remove("/external_root/run/encrypted_storage_bootstrap_setup");
                setupExitWidget(exitStatus);
            }
        } );
        t->start();
    }
}

void encryptionManager::unlockEncryptedStorage() {
    if(global::encfs::cancelSetup == true) {
        global::encfs::cancelSetup = false;
        poweroff(true);
    }
    else {
        this->setStyleSheet("background-color: white");
        ui->activityWidget->show();
        mkEncfsDirs();
        std::string passphrase = global::encfs::passphrase.toStdString();
        global::encfs::passphrase = "";
        string_writeconfig("/external_root/run/encfs/encrypted_storage_archive", "/data/onboard/data.encfs");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_mountpoint", "/data/onboard/encfs-decrypted");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_bindmount", "/kobo/mnt/onboard/onboard/encfs-decrypted");
        string_writeconfig("/external_root/run/encfs/encrypted_storage_passphrase", passphrase);
        string_writeconfig("/opt/ibxd", "encfs_restart\n");
        bool exitStatus;

        string_checkconfig_ro("/inkbox/encryptedStoragePassphraseTries");
        if(checkconfig_str_val.isEmpty()) {
            passphraseTries = 0;
        }
        else {
            passphraseTries = checkconfig_str_val.toInt();
            passphraseTries++;
        }

        ui->activityWidget->setCurrentIndex(3);
        QTimer * t = new QTimer(this);
        t->setInterval(1000);
        connect(t, &QTimer::timeout, [&]() {
            if(QFile::exists("/external_root/run/encfs_mounted")) {
                exitStatus = checkconfig("/external_root/run/encfs_mounted");
                if(exitStatus == false) {
                    if(setupMessageBoxRan == false) {
                        int delay = 0;
                        if(passphraseTries <= 3) {
                            if(passphraseTries == 0) {
                                string_writeconfig("/inkbox/encryptedStoragePassphraseTries", "0");
                                delay = 5000;
                            }
                            else if(passphraseTries == 1) {
                                string_writeconfig("/inkbox/encryptedStoragePassphraseTries", "1");
                                delay = 10000;
                            }
                            else if(passphraseTries == 2) {
                                string_writeconfig("/inkbox/encryptedStoragePassphraseTries", "2");
                                delay = 20000;
                            }
                            else if(passphraseTries >= 3) {
                                string_writeconfig("/inkbox/encryptedStoragePassphraseTries", "3");
                                unsigned long currentEpoch = QDateTime::currentSecsSinceEpoch();
                                currentEpoch += 86400;
                                std::string unlockTime_str = to_string(currentEpoch);
                                global::encfs::unlockTime = QDateTime::fromTime_t(currentEpoch).toString();
                                QString message = "FATAL: 4 invalid passphrase tries, locking down device until " + global::encfs::unlockTime;
                                qDebug() << message;
                                string_writeconfig("/external_root/boot/flags/ENCRYPT_LOCK", unlockTime_str);
                                global::encfs::lockdown = true;
                                setupMessageBoxRan = true;

                                alertWindow = new alert();
                                alertWindow->setAttribute(Qt::WA_DeleteOnClose);
                                alertWindow->showFullScreen();
                                poweroff(false);
                            }

                            if(passphraseTries <= 2) {
                                QTimer::singleShot(delay, this, SLOT(setupFailedAuthenticationMessageBox()));
                                setupMessageBoxRan = true;
                            }
                        }
                    }
                }
                else {
                    quit_restart();
                }
            }
        } );
        t->start();
    }
}

void encryptionManager::mkEncfsDirs() {
    QDir encfsDir;
    QString encfsPath("/external_root/run/encfs");
    encfsDir.mkpath(encfsPath);
    QDir dumpDir;
    QString dumpPath("/mnt/onboard/onboard/encfs-dropbox");
    dumpDir.mkpath(dumpPath);
    QDir decDir;
    QString decPath("/mnt/onboard/onboard/encfs-decrypted");
    decDir.mkpath(decPath);
}

void encryptionManager::on_exitSuccessBtn_clicked()
{
    setDefaultWorkDir();
    string_writeconfig(".config/18-encrypted_storage/initial_setup_done", "true");
    quit_restart();
}

void encryptionManager::setupExitWidget(bool exitStatus) {
    if(setupExitWidgetRan == false) {
        if(exitStatus == true) {
            ui->activityWidget->setCurrentIndex(1);
        }
        else {
            string_writeconfig(".config/18-encrypted_storage/status", "false");
            ui->activityWidget->setCurrentIndex(2);
        }
        setupExitWidgetRan = true;
    }
}

void encryptionManager::on_failureContinueBtn_clicked()
{
    setDefaultWorkDir();
    string_writeconfig(".config/18-encrypted_storage/initial_setup_done", "true");
    quit_restart();
}

void encryptionManager::setupFailedAuthenticationMessageBox() {
    ui->activityWidget->hide();
    QMessageBox::critical(this, tr("Invalid argument"), tr("Invalid passphrase. Please try again."));
    QFile::remove("/external_root/run/encfs_mounted");
    quit_restart();
}

void encryptionManager::on_acceptBtn_clicked()
{
    string_writeconfig(".config/18-encrypted_storage/status", "false");
    quit_restart();
}


void encryptionManager::on_usbmsBtn_clicked()
{
    global::usbms::launchUsbms = true;
    usbmsWindow = new usbms_splash();
    usbmsWindow->setAttribute(Qt::WA_DeleteOnClose);
    usbmsWindow->setGeometry(QRect(QPoint(0,0), screen()->geometry ().size()));
    usbmsWindow->show();
}

