/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/brain/CElegansBrain.hpp>
#include <gtest/gtest.h>

using namespace NSnu;
using namespace std;

TEST(CElegansBrain, testNeurons)
{
    CElegansBrain worm;

    EXPECT_EQ(397, worm.getNeuronsCount());
    EXPECT_TRUE(worm.hasNeuron("ADAL"));
    EXPECT_FALSE(worm.hasNeuron("UNKNOWN"));
}
