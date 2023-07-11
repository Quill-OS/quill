#ifndef BRIGHTNESSDIALOG_H
#define BRIGHTNESSDIALOG_H

#include <QDialog>

using namespace std;

namespace Ui {
class brightnessDialog;
}

class brightnessDialog : public QDialog
{
    Q_OBJECT

public:
    QString className = this->metaObject()->className();
    int oldValue;
    int oldWarmthValue;
    void preSetBrightness(int brightnessValue);
    explicit brightnessDialog(QWidget *parent = nullptr);
    ~brightnessDialog();

private slots:
    void on_quitBtn_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_incBtn_clicked();
    void on_decBtn_clicked();
    void on_okBtn_clicked();
    void on_warmthSlider_valueChanged(int value);
    void on_warmthDecBtn_clicked();
    void on_warmthIncBtn_clicked();

private:
    Ui::brightnessDialog *ui;
};

#endif // BRIGHTNESSDIALOG_H
