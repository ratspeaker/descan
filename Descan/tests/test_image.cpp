#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../headers/image.h"

TEST_CASE("Image", "[class]")
{
    SECTION("My first test")
    {
        REQUIRE(1 == 1);
    }
}
