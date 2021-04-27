#ifndef KOBOXSETTINGS_H
#define KOBOXSETTINGS_H

#include <QWidget>

namespace Ui {
class koboxSettings;
}

class koboxSettings : public QWidget
{
    Q_OBJECT

public:
    explicit koboxSettings(QWidget *parent = nullptr);
    ~koboxSettings();

private slots:
    void on_okBtn_clicked();

private:
    Ui::koboxSettings *ui;
};

#endif // KOBOXSETTINGS_H
