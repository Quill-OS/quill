#ifndef USERAPP_H
#define USERAPP_H

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
    QString jsonFilePath;

public slots:
    void provideInfo(QJsonObject jsonObject);
    void changePageEnabling(bool goThere);

signals:
    void remakeApps();

private slots:
    void on_pushButtonAppInfo_clicked();

    void on_pushButton_2EnablingButton_clicked();

private:
    Ui::userapp *ui;
    QDir appDir;
    QFile execPath;

};

#endif // USERAPP_H
