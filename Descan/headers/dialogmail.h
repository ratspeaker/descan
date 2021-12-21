#ifndef DIALOGMAIL_H
#define DIALOGMAIL_H

#include <curl/curl.h>
#include <iostream>
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class DialogMail;
}

class DialogMail : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMail(QWidget *parent = nullptr, QStringList filePathsPdf = {});
    ~DialogMail();

    const QStringList mailContent(QString& recipient, QString& subject);
    std::string passwordReader();
    void initSettings(CURL* curl, std::string password);
    void messageInit(curl_mime* &mime, curl_mime* &alt, CURL* curl);
    void messageText(curl_mimepart* &part, curl_mime* mime, curl_mime* alt, QString& message);
    void addAttachment(curl_mimepart* &part, curl_mime* mime, CURL* curl);
    void initStructRecipients(struct curl_slist* recipients, QString& recipient, CURL* curl);
    void initStructHeaders(const QStringList content, struct curl_slist* headers, CURL* curl);
    void initStructSlist(struct curl_slist* slist, curl_mimepart* part);

    auto mailSender(QString& recipient, QString& subject, QString& message);

    QStringList m_filePathsPdf;

private slots:
    void on_pbExit_clicked();
    void on_pbBrowse_clicked();
    void on_pbSend_clicked();

private:
    Ui::DialogMail *ui;
};

#endif // DIALOGMAIL_H
