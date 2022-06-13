#ifndef USERAPP_H
#define USERAPP_H

#include "generaldialog.h"

#include <QWidget>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>
#include <QFile>

namespace Ui {
class userapp;
}

class userapp : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit userapp(QWidget *parent = nullptr);
    ~userapp();
    QJsonDocument jsonDocument; // I need this for changing enabled / disabled bool in json. this is propably not good for ram. maybe there is a better way in the future
    QJsonObject jsonObject; // Needed for info button
    QString jsonFilePath;

signals:
    // This is needed - because userapps have their own json. if the user will change it fast they won't read it and overwrite previous changes
    void updateJsonFileSignalUA(QJsonDocument jsonDocument);


public slots:
    void provideInfo(QJsonObject jsonObject);
    void changePageEnabling(bool goThere);
    void updateJsonFileSlotUA(QJsonDocument jsonDocument);
    bool manageRequiredFeatures();

private slots:
    void on_pushButtonAppInfo_clicked();
    void on_pushButton_2EnablingButton_clicked();

    void on_pushButtonLaunch_clicked();

private:
    Ui::userapp *ui;
    generalDialog *generalDialogWindow;

    QDir appDir;
    QFile execPath;
    bool userAppEnabled;
    QString appName;


};

#endif // USERAPP_H
