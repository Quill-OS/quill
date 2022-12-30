#ifndef AUDIODIALOG_H
#define AUDIODIALOG_H

#include <QDialog>

namespace Ui {
class audioDialog;
}

class audioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit audioDialog(QWidget *parent = nullptr);
    ~audioDialog();
    QString className = this->metaObject()->className();
    bool currentMenu = true; // true means Library is clicked so its grey, false means Queue
    void changeMenu();
    void refreshFileList();
    void refreshAudioFileWidgets();

private slots:
    void on_libraryBtn_clicked();

    void on_queueBtn_clicked();

    void on_refreshBtn_clicked();

signals:
    void deleteItself();

private:
    Ui::audioDialog *ui;
};

#endif // AUDIODIALOG_H
