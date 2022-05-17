#ifndef USERAPP_H
#define USERAPP_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>

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

public slots:
    void provideInfo(QJsonObject jsonObject);

private:
    Ui::userapp *ui;
    QDir appDir;

};

#endif // USERAPP_H
