/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/worm/CElegans.hpp>
#include <gtest/gtest.h>
#include <iostream>

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

        EXPECT_FLOAT_EQ(0.10944445, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.13111112, worm.getNosePeriodFrequency());
    }

    {
        CElegans worm;
        for(int t = 0; t < tries * 2; ++t)
        {
            worm.teachNoseDirection(step, period);
        }

        for(int t = 0; t < tries * 2; ++t)
        {
            worm.teachFoodDirection(step, period);
        }

        EXPECT_FLOAT_EQ(0.096111104, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.12611112, worm.getNosePeriodFrequency());
    }

    {
        CElegans worm;
        for(int t = 0; t < tries * 10; ++t)
        {
            worm.teachNoseDirection(step, period);
        }

        for(int t = 0; t < tries * 10; ++t)
        {
            worm.teachFoodDirection(step, period);
        }

        EXPECT_FLOAT_EQ(0.094898984, worm.getFoodPeriodFrequency());
        EXPECT_FLOAT_EQ(0.12469697, worm.getNosePeriodFrequency());
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
        if (t < foodBefore || t > foodAfter)
        {
            worm.findFood();
            touched = false;
        }
        else
        {
            worm.touchNose();
            touched = true;
        }

        worm.step(step, period);
        if (touched && t > foodBefore + period + 1)
        {
            EXPECT_EQ(touched, worm.noseTouched());
        }
    }
}
