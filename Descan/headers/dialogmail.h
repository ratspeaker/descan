#ifndef DIALOGMAIL_H
#define DIALOGMAIL_H

#include <curl/curl.h>
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include<iostream>

namespace Ui {
class DialogMail;
}

class DialogMail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMail(QWidget *parent = nullptr,QStringList filePathsPdf = {});
    auto mailSender(QString& recipient, QString& subject, QString& message);
    ~DialogMail();

    QString fileName;
    QStringList m_filePathsPdf;

private slots:
    void on_pbExit_clicked();
    void on_pbBrowse_clicked();
    void on_pbSend_clicked();


private:
    Ui::DialogMail *ui;
};

#endif // DIALOGMAIL_H
