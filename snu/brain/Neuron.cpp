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

/**
 * Threshold of membrane potential to make a spike.
 */
const float SPIKE_THRESHOLD = 35.0f;

Neuron::Neuron(float extI) throw()
    : mExtI(extI)
    , mSynI(0)
    , mPrevMembrane(DEFAULT_MEMBRANE)
    , mCurMembrane(DEFAULT_MEMBRANE)
    , mPrevAdditional(0)
    , mCurAdditional(0)
    , mFired(false)
    , mPrevFired(false)
{
}

void Neuron::connectTo(Neuron* target, float weight)
{
    const float teachedWeight = 0;
    const float prevSynI = 0;
    const float curSynI = 0;
    mAxones.push_back({target, weight, teachedWeight, prevSynI, curSynI});

    target->connectFrom(this, mAxones.size() - 1);
}

void Neuron::connectFrom(Neuron* source, unsigned axonId)
{
    mDendrites.push_back({source, axonId});
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
    // Some magic configurable params of Izhikevich neuron model.
    const float k = 0.5f;
    const float vt = -45.0f;
    // Capacitance of neuron, dimension pF.
    const float cm = 50.0f;

    return (k * (mPrevMembrane - DEFAULT_MEMBRANE) * (mPrevMembrane - vt) -
        mPrevAdditional + mExtI + mSynI) / cm;
}

float Neuron::getPrevAdditionalValue() const
{
    // Some magic configurable params of Izhikevich neuron model.
    const float a = 0.02f;
    const float b = 0.5f;

    return a * (b * (mPrevMembrane - DEFAULT_MEMBRANE) - mPrevAdditional);
}

bool Neuron::step(float h)
{
    mPrevFired = mFired;
    mFired = mPrevMembrane > SPIKE_THRESHOLD;
    if (mFired)
    {
        const float resetMembraneTo = -40.0f;
        mCurMembrane = resetMembraneTo;
        mCurAdditional = mPrevAdditional + 100.0f;
    }
    else
    {
        mCurMembrane = mPrevMembrane + h * getPrevMembraneValue();
        mCurAdditional = mPrevAdditional + h * getPrevAdditionalValue();
    }

    // Only if spike occured need to teach connections.
    if (mFired)
    {
        for (unsigned i = 0; i < mDendrites.size(); ++i)
        {
            mDendrites[i].source->teachSynWeight(mDendrites[i].axonId, mFired, mPrevFired);
        }
    }

    // Defines how the synaptic current should fade down.
    const float expireSpeed = 4.0f;
    const float expireFactor = exp(-h / expireSpeed);
    for (unsigned i = 0; i < mAxones.size(); ++i)
    {
        SAxon& target = mAxones.at(i);

        target.curSynI = mFired ? 1.0f : (target.prevSynI * expireFactor);
        target.target->teachSynI(target.curSynI * target.getWeight());
        target.prevSynI = target.curSynI;
    }

    mSynI = 0.0f;
    mPrevMembrane = mCurMembrane;
    mPrevAdditional = mCurAdditional;

    return mFired;
}

void Neuron::spike()
{
    mPrevMembrane = SPIKE_THRESHOLD + 0.1f;
}

void Neuron::teachSynI(float value)
{
    mSynI += value;
}

float Neuron::getSynI() const
{
    return mSynI;
}

bool Neuron::teachSynWeight(unsigned axonId, unsigned curTarget, unsigned prevTarget)
{
    // No need to teach connection if no spike occured on current neuron.
    if (axonId >= mAxones.size() || mFired == false || curTarget == 0)
    {
        return false;
    }

    const float teachSpeed = 0.1;
    float value = teachSpeed * (mFired - mPrevFired) * (curTarget - prevTarget);

    SAxon& target = mAxones.at(axonId);
    target.teachedWeight += value;

    return true;
}

float Neuron::getMembraneValue() const
{
    return mPrevMembrane;
}

bool Neuron::fired() const
{
    return mFired;
}

bool Neuron::prevFired() const
{
    return mPrevFired;
}

const TAxones& Neuron::getAxons() const
{
    return mAxones;
}

const TDendrites& Neuron::getDendrites() const
{
    return mDendrites;
}

float SAxon::getWeight() const
{
    const float expireSpeed = 50;
    float result = baseWeight;
    if (teachedWeight != 0)
    {
        float value = exp(-teachedWeight / expireSpeed);
        if (baseWeight < 0)
        {
            result -= value;
        }
        else
        {
            result += value;
        }
    }

    return result;
}

} // namespace NSnu

