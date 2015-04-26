/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "CElegans.hpp"
#include <cmath> // fabs

namespace NSnu
{

CElegans::CElegans() throw()
    : mPeriodIndex(0)
    , mPeriodSpikesCount(0)
    , mNoseTouched(false)
    , mAverageNosePeriodFrequency(0)
    , mAverageFoodPeriodFrequency(0)
    , mSumNoseFrequencies(0)
    , mSumFoodFrequencies(0)
    , mTeachNoseIndex(0)
    , mTeachFoodIndex(0)
{
}

void CElegans::step(float h, unsigned period)
{
    mBrain.step(h);
    calculateDirection(period);
}

float CElegans::getFoodPeriodFrequency() const
{
    return mAverageFoodPeriodFrequency;
}

void CElegans::setFoodPeriodFrequency(float value)
{
    mAverageFoodPeriodFrequency = value;
}

float CElegans::getNosePeriodFrequency() const
{
    return mAverageNosePeriodFrequency;
}

void CElegans::setNosePeriodFrequency(float value)
{
    mAverageNosePeriodFrequency = value;
}

bool CElegans::getPeriodFrequency(unsigned period, float& value)
{
    bool result = false;
    if (mPeriodIndex > period)
    {
        value = mPeriodSpikesCount / (float)period;
        mPeriodSpikesCount = 0;
        mPeriodIndex = 0;
        result = true;
    }
    else
    {
        float va = getAverageVA();
        // Calculate how many times neurons have been activated.
        if (va > 0)
        {
            ++mPeriodSpikesCount;
        }

        ++mPeriodIndex;
    }

    return result;
}

void CElegans::teachFoodDirection(float h, unsigned period)
{
    findFood();
    mBrain.step(h);
    float f = 0;
    if (getPeriodFrequency(period, f))
    {
        ++mTeachFoodIndex;
        mSumFoodFrequencies += f;
        mAverageFoodPeriodFrequency = mSumFoodFrequencies / mTeachFoodIndex;
    }
}

void CElegans::teachNoseDirection(float h, unsigned period)
{
    touchNose();
    mBrain.step(h);
    float f = 0;
    if (getPeriodFrequency(period, f))
    {
        ++mTeachNoseIndex;
        mSumNoseFrequencies += f;
        mAverageNosePeriodFrequency = mSumNoseFrequencies / mTeachNoseIndex;
    }
}

void CElegans::calculateDirection(unsigned period)
{
    float f = 0;
    if (getPeriodFrequency(period, f))
    {
        float toNose = fabs(f - mAverageNosePeriodFrequency);
        float toFood = fabs(f - mAverageFoodPeriodFrequency);

        mNoseTouched = toNose < toFood;
    }
}

float CElegans::getAverageVA() const
{
    return (
        mBrain.fired("VA1") +
        mBrain.fired("VA2") +
        mBrain.fired("VA3") +
        mBrain.fired("VA4") +
        mBrain.fired("VA5") +
        mBrain.fired("VA6") +
        mBrain.fired("VA7") +
        mBrain.fired("VA8") +
        mBrain.fired("VA9") +
        mBrain.fired("VA10") +
        mBrain.fired("VA11") +
        mBrain.fired("VA12")
        ) / 12.0f;
}

bool CElegans::noseTouched() const
{
    return mNoseTouched;
}

void CElegans::findFood()
{
    mBrain.spike("ADFL");
    mBrain.spike("ADFR");
    mBrain.spike("ASGL");
    mBrain.spike("ASGR");
    mBrain.spike("ASIL");
    mBrain.spike("ASIR");
    mBrain.spike("ASJR");
    mBrain.spike("ASJL");
    mBrain.spike("AWCL");
    mBrain.spike("AWCR");
    mBrain.spike("AWAL");
    mBrain.spike("AWAR");
}

void CElegans::touchNose()
{
    mBrain.spike("ASHL");
    mBrain.spike("ASHR");
    mBrain.spike("FLPL");
    mBrain.spike("FLPR");
    mBrain.spike("OLQDL");
    mBrain.spike("OLQDR");
    mBrain.spike("OLQVL");
    mBrain.spike("OLQVR");

    mBrain.spike("IL1VL");
    mBrain.spike("IL1VR");
    mBrain.spike("IL1L");
    mBrain.spike("IL1R");
    mBrain.spike("IL1DL");
    mBrain.spike("IL1DR");
}

void CElegans::touchLeft()
{
    mBrain.spike("PLML");
    mBrain.spike("PVDL");
    mBrain.spike("PDEL");
    mBrain.spike("PVM");
    mBrain.spike("LUAL");
}

void CElegans::touchRight()
{
    mBrain.spike("PLMR");
    mBrain.spike("PVDR");
    mBrain.spike("PDER");
    mBrain.spike("PVM");
    mBrain.spike("LUAR");
}

void CElegans::getDorsalMagnitudes(float result[]) const
{
    unsigned i = 0;
    float m = (
      mBrain.fired("MDL1") +
      mBrain.fired("MDL2") +
      mBrain.fired("MDR1") +
      mBrain.fired("MDR2")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL3") +
      mBrain.fired("MDL4") +
      mBrain.fired("MDR3") +
      mBrain.fired("MDR4")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL5") +
      mBrain.fired("MDL6") +
      mBrain.fired("MDR5") +
      mBrain.fired("MDR6")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL7") +
      mBrain.fired("MDL8") +
      mBrain.fired("MDR7") +
      mBrain.fired("MDR8")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL9") +
      mBrain.fired("MDL10") +
      mBrain.fired("MDR9") +
      mBrain.fired("MDR10")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL11") +
      mBrain.fired("MDL12") +
      mBrain.fired("MDR11") +
      mBrain.fired("MDR12")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL13") +
      mBrain.fired("MDL14") +
      mBrain.fired("MDR13") +
      mBrain.fired("MDR14")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL15") +
      mBrain.fired("MDL16") +
      mBrain.fired("MDR15") +
      mBrain.fired("MDR16")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL17") +
      mBrain.fired("MDL18") +
      mBrain.fired("MDR17") +
      mBrain.fired("MDR18")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL19") +
      mBrain.fired("MDL20") +
      mBrain.fired("MDR19") +
      mBrain.fired("MDR20")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL21") +
      mBrain.fired("MDL22") +
      mBrain.fired("MDR21") +
      mBrain.fired("MDR22")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MDL23") +
      mBrain.fired("MDL24") +
      mBrain.fired("MDR23") +
      mBrain.fired("MDR24")
      ) / 4.0f;
    result[i++] = m;
}

void CElegans::getVentralMagnitudes(float result[]) const
{
    unsigned i = 0;
    float m = (
      mBrain.fired("MVL1") +
      mBrain.fired("MVL2") +
      mBrain.fired("MVR1") +
      mBrain.fired("MVR2")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL3") +
      mBrain.fired("MVL4") +
      mBrain.fired("MVR3") +
      mBrain.fired("MVR4")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL5") +
      mBrain.fired("MVL6") +
      mBrain.fired("MVR5") +
      mBrain.fired("MVR6")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL7") +
      mBrain.fired("MVL8") +
      mBrain.fired("MVR7") +
      mBrain.fired("MVR8")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL9") +
      mBrain.fired("MVL10") +
      mBrain.fired("MVR9") +
      mBrain.fired("MVR10")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL11") +
      mBrain.fired("MVL12") +
      mBrain.fired("MVR11") +
      mBrain.fired("MVR12")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL13") +
      mBrain.fired("MVL14") +
      mBrain.fired("MVR13") +
      mBrain.fired("MVR14")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL15") +
      mBrain.fired("MVL16") +
      mBrain.fired("MVR15") +
      mBrain.fired("MVR16")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL17") +
      mBrain.fired("MVL18") +
      mBrain.fired("MVR17") +
      mBrain.fired("MVR18")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL19") +
      mBrain.fired("MVL20") +
      mBrain.fired("MVR19") +
      mBrain.fired("MVR20")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL21") +
      mBrain.fired("MVL22") +
      mBrain.fired("MVR21") +
      mBrain.fired("MVR22")
      ) / 4.0f;
    result[i++] = m;

    m = (
      mBrain.fired("MVL23") +
      mBrain.fired("MVR23") +
      mBrain.fired("MVR24")
      ) / 3.0f;
    result[i++] = m;
}

void CElegans::getMusclesActivites(
    unsigned left[SIDE_MUSCLE_NEURONS][2],
    unsigned right[SIDE_MUSCLE_NEURONS][2]
    ) const
{
    left[0][0] = mBrain.fired("MDL01");
    left[1][0] = mBrain.fired("MDL02");
    left[2][0] = mBrain.fired("MDL03");
    left[3][0] = mBrain.fired("MDL04");
    left[4][0] = mBrain.fired("MDL05");
    left[5][0] = mBrain.fired("MDL06");
    left[6][0] = mBrain.fired("MDL07");
    left[7][0] = mBrain.fired("MDL08");
    left[8][0] = mBrain.fired("MDL09");
    left[9][0] = mBrain.fired("MDL10");
    left[10][0] = mBrain.fired("MDL11");
    left[11][0] = mBrain.fired("MDL12");
    left[12][0] = mBrain.fired("MDL13");
    left[13][0] = mBrain.fired("MDL14");
    left[14][0] = mBrain.fired("MDL15");
    left[15][0] = mBrain.fired("MDL16");
    left[16][0] = mBrain.fired("MDL17");
    left[17][0] = mBrain.fired("MDL18");
    left[18][0] = mBrain.fired("MDL19");
    left[19][0] = mBrain.fired("MDL20");
    left[20][0] = mBrain.fired("MDL21");
    left[21][0] = mBrain.fired("MDL22");
    left[22][0] = mBrain.fired("MDL23");
    left[23][0] = mBrain.fired("MDL24");

    left[0][1] = mBrain.fired("MVL01");
    left[1][1] = mBrain.fired("MVL02");
    left[2][1] = mBrain.fired("MVL03");
    left[3][1] = mBrain.fired("MVL04");
    left[4][1] = mBrain.fired("MVL05");
    left[5][1] = mBrain.fired("MVL06");
    left[6][1] = mBrain.fired("MVL07");
    left[7][1] = mBrain.fired("MVL08");
    left[8][1] = mBrain.fired("MVL09");
    left[9][1] = mBrain.fired("MVL10");
    left[10][1] = mBrain.fired("MVL11");
    left[11][1] = mBrain.fired("MVL12");
    left[12][1] = mBrain.fired("MVL13");
    left[13][1] = mBrain.fired("MVL14");
    left[14][1] = mBrain.fired("MVL15");
    left[15][1] = mBrain.fired("MVL16");
    left[16][1] = mBrain.fired("MVL17");
    left[17][1] = mBrain.fired("MVL18");
    left[18][1] = mBrain.fired("MVL19");
    left[19][1] = mBrain.fired("MVL20");
    left[20][1] = mBrain.fired("MVL21");
    left[21][1] = mBrain.fired("MVL22");
    left[22][1] = mBrain.fired("MVL23");
    left[23][1] = 0;

    right[0][0] = mBrain.fired("MVR01");
    right[1][0] = mBrain.fired("MVR02");
    right[2][0] = mBrain.fired("MVR03");
    right[3][0] = mBrain.fired("MVR04");
    right[4][0] = mBrain.fired("MVR05");
    right[5][0] = mBrain.fired("MVR06");
    right[6][0] = mBrain.fired("MVR07");
    right[7][0] = mBrain.fired("MVR08");
    right[8][0] = mBrain.fired("MVR09");
    right[9][0] = mBrain.fired("MVR10");
    right[10][0] = mBrain.fired("MVR11");
    right[11][0] = mBrain.fired("MVR12");
    right[12][0] = mBrain.fired("MVR13");
    right[13][0] = mBrain.fired("MVR14");
    right[14][0] = mBrain.fired("MVR15");
    right[15][0] = mBrain.fired("MVR16");
    right[16][0] = mBrain.fired("MVR17");
    right[17][0] = mBrain.fired("MVR18");
    right[18][0] = mBrain.fired("MVR19");
    right[19][0] = mBrain.fired("MVR20");
    right[20][0] = mBrain.fired("MVR21");
    right[21][0] = mBrain.fired("MVR22");
    right[22][0] = mBrain.fired("MVR23");
    right[23][0] = mBrain.fired("MVR24");

    right[0][1] = mBrain.fired("MDR01");
    right[1][1] = mBrain.fired("MDR02");
    right[2][1] = mBrain.fired("MDR03");
    right[3][1] = mBrain.fired("MDR04");
    right[4][1] = mBrain.fired("MDR05");
    right[5][1] = mBrain.fired("MDR06");
    right[6][1] = mBrain.fired("MDR07");
    right[7][1] = mBrain.fired("MDR08");
    right[8][1] = mBrain.fired("MDR09");
    right[9][1] = mBrain.fired("MDR10");
    right[10][1] = mBrain.fired("MDR11");
    right[11][1] = mBrain.fired("MDR12");
    right[12][1] = mBrain.fired("MDR13");
    right[13][1] = mBrain.fired("MDR14");
    right[14][1] = mBrain.fired("MDR15");
    right[15][1] = mBrain.fired("MDR16");
    right[16][1] = mBrain.fired("MDR17");
    right[17][1] = mBrain.fired("MDR18");
    right[18][1] = mBrain.fired("MDR19");
    right[19][1] = mBrain.fired("MDR20");
    right[20][1] = mBrain.fired("MDR21");
    right[21][1] = mBrain.fired("MDR22");
    right[22][1] = mBrain.fired("MDR23");
    right[23][1] = mBrain.fired("MDR24");
}

} // namespace NSnu

