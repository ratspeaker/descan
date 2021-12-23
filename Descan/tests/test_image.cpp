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
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        REQUIRE(img.size() == img.getImage().size());
    }

    SECTION("Metod isNull uspesno proverava da li slika zapravo postoji")
    {
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        REQUIRE(img.isNull() == img.getImage().isNull());
    }

    SECTION("Metod getScaleFactor uspesno vraca vrednost polja scaleFactor")
    {
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        REQUIRE(img.getScaleFactor() == 0.0);
    }

    SECTION("Metod setScaleFactor uspesno postavlja vrednost polja scaleFactor")
    {
        const QString filePath = "../jojo.png";
        Image img = Image(filePath);
        img.setScaleFactor(1.0);
        REQUIRE(img.getScaleFactor() == 1.0);
    }
}
