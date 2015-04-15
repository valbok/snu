/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/brain/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;

TEST(CElegans, testNeurons)
{
    CElegans celegans;

    EXPECT_EQ(397, celegans.getNeuronsCount());
    EXPECT_TRUE(celegans.hasNeuron("ADAL"));
    EXPECT_FALSE(celegans.hasNeuron("UNKNOWN"));
}

