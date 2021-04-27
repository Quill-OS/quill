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
    explicit settingsChooser(QWidget *parent = nullptr);
    ~settingsChooser();

    float sW;
    float sH;
    float homeIconWidth;
    float homeIconHeight;

private slots:
    void on_inkboxSettingsBtn_clicked();

    void on_koboxSettingsBtn_clicked();

private:
    Ui::settingsChooser *ui;
    settings *settingsWindow;
    koboxSettings *koboxSettingsWindow;
};

#endif // SETTINGSCHOOSER_H
