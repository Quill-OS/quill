#include "audiodialog.h"
#include "ui_audiodialog.h"
#include "functions.h"
#include "audiofile.h"

#include <sndfile.h>
#include <unistd.h>
#include <math.h>

#include <QScreen>

audioDialog::audioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::audioDialog)
{
    ui->setupUi(this);

    // Size
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    this->setFixedWidth(screenGeometry.width() / 1.1);

    int halfOfHalfHeight = ((screenGeometry.height() / 2) / 2) / 2;
    int finalHeight = screenGeometry.height() - halfOfHalfHeight;

    this->setFixedHeight(finalHeight);

    // Centering dialog
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);

    ui->minusBtn->setProperty("type", "borderless");
    ui->plusBtn->setProperty("type", "borderless");
    ui->exitBtn->setProperty("type", "borderless");
    ui->nextBtn->setProperty("type", "borderless");
    ui->previousBtn->setProperty("type", "borderless");
    ui->playBtn->setProperty("type", "borderless");
    ui->libraryBtn->setProperty("type", "borderless");
    ui->queueBtn->setProperty("type", "borderless");
    ui->refreshBtn->setProperty("type", "borderless");


    int topButtonsSize = 55;
    int menuButtonsSize = 80;
    int playButtonsSize = 50;

    ui->minusBtn->setIconSize(QSize{topButtonsSize,topButtonsSize});
    ui->plusBtn->setIconSize(QSize{topButtonsSize,topButtonsSize});
    ui->exitBtn->setIconSize(QSize{topButtonsSize,topButtonsSize});
    ui->refreshBtn->setIconSize(QSize{topButtonsSize,topButtonsSize});

    ui->nextBtn->setIconSize(QSize{playButtonsSize,playButtonsSize});
    ui->previousBtn->setIconSize(QSize{playButtonsSize,playButtonsSize});
    ui->playBtn->setIconSize(QSize{playButtonsSize,playButtonsSize});

    ui->libraryBtn->setIconSize(QSize{menuButtonsSize,menuButtonsSize});
    ui->queueBtn->setIconSize(QSize{menuButtonsSize,menuButtonsSize});

    ui->libraryBtn->setStyleSheet("background: grey;");

    if(global::audio::firstScan == true) {
        global::audio::firstScan = false;
        refreshFileList();
    }

    refreshAudioFileWidgets();
}

audioDialog::~audioDialog()
{
    delete ui;
}

void audioDialog::changeMenu() {
    if(currentMenu == true) {
        currentMenu = false;
        ui->libraryBtn->setStyleSheet("background: white;");
        ui->queueBtn->setStyleSheet("background: grey;");
    }
    else {
        currentMenu = true;
        ui->libraryBtn->setStyleSheet("background: grey;");
        ui->queueBtn->setStyleSheet("background: white;");
    }
}

void audioDialog::on_libraryBtn_clicked()
{
    if(currentMenu == false) {
        changeMenu();
    }

}

void audioDialog::on_queueBtn_clicked()
{
    if(currentMenu == true) {
        changeMenu();
    }

}

void audioDialog::refreshFileList() {
    log("Refreshing file list", className);
    // For example in this path: /mnt/onboard/onboard/music/ ( with / at the end )
    QString path = readFile(".config/e-2-audio/path").replace("\n", "");
    log("Path for audio files: " + path, className);
    QDir dir{path};
    dir.setNameFilters(QStringList("*.wav"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    log("File count: " + QString::number(fileList.count()), className);
    for (int i=0; i<fileList.count(); i++)
    {
        log("Audio file: " + fileList[i], className);
        global::audio::musicFile newMusicFile;
        newMusicFile.path = path + fileList[i];
        QString tempName = fileList[i];
        tempName.chop(4);
        newMusicFile.name = tempName;
        log("File name length: " + QString::number(fileList[i].length()), className);
        log("File name: " + tempName, className);
        // Limit for chars, this one for Nia
        if(newMusicFile.name.length() > 30) {
            newMusicFile.name.chop(newMusicFile.name.length() - 30);
        }
        // http://libsndfile.github.io/libsndfile/api#open_fd
        /*
        sf_count_t  frames ;
        int         samplerate ;
        int         channels ;
        int         format ;
        int         sections ;
        int         seekable ;
        */
        SF_INFO sfinfo;
        SNDFILE *wavFile = ::sf_open(newMusicFile.path.toStdString().c_str(), SFM_READ, &sfinfo);
        // http://soundfile.sapp.org/doc/WaveFormat/
        // https://stackoverflow.com/questions/53338925/get-the-audio-duration-using-libsndfile
        int bareSeconds = static_cast<double>(sfinfo.frames) / (sfinfo.samplerate);
        sf_close(wavFile);
        newMusicFile.lengths = bareSeconds;
        int countMin = 0;
        while(bareSeconds >= 60) {
            countMin = countMin + 1;
            bareSeconds = bareSeconds - 60;
        }
        newMusicFile.length = QString::number(countMin) + ":" + QString::number(bareSeconds);
        // To avoid shifting the line
        while(newMusicFile.length.length() < 4) {
            newMusicFile.length = newMusicFile.length + " ";
        }
        log("File length: " + newMusicFile.length, className);
        global::audio::fileList.append(newMusicFile);
    }
}

void audioDialog::refreshAudioFileWidgets() {
    emit deleteItself();
    for(int i = 0; i < global::audio::fileList.size(); i++) {
        log("Adding new item: " + QString::number(i));
        audiofile* newAudioFile = new audiofile;
        newAudioFile->provideData(global::audio::fileList[i]);
        QObject::connect(this, &audioDialog::deleteItself, newAudioFile, &audiofile::die);
        ui->filesLayout->addWidget(newAudioFile, Qt::AlignTop);
    }
}

void audioDialog::on_refreshBtn_clicked()
{
    refreshFileList();
    refreshAudioFileWidgets();
}
