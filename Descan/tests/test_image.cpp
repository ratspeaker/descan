#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/image.h"

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

        // Act + Assert
        REQUIRE(!img.getImage().isNull());
    }

    SECTION("Metod setImage uspesno postavlja vrednost polja m_image")
    {
        // Arrange
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        const QString newFilePath = "../sneg.jpg";
        QImage qimg = QImage(newFilePath);

        // Act
        QImage value = img.getImage();
        img.setImage(qimg);
        QImage newValue = img.getImage();

        // Assert
        REQUIRE(value != newValue);
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
}
