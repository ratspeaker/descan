#include "headers/dialogmail.h"
#include "ui_dialogmail.h"

DialogMail::DialogMail(QWidget *parent, QStringList filePathsPdf):
    QDialog(parent),
    m_filePathsPdf(filePathsPdf),
    ui(new Ui::DialogMail)
{
    ui->setupUi(this);
    this->setWindowTitle("Send Mail");

    for (auto& path:m_filePathsPdf) {
        auto fileName = path.right(path.size() - path.lastIndexOf('/') - 1);
        ui->teAttach->append(fileName);
    }
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
    QString browseFilePath = QFileDialog::getOpenFileName(this, tr("Import Document"), "/home/");
    QString browseFileName = browseFilePath.right(browseFilePath.size() - browseFilePath.lastIndexOf('/') - 1);
    ui->teAttach->append(browseFileName);
    m_filePathsPdf.append(browseFilePath);
}

const QStringList DialogMail::mailContent(QString& recipient, QString& subject) {
    return {tr("To: <%1>").arg(recipient),
            "From: <descan.soft@gmail.com> Descan Soft",
            "Message-ID: <dcd7cb36-21db-687a-9f3a-e657a9452efd@",
            tr("Subject: %1").arg(subject),
            nullptr};
}

std::string DialogMail::passwordReader() {
    QString path = QDir("..").absoluteFilePath("sifra.txt");
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    auto password = file.readAll();
    file.close();
    return password.toStdString();
}

void DialogMail::messageInit(curl_mime* &mime, curl_mime* &alt, CURL* curl) {
    mime = curl_mime_init(curl);
    alt = curl_mime_init(curl);
}

void DialogMail::messageText(curl_mimepart* &part, curl_mime* mime, curl_mime* alt, QString& message) {
    part = curl_mime_addpart(alt);
    curl_mime_data(part, message.toStdString().c_str(), CURL_ZERO_TERMINATED);
    part = curl_mime_addpart(mime);
    curl_mime_subparts(part, alt);
    curl_mime_type(part, "multipart/alternative");
}

void DialogMail::addAttachment(curl_mimepart* &part, curl_mime* mime, CURL* curl) {
    for (auto &path: m_filePathsPdf) {
        part = curl_mime_addpart(mime);
        curl_mime_encoder(part, "base64");
        curl_mime_filedata(part, path.toStdString().c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }
}

void DialogMail::initStructRecipients(struct curl_slist* &recipients, QString& recipient, CURL* curl) {
    recipients = curl_slist_append(recipients, recipient.toStdString().c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
}

void DialogMail::initStructHeaders(const QStringList content, struct curl_slist* &headers, CURL* curl) {
    for (auto &con: content) {
        headers = curl_slist_append(headers, con.toStdString().c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

void DialogMail::initStructSlist(struct curl_slist* &slist, curl_mimepart* part) {
    slist = curl_slist_append(NULL, "Content-Disposition: inline");
    curl_mime_headers(part, slist, 1);
}

CURLcode DialogMail::mailSender(QString& recipient, QString& subject, QString& message) {

    const QStringList content = mailContent(recipient, subject);

    CURL *curl = nullptr;
    curl = curl_easy_init();

    CURLcode res = CURLE_OK;

    if (curl) {
        std::string password = passwordReader();

        curl_easy_setopt(curl, CURLOPT_USERNAME, "descan.soft@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com");

        #ifdef SKIP_PEER_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

        #ifdef SKIP_HOSTNAME_VERIFICATION
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, "<descan.soft@gmail.com>");

        struct curl_slist *recipients = NULL;
        initStructRecipients(recipients, recipient, curl);

        struct curl_slist *headers = NULL;
        initStructHeaders(content, headers, curl);

        //inicijalizuje se poruka
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        messageInit(mime, alt, curl);

        //dodaje se tekst poruke
        curl_mimepart *part = nullptr;
        messageText(part, mime, alt, message);

        struct curl_slist *slist = NULL;
        initStructSlist(slist, part);

        addAttachment(part, mime, curl);

        //salje se poruka i kupi se rezultat
        res = curl_easy_perform(curl);

        //oslobadjaju se strukture i raskida konekcija
        curl_slist_free_all(recipients);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        //oslobadja se poruka koja je poslata
        curl_mime_free(mime);
    }

    qDebug() << static_cast<int>(res); //0
    return res;
}

void DialogMail::on_pbSend_clicked()
{
    QString recipient = ui->leSendTo->text();
    QString subject = ui->leSubject->text();
    QString message = ui->pteMessage->toPlainText();

    auto res = mailSender(recipient, subject, message);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        QMessageBox::warning(this, "Email", "Email has not been sent!");
    }
    else {
        QMessageBox::information(this, "Email", "Email has been sent.");
    }
}
