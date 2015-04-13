/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;

TEST(CElegans, test)
{
    CElegans celegans;

    EXPECT_EQ(397, celegans.getNeuronsCount());

}

