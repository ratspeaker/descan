#ifndef DIALOGMAIL_H
#define DIALOGMAIL_H

#include <QDialog>
#include <curl/curl.h>
#include <QString>
#include <QMessageBox>

namespace Ui {
class DialogMail;
}

class DialogMail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMail(QWidget *parent = nullptr);
    ~DialogMail();
    QString fileName;

private slots:
    void on_pbExit_clicked();
    void on_pbBrowse_clicked();
    void on_pbSend_clicked();

//private:
public:
    Ui::DialogMail *ui;
};

#endif // DIALOGMAIL_H
