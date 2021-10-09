#include "encryptionmanager.h"
#include "ui_encryptionmanager.h"

#include <QFile>
#include <QScreen>
#include <QDebug>
#include <QDir>
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
    ui->setupContinueBtn->setStyleSheet("font-size: 10pt; padding: 10px; font-weight: bold; background: lightGrey");
    ui->setupAbortBtn->setStyleSheet("font-size: 10pt; padding: 10px; font-weight: bold; background: lightGrey");

    // Getting the screen's size
    float sW = QGuiApplication::screens()[0]->size().width();
    float sH = QGuiApplication::screens()[0]->size().height();
    float stdIconWidth = sW / 1.50;
    float stdIconHeight = sH / 1.50;

    QPixmap pixmap(":/resources/encryption.png");
    QPixmap scaledPixmap = pixmap.scaled(stdIconWidth, stdIconHeight, Qt::KeepAspectRatio);
    ui->encryptionImageLabel->setPixmap(scaledPixmap);
}

encryptionManager::~encryptionManager()
{
    delete ui;
}

void encryptionManager::on_setupContinueBtn_clicked()
{
    ui->activityWidget->hide();
    this->setStyleSheet("background-color: black");
    global::keyboard::keyboardDialog = true;
    global::keyboard::encfsDialog = true;
    global::keyboard::keyboardText = "";
    generalDialogWindow = new generalDialog();
    generalDialogWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(generalDialogWindow, SIGNAL(refreshScreen()), SLOT(refreshScreen()));
    connect(generalDialogWindow, SIGNAL(destroyed(QObject*)), SLOT(setupEncryptedStorage()));
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
    mkEncfsDirs();
    std::string bootstrapPassphrase = global::encfs::passphrase.toStdString();
    global::encfs::passphrase = "";
    string_writeconfig("/external_root/run/encfs/encrypted_storage_create", "true");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_files_location", "/data/onboard/encfs-dropbox");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_archive_location", "/data/onboard/data.encfs");
    string_writeconfig("/external_root/run/encfs/encrypted_storage_bootstrap_passphrase", bootstrapPassphrase);
    string_writeconfig("/opt/ibxd", "encfs_restart\n");
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
