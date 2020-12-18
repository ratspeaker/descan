#include "headers/dialogmail.h"
#include "ui_dialogmail.h"
#include <QFileDialog>
#include <QDebug>
#include<ctime>
#include<string>

#include <cstdio>
#include <cstring>

#define FROM    "<descan.soft@gmail.com>"
#define TO      "<matf.materijali@gmail.com>"
#define CC      "<info@example.org>"

DialogMail::DialogMail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMail)
{
    ui->setupUi(this);
    this->setWindowTitle("Send Mail");
}

DialogMail::~DialogMail()
{
    delete ui;
}

void DialogMail::on_pbExit_clicked()
{
    close();
}

void DialogMail::on_pbBrowse_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Import Document"), "/home/");
    ui->leAttach->setText(fileName);
}



void DialogMail::on_pbSend_clicked()
{

    QString recipient = ui->leSendTo->text();
    QString subject = ui->leSubject->text();
    QString message = ui->pteMessage->toPlainText();

    const QStringList emailContent{ tr("To: <%1>").arg(recipient),
                                   "From: <descan.soft@gmail.com> Descan Soft",
                                   "Message-ID: <dcd7cb36-21db-687a-9f3a-e657a9452efd@",
                                    tr("Subject: %1").arg(subject),
                                   nullptr};

      CURL *curl;
      CURLcode res = CURLE_OK;

      // Ovo mora da se prvo pozove i vraca hendler koji se koristi dole u funkcijama
      curl = curl_easy_init();

      if(curl) {
        struct curl_slist *headers = NULL;
        struct curl_slist *recipients = NULL;
        struct curl_slist *slist = NULL;
        curl_mime *mime;
        curl_mime *alt;
        curl_mimepart *part;

        // Posiljalac,sifra i server se podesavaju
        curl_easy_setopt(curl, CURLOPT_USERNAME, "descan.soft@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "descanRS2020");

        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");


    #ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    #endif

    #ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    #endif

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<descan.soft@gmail.com>");

        // Ovde dodajemo primaoca
        recipients = curl_slist_append(recipients, recipient.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // dodajemo u strukturu headers delove mejla koji se salje
        for(auto &con: emailContent)
            headers = curl_slist_append(headers, con.toStdString().c_str());


        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Inicijalizuje se poruka
        mime = curl_mime_init(curl);


        alt = curl_mime_init(curl);

        // Dodaje se tekst poruke
        part = curl_mime_addpart(alt);
        curl_mime_data(part, message.toStdString().c_str(), CURL_ZERO_TERMINATED);


        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");
        slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);

        // Dodaje se attachment
        part = curl_mime_addpart(mime);
        curl_mime_encoder(part, "base64");
        curl_mime_filedata(part, fileName.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);


        // Salje se poruka i kupi se rezultat
        res = curl_easy_perform(curl);

        //Proverava se da li je poruka poslata
        if(res != CURLE_OK) {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));
        QMessageBox::warning(this,"Email","Email has not been sent!");
        }

        // Oslobadjaju se strukture i raskida konekcija
        curl_slist_free_all(recipients);
        curl_slist_free_all(headers);

        curl_easy_cleanup(curl);

        // Oslobadja se poruka koja je poslata
        curl_mime_free(mime);
      }

      qDebug() << static_cast<int>(res);

}
