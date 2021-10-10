#include "encryptionmanager.h"
#include "ui_encryptionmanager.h"

#include <QFile>
#include <QScreen>
#include <QDebug>
#include <QDir>
#include <QTimer>
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
    ui->setupContinueBtn->setStyleSheet("font-size: 10pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->setupAbortBtn->setStyleSheet("font-size: 10pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->exitSuccessBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");
    ui->failureContinueBtn->setStyleSheet("background: lightGrey; border: 3px solid black; color: black; padding: 10px; outline: none; font-size: 10pt; font-weight: bold");

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

    hourglassAnimationWidgetWindow = new hourglassAnimationWidget();
    ui->hourglassWidget->insertWidget(0, hourglassAnimationWidgetWindow);
    ui->hourglassWidget->setCurrentIndex(0);

    setDefaultWorkDir();
    if(checkconfig(".config/18-encrypted_storage/initial_setup_done") == true) {
        setupPassphraseDialog(1);
    }
}

encryptionManager::~encryptionManager()
{
    delete ui;
}

void encryptionManager::on_setupContinueBtn_clicked()
{
    setupPassphraseDialog(0);
}

void encryptionManager::setupPassphraseDialog(int mode) {
    /*
     * Mode can be:
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
    if(mode == 0) {
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
    qDebug() << "showToast";
    global::toast::message = messageToDisplay;
    toastWindow = new toast(this);
    toastWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(toastWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    toastWindow->show();
}

void encryptionManager::setupEncryptedStorage() {
    this->setStyleSheet("background-color: white");
    ui->activityWidget->show();
    mkEncfsDirs();
    std::string bootstrapPassphrase = global::encfs::passphrase.toStdString();
    global::encfs::passphrase = "";
    string_writeconfig("/external_root/run/encfs/encrypted_storage_create", "true");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_files_location", "/data/onboard/encfs-dropbox");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_archive_location", "/data/onboard/data.encfs");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_passphrase", bootstrapPassphrase);
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

void encryptionManager::unlockEncryptedStorage() {
    this->setStyleSheet("background-color: white");
    ui->activityWidget->show();
    mkEncfsDirs();
    std::string passphrase = global::encfs::passphrase.toStdString();
    global::encfs::passphrase = "";
    string_writeconfig("/external_root/run/encfs/encrypted_storage_archive", "/data/onboard/data.encfs");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_mountpoint", "/data/onboard/books");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_passphrase", passphrase);
    bool exitStatus;
    ui->activityWidget->setCurrentIndex(3);
    QTimer * t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() {
        if(QFile::exists("/external_root/run/encfs_mounted")) {
            exitStatus = checkconfig("/external_root/run/encfs_mounted");
            quit_restart();
        }
    } );
    t->start();
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

