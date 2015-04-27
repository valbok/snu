/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "SnuPuck.hpp"
#include <cmath>

#include <iostream>
using namespace std;

namespace NSnu
{

SnuPuck::SnuPuck() throw()
    : mLeftSpeed(0)
    , mRightSpeed(0)
    , mDirection(1)
    , mDirectionChanged(false)
{
}

bool SnuPuck::frontOrBackTouched() const
{
    const unsigned minDist = 3;
    return infraredSensor0.getDist() < minDist ||
        infraredSensor7.getDist() < minDist ||
        infraredSensor3.getDist() < minDist ||
        infraredSensor4.getDist() < minDist;
}

void SnuPuck::step(float h, unsigned period)
{
    const unsigned minFrontDist = 3;
    const unsigned minSideDist = 3;

    if (frontOrBackTouched())
    {
        mBrain.touchNose();
    }
    else if (infraredSensor5.getDist() < minSideDist)
    {
        mBrain.touchLeft();
    }
    else if (infraredSensor2.getDist() < minSideDist)
    {
        mBrain.touchRight();
    }
    else
    {
        mBrain.findFood();
    }

    mBrain.step(h, period);
    drive();
}

void SnuPuck::drive()
{
    /*
    unsigned left[SIDE_MUSCLE_NEURONS][2] = {0};
    unsigned right[SIDE_MUSCLE_NEURONS][2] = {0};
    int sumLeft = 0, sumRight = 0;
    mBrain.getMusclesActivites(left, right);
    for (int i = 0; i < SIDE_MUSCLE_NEURONS; ++i)
    {
        sumLeft += left[i][0] + left[i][1];
        sumRight += right[i][0] + right[i][1];
    }
    mLeftSpeed += sumLeft;
    mRightSpeed += sumRight;
    */

    float dorsalMagnitudes[MUSCLE_MAGNITUDES] = {0};
    float ventralMagnitudes[MUSCLE_MAGNITUDES] = {0};
    mBrain.getVentralMagnitudes(dorsalMagnitudes);
    mBrain.getDorsalMagnitudes(ventralMagnitudes);
    for (int i = 0; i < MUSCLE_MAGNITUDES; ++i)
    {
        float diff = dorsalMagnitudes[i] - ventralMagnitudes[i];
        if (diff < 0)
        {
            mLeftSpeed += fabs(diff);
        }
        else
        {
            mRightSpeed += diff;
        }
    }
}

void SnuPuck::setNosePeriodFrequency(float value)
{
    mBrain.setNosePeriodFrequency(value);
}

void SnuPuck::setFoodPeriodFrequency(float value)
{
    mBrain.setFoodPeriodFrequency(value);
}

void SnuPuck::stop()
{
    mLeftSpeed = mRightSpeed = 0;
}

void SnuPuck::move()
{
    bool touched = mBrain.noseTouched();
    if (!mDirectionChanged && touched)
    {
        // Moving forward? Now move backward.
        if (leftSpeed >= 0)
        {
            mDirection = -1;
            leftSpeed = -mLeftSpeed;
        }
        else // Moving backward? Move forward.
        {
            leftSpeed = mLeftSpeed;
            mDirection = 1;
        }

        if (rightSpeed >= 0)
        {
            mDirection = -1;
            rightSpeed = -mRightSpeed;
        }
        else
        {
            rightSpeed = mRightSpeed;
            mDirection = 1;
        }

        // If touched need to move to different direction.
        mDirectionChanged = true;
    }
    else
    {
        // Keep previous direction.
        leftSpeed = mDirection >= 0 ? mLeftSpeed : (mLeftSpeed != 0 ? -mLeftSpeed : 0);
        rightSpeed = mDirection >= 0 ? mRightSpeed : (mRightSpeed != 0 ? -mRightSpeed : 0);
        // Keep moving.
        if (!touched)
        {
            mDirectionChanged = false;
        }
    }
}

} // namespace NSnu
