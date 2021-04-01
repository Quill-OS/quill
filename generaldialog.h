#ifndef GENERALDIALOG_H
#define GENERALDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <fstream>

using namespace std;

namespace Ui {
class generalDialog;
}

class generalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit generalDialog(QWidget *parent = nullptr);
    ~generalDialog();
    bool resetDialog = false;
    bool updateDialog = false;
    bool checkconfig(QString file) {
        QFile config(file);
        config.open(QIODevice::ReadWrite);
        QTextStream in (&config);
        const QString content = in.readAll();
        string contentstr = content.toStdString();
        if(contentstr.find("true") != std::string::npos) {
            return true;
        }
        else {
            return false;
        }
        config.close();
    };
    void string_writeconfig(string file, string config_option) {
        ofstream fhandler;
        fhandler.open(file);
        fhandler << config_option;
        fhandler.close();
    }

private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();

private:
    Ui::generalDialog *ui;
};

#endif // GENERALDIALOG_H
