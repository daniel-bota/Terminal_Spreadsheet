#include "pch.h"
#include "Address.h"
#include "Coordinates.h"
#include "Position.h"
#include <vector>

namespace CoordinateFacts
{
    TEST(Coordinates, HasValidSetter) 
    {
        Address address(1, 1);
        std::vector<int> results;
        results.push_back(address.Col());
        results.push_back(address.Row());

        std::vector<int> expected{ 1, 1 };
        EXPECT_EQ(expected, results);
    }

    TEST(Coordinates, HasValidEqualityComparer)
    {
        EXPECT_TRUE(Position(1, 1) == Position(1, 1));
        EXPECT_TRUE(Address(1, 1) < Address(1, 3));
        EXPECT_FALSE(Position(1, 1) == Position(1, 2));
    }
};