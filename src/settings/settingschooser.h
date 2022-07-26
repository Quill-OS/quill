#ifndef SETTINGSCHOOSER_H
#define SETTINGSCHOOSER_H

#include <QWidget>

#include "settings.h"
#include "koboxsettings.h"

namespace Ui {
class settingsChooser;
}

class settingsChooser : public QWidget
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    explicit settingsChooser(QWidget *parent = nullptr);
    ~settingsChooser();

    float sW;
    float sH;
    float homeIconWidth;
    float homeIconHeight;

private slots:
    void on_inkboxSettingsBtn_clicked();
    void on_koboxSettingsBtn_clicked();
    void on_powerSettingsBtn_clicked();
    void showToastNative(QString messageToDisplay);
    void closeIndefiniteToastNative();

signals:
    void showToast(QString messageToDisplay);
    void closeIndefiniteToast();

private:
    Ui::settingsChooser *ui;
    settings *settingsWindow;
    koboxSettings *koboxSettingsWindow;
};

#endif // SETTINGSCHOOSER_H
