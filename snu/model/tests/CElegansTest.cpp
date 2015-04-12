/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/model/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;

TEST(CElegans, test)
{
    ConnectomeParser parser("../../../connectome/data/celegans.csv");
    ConnectomeParser::TConnectome t;
    EXPECT_TRUE(parser.parse(t));
    CElegans celegans(t);

    EXPECT_EQ(398, celegans.getNeuronsCount());

}

