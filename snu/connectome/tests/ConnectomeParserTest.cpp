/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/connectome/ConnectomeParser.hpp>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;

TEST(ConnectomeParser, testParseNonexistentFile)
{
    ConnectomeParser parser("data/connectome.csv");
    ConnectomeParser::TConnections t;

    EXPECT_FALSE(parser.parse(t));
}

TEST(ConnectomeParser, testEmptyFile)
{
    ConnectomeParser parser("../data/empty.csv");
    ConnectomeParser::TConnections t;

    EXPECT_FALSE(parser.parse(t));
}

TEST(ConnectomeParser, testParse)
{
    ConnectomeParser parser("../data/connectome.csv");
    ConnectomeParser::TConnections t;
    EXPECT_TRUE(parser.parse(t));
    EXPECT_EQ(3, t.size());
    EXPECT_EQ("ADAL", std::get<0>(t[0]));
    EXPECT_EQ("ADAR", std::get<1>(t[0]));
    EXPECT_EQ(1, std::get<2>(t[0]));

    EXPECT_EQ("ADAL", std::get<0>(t[1]));
    EXPECT_EQ("ADFL", std::get<1>(t[1]));
    EXPECT_EQ(-1, std::get<2>(t[1]));

    EXPECT_EQ("ADEL", std::get<0>(t[2]));
    EXPECT_EQ("ADFL", std::get<1>(t[2]));
    EXPECT_EQ(10, std::get<2>(t[2]));
}

