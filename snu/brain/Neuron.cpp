/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "Neuron.hpp"
#include <cmath>
#include <cstdlib>

namespace NSnu
{

/**
 * Default value for membrane potential.
 */
const float DEFAULT_MEMBRANE = -60.0f;

Neuron::Neuron(TCallback callback) throw()
    : mExtI(0)
    , mSynI(0)
    , mPrevMembrane(DEFAULT_MEMBRANE)
    , mCurMembrane(DEFAULT_MEMBRANE)
    , mPrevAdditional(0)
    , mCurAdditional(0)
    , mForceSpike(false)
    , mSpiked(false)
    , mCallback(callback)
{
    // Max external current that applied to the neuron, pA
    const float extIMax = 40.0f;
    mExtI = (rand() % (int) (extIMax * 10)) / 10.0f;
}

void Neuron::connectTo(Neuron* target, float weight)
{
    const float prevSynCurrent = 0;
    const float curSynCurrent = 0;
    mTargets.push_back({target, weight, prevSynCurrent, curSynCurrent});
}

static inline float getRndWeight()
{
    const float maxWeight = 100.0f;
    const float minWeight = 50.0f;

    return (rand() % ((int)(maxWeight - minWeight) * 10)) / 10.0f + minWeight;
}

void Neuron::connectTo(Neuron* target, bool isPositive)
{
    float w = getRndWeight();
    connectTo(target, isPositive ? w : -w);
}

float Neuron::getPrevMembraneValue() const
{
    // Some configurable params of Izhikevich neuron model.
    const float k = 0.5f;
    const float vt = -45.0f;
    // Capacitance of neuron, dimension pF
    const float cm = 50.0f;

    return (k * (mPrevMembrane - DEFAULT_MEMBRANE) * (mPrevMembrane - vt) -
        mPrevAdditional + mExtI + mSynI) / cm;
}

float Neuron::getPrevAdditionalValue() const
{
    // Some configurable params of Izhikevich neuron model.
    const float a = 0.02f;
    const float b = 0.5f;

    return a * (b * (mPrevMembrane - DEFAULT_MEMBRANE) - mPrevAdditional);
}

bool Neuron::tick(float h)
{
    const float spikeThreshold = 35.0f;
    mSpiked = false;

    // If need to force the spike.
    // But this spike will be handled in next step
    // due to need to have an ability to show the spike on a graph if any.
    if (mForceSpike)
    {
        mCurMembrane = spikeThreshold + 0.1f;
        mCurAdditional = 100.0f;
    }
    else
    {
        mCurMembrane = mPrevMembrane + h * getPrevMembraneValue();
        mCurAdditional = mPrevAdditional + h * getPrevAdditionalValue();

        mSpiked = mPrevMembrane > spikeThreshold;
    }

    // Spike occured.
    if (mSpiked)
    {
        const float resetMembraneTo = -40.0f;
        mCurMembrane = resetMembraneTo;
        mCurAdditional = mPrevAdditional + 100.0f;
        if (mCallback != 0)
        {
            mCallback(this);
        }
    }

    mSynI = 0.0f;
    mPrevMembrane = mCurMembrane;
    mPrevAdditional = mCurAdditional;
    mForceSpike = false;

    // Defines how the synaptic current should fade down.
    const float expireCoeff = exp(-h / 4.0f);
    for (unsigned i = 0; i < mTargets.size(); ++i)
    {
        STarget& target = mTargets.at(i);

        target.mCurSynI = mSpiked ? 1.0f : (target.mPrevSynI * expireCoeff);
        target.mTarget->teach(target.mCurSynI * target.mWeight);
        target.mPrevSynI = target.mCurSynI;
    }

    return mSpiked;
}

void Neuron::spike()
{
    mForceSpike = true;
}

void Neuron::teach(float value)
{
    mSynI += value;
}

float Neuron::getMembraneValue() const
{
    return mCurMembrane;
}

bool Neuron::spiked() const
{
    return mSpiked;
}

} // namespace NSnu
