#include "catch.hpp"
#include "../headers/dialogmail.h"
#include <QApplication>
#include <QStringList>

TEST_CASE("DialogMail", "[class]")
{
    SECTION("Konstruktor DialogMail uspesno konstruise objekat")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};

        // Act + Assert
        REQUIRE_NOTHROW(DialogMail(parent, filePathsPdf));
    }

    SECTION("Metod mailContent uspesno popunjava sadrzaj mejla odredjenim informacijama")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        QString recipient = "andjelijaaam@gmail.com";
        QString subject = "subj";
        const QStringList ocekivanIzlaz = {
            "To: <andjelijaaam@gmail.com>",
            "From: <descan.soft@gmail.com> Descan Soft",
            "Message-ID: <dcd7cb36-21db-687a-9f3a-e657a9452efd@",
            "Subject: subj", ""};

        // Act
        const QStringList dobijenIzlaz = mail.mailContent(recipient, subject);

        // Assert
        REQUIRE(dobijenIzlaz == ocekivanIzlaz);
    }

    SECTION("Metod passwordReader uspesno cita sifru iz fajla sifra.txt")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);

        // Act
        std::string dobijenIzlaz = "";
        dobijenIzlaz = mail.passwordReader();

        // Assert
        REQUIRE(!dobijenIzlaz.empty());
    }

    SECTION("Metod messageInit uspesno inicijalizuje poruku")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        CURL *curl = curl_easy_init();

        // Act
        mail.messageInit(mime, alt, curl);

        // Assert
        REQUIRE(mime);
        REQUIRE(alt);
    }

    SECTION("Metod messageText uspesno dodaje tekst poruke koja se salje")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        curl_mimepart *part = nullptr;
        CURL *curl = curl_easy_init();
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        mail.messageInit(mime, alt, curl);
        QString message = "Tekst poruke";

        // Act
        mail.messageText(part, mime, alt, message);

        // Assert
        REQUIRE(part);
    }

    SECTION("Metod addAttachment uspesno dodaje jedan fajl kao attachment poruke koja se salje")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {"nazivFajla.pdf"};
        DialogMail mail(parent, filePathsPdf);
        curl_mimepart *part = nullptr;
        CURL *curl = curl_easy_init();
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        mail.messageInit(mime, alt, curl);

        // Act
        mail.addAttachment(part, mime, curl);

        // Assert
        REQUIRE(part);
    }

    SECTION("Metod addAttachment uspesno dodaje vise fajlova kao attachment poruke koja se salje")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {"prvi.pdf", "drugi.pdf", "treci.pdf"};
        DialogMail mail(parent, filePathsPdf);
        curl_mimepart *part = nullptr;
        CURL *curl = curl_easy_init();
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        mail.messageInit(mime, alt, curl);

        // Act
        mail.addAttachment(part, mime, curl);

        // Assert
        REQUIRE(part);
    }

    SECTION("Metod initStructRecipients uspesno inicijalizuje strukturu recipients")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        struct curl_slist *recipients = NULL;
        QString recipient = "andjelijaaam@gmail.com";
        CURL *curl = curl_easy_init();

        // Act
        mail.initStructRecipients(recipients, recipient, curl);

        // Assert
        REQUIRE(recipients != NULL);
    }

    SECTION("Metod initStructHeaders uspesno inicijalizuje strukturu headers")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        struct curl_slist *headers = NULL;
        CURL *curl = curl_easy_init();
        QString recipient = "andjelijaaam@gmail.com";
        QString subject = "subj";
        const QStringList content = mail.mailContent(recipient, subject);

        // Act
        mail.initStructHeaders(content, headers, curl);

        // Assert
        REQUIRE(headers != NULL);
    }

    SECTION("Metod initStructSlist uspesno inicijalizuje strukturu slist")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {};
        DialogMail mail(parent, filePathsPdf);
        struct curl_slist *slist = NULL;
        curl_mimepart *part = nullptr;
        CURL *curl = curl_easy_init();
        curl_mime *mime = nullptr;
        curl_mime *alt  = nullptr;
        mail.messageInit(mime, alt, curl);
        QString message = "Tekst poruke";
        mail.messageText(part, mime, alt, message);

        // Act
        mail.initStructSlist(slist, part);

        // Assert
        REQUIRE(slist != NULL);
    }

    SECTION("Metod mailSender uspesno salje mejl")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {"../jojo.pdf"};
        DialogMail mail(parent, filePathsPdf);
        QString recipient = "andjelijaaam@gmail.com";
        QString subject = "subj";
        QString message = "mess";
        CURLcode ocekivanIzlaz = CURLE_OK;

        // Act
        CURLcode dobijenIzlaz = mail.mailSender(recipient, subject, message);

        // Assert
        REQUIRE(dobijenIzlaz == ocekivanIzlaz);
    }

    SECTION("Metod mailSender ne salje mejl ukoliko se ne unese primalac")
    {
        // Arrange
        int x = 1;
        QApplication app(x, nullptr);
        QWidget *parent = nullptr;
        QStringList filePathsPdf = {"../jojo.pdf"};
        DialogMail mail(parent, filePathsPdf);
        QString recipient = "";
        QString subject = "subj";
        QString message = "mess";

        // Act + Assert
        REQUIRE(mail.mailSender(recipient, subject, message) != CURLE_OK);
    }
}
