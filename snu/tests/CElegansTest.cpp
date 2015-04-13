/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;

TEST(CElegans, testNeurons)
{
    CElegans celegans;

    EXPECT_EQ(397, celegans.getNeuronsCount());
    EXPECT_EQ(true, celegans.hasNeuron("ADAL"));
    EXPECT_EQ(false, celegans.hasNeuron("UNKNOWN"));
    try
    {
        celegans.getNeuron("UNKNOWN");
    }
    catch (std::out_of_range)
    {
    }
}

