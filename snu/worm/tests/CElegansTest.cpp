/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/worm/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

using namespace NSnu;
using namespace std;

TEST(CElegans, testTeachingDirection)
{
    const float step = 0.5f;
    const unsigned period = 1800;
    const unsigned tries = 2000;

    {
        CElegans worm;
        for(int t = 0; t < tries; ++t)
        {
            worm.teachNoseDirection(step, period);
        }

        for(int t = 0; t < tries; ++t)
        {
            worm.teachFoodDirection(step, period);
        }

        EXPECT_FLOAT_EQ(0.098888889, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.12333333, worm.getNosePeriodFrequency());
    }

    {
        CElegans worm;
        for(int t = 0; t < tries*2; ++t)
        {
            worm.teachNoseDirection(step, period);
        }

        for(int t = 0; t < tries*2; ++t)
        {
            worm.teachFoodDirection(step, period);
        }

        EXPECT_FLOAT_EQ(0.095277779, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.12333333, worm.getNosePeriodFrequency());
    }

    {
        CElegans worm;
        for(int t = 0; t < tries*10; ++t)
        {
            worm.teachNoseDirection(step, period);
        }

        for(int t = 0; t < tries*10; ++t)
        {
            worm.teachFoodDirection(step, period);
        }

        EXPECT_FLOAT_EQ(0.087575756, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.1190404, worm.getNosePeriodFrequency());
    }
}

TEST(CElegans, testMovingDirection)
{
    CElegans worm;
    const float step = 0.5f;
    const unsigned period = 1800;
    const unsigned tries = 40000;

    for(int t = 0; t < tries; ++t)
    {
        worm.teachNoseDirection(step, period);
    }

    for(int t = 0; t < tries; ++t)
    {
        worm.teachFoodDirection(step, period);
    }

    bool touched = true;
    const unsigned foodBefore = 10113;
    const unsigned foodAfter = 15948;
    for(int t = 0; t < 20000; ++t)
    {
        if (t < foodBefore || t>foodAfter){
            worm.findFood();
            touched = true;
        }
        else
        {
            worm.touchNose();
            touched = false;
        }

        worm.step(step, period);
        if (t < foodBefore - period + 1 || t > foodAfter + period - 1)
        {
            EXPECT_EQ(touched, worm.noseTouched());
        }
    }
}
