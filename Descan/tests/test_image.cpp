#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/image.h"
#include "QGuiApplication"

TEST_CASE("Image", "[class]")
{
    SECTION("Konstruktor Image uspesno konstruise objekat")
    {
        // Arrange
        const QString filePath = "../jojo.png";

        // Act + Assert
        REQUIRE_NOTHROW(Image(filePath));
    }

    SECTION("Metod getImage uspesno vraca vrednost polja m_image")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        QImage qimg = QImage(filePath);

        // Act + Assert
        REQUIRE(img.getImage() == qimg);
    }

    SECTION("Metod setImage uspesno postavlja vrednost polja m_image")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        const QString newFilePath = "../sneg.jpg";
        QImage qimg = QImage(newFilePath);

        // Act
        img.setImage(qimg);

        // Assert
        REQUIRE(img.getImage() == qimg);
    }

    SECTION("Metod getFilePath uspesno vraca vrednost polja m_filePath")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act + Assert
        REQUIRE(img.getFilePath() == filePath);
    }

    SECTION("Metod setFilePath uspesno postavlja vrednost polja m_filePath")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        const QString newFilePath = "../sneg.jpg";

        // Act
        img.setFilePath(newFilePath);

        // Assert
        REQUIRE(img.getFilePath() == newFilePath);
    }

    SECTION("Metod size uspesno vraca velicinu slike")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act + Assert
        REQUIRE(img.size() == QSize(1366, 768));
    }

    SECTION("Metod isNull uspesno proverava da li slika zapravo postoji")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        const QString filePath2 = "../nePostoji.png";
        Image img2 = Image(filePath2);

        // Act + Assert
        REQUIRE(!img.isNull());
        REQUIRE(img2.isNull());
    }

    SECTION("Metod getScaleFactor uspesno vraca vrednost polja scaleFactor")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act + Assert
        REQUIRE(img.getScaleFactor() == 0.0);
    }

    SECTION("Metod setScaleFactor uspesno postavlja vrednost polja scaleFactor")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act
        img.setScaleFactor(1.0);

        // Assert
        REQUIRE(img.getScaleFactor() == 1.0);
    }

    SECTION("Metod getSlider uspesno vraca vrednost polja sliders (vraca celu mapu)")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        std::map<QString, int> ocekivanRezultat = {{"scale", 0}, {"hor", 0}, {"ver", 0},
                                                   {"brigh", 0}, {"con", 0}, {"gam", 0}, {"sat", 0}};

        // Act
        std::map<QString, int> dobijenRezultat = img.getSlider();

        // Assert
        REQUIRE(dobijenRezultat == ocekivanRezultat);
    }

    SECTION("Metod setSlider uspesno postavlja vrednost polja sliders (postavlja vrednost jednog elementa mape)")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act
        img.setSlider("scale", 1);
        img.setSlider("brigh", 2);

        // Assert
        REQUIRE(img.getSlider()["scale"] == 1);
        REQUIRE(img.getSlider()["brigh"] == 2);
    }

    SECTION("Metod resizeImage uspesno menja dimenzije slike i vraca izmenjenu sliku")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        double factor = 1.2;

        // Act
        QImage qimg1 = img.resizeImage(factor, 'w');
        QImage qimg2 = img.resizeImage(factor, 'h');
        QImage qimg3 = img.resizeImage(factor, 's');

        // Assert
        REQUIRE(qimg1.width() == static_cast<int>(1366 * factor));
        REQUIRE(qimg2.height() == static_cast<int>(768 * factor));
        REQUIRE(qimg3.size() == QSize(1366 * factor, 768 * factor));
    }

    SECTION("Metod changeBrightness uspesno menja osvetljenje slike i vraca izmenjenu sliku")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        double factor = 80;

        // Act
        QImage qimg = img.changeBrightness(factor);

        // Assert
        REQUIRE(!qimg.isNull());
        // da li se promenila boja random pixela
        REQUIRE(qimg.pixel(1200, 120) != img.getImage().pixel(1200, 120));
    }

    SECTION("Metod width uspesno vraca sirinu slike")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act + Assert
        REQUIRE(img.width() == 1366);
    }

    SECTION("Metod height uspesno vraca visinu slike")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act + Assert
        REQUIRE(img.height() == 768);
    }

    SECTION("Metod printImagesIntoPdf uspesno iscrtava sliku 'na papiru'")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        int x = 1;
        QGuiApplication app(x, nullptr);
        QPdfWriter pdfWriter("../jojo.pdf");
        QPainter painter(&pdfWriter);

        // Act + Assert
        REQUIRE_NOTHROW(img.printImageIntoPdf(painter));
        // pdf fajl se promenio, drugacije se stampa slika
    }

    SECTION("Metod saveAction uspesno dodaje par <slika, slajderi> na vrh steka undoStack")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);

        // Act
        img.saveAction();

        // Assert
        REQUIRE(!img.undoStack.empty());
    }

    SECTION("Metod undoAction uspesno dodaje element na vrh steka redoStack, a skida element sa vrha steka undoStack")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        img.saveAction();
        int ocekivanRez1 = img.undoStack.size() - 1;
        int ocekivanRez2 = img.redoStack.size() + 1;

        // Act
        img.undoAction();
        int dobijenRez1 = img.undoStack.size();
        int dobijenRez2 = img.redoStack.size();

        // Assert
        REQUIRE(dobijenRez1 == ocekivanRez1);
        REQUIRE(dobijenRez2 == ocekivanRez2);
    }

    SECTION("Metod redoAction uspesno dodaje element na vrh steka undoStack, a skida element sa vrha steka redoStack")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        img.saveAction();
        img.undoAction();
        int ocekivanRez1 = img.undoStack.size() + 1;
        int ocekivanRez2 = img.redoStack.size() - 1;

        // Act
        img.redoAction();
        int dobijenRez1 = img.undoStack.size();
        int dobijenRez2 = img.redoStack.size();

        // Assert
        REQUIRE(dobijenRez1 == ocekivanRez1);
        REQUIRE(dobijenRez2 == ocekivanRez2);
    }

    SECTION("Metod emptyUndoActions uspesno prazni stek undoStack")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        img.saveAction();

        // Act
        img.emptyUndoActions();

        // Assert
        REQUIRE(img.undoStack.empty());
    }

    SECTION("Metod emptyRedoActions uspesno prazni stek redoStack")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        img.saveAction();
        img.undoAction();

        // Act
        img.emptyRedoActions();

        // Assert
        REQUIRE(img.redoStack.empty());
    }

    SECTION("Metod cropImage uspesno sece sliku od zadatog gornjeg levog ugla do zadatog donjeg desnog ugla")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        int x1 = 390;
        int y1 = 220;
        int x2 = 1160;
        int y2 = 650;

        // Act
        img.cropImage(QPoint(x1, y1), QPoint(x2, y2));

        // Assert
        REQUIRE(img.width()  == x2-x1+1);
        REQUIRE(img.height() == y2-y1+1);
    }

    SECTION("Metod rotateImage uspesno rotira sliku za 90 stepeni")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        int width  = img.width();
        int height = img.height();
        int x = 1;
        QGuiApplication app(x, nullptr);

        // Act
        img.rotateImage(90);

        // Assert
        REQUIRE(img.width()  == height);
        REQUIRE(img.height() == width);
    }
}
