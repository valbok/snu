/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "CElegans.hpp"
#include "connectome.hpp"

namespace NSnu
{

CElegans::CElegans() throw()
{
    NConnectome::init(*this);
}

void CElegans::connect(const std::string& from, const std::string& to, int count)
{
    if (mNeurons.find(from) == mNeurons.end())
    {
        mNeurons.insert(std::make_pair(from, Neuron()));
    }

    if (mNeurons.find(to) == mNeurons.end())
    {
        mNeurons.insert(std::make_pair(to, Neuron()));
    }

    bool isPositive = true;
    if (count < 0)
    {
        count = -count;
        isPositive = false;
    }

    float maxSynapses = 21.0f;
    float minWeight = 50.0f;
    float k = minWeight / maxSynapses;
    float weight = count * k + minWeight;
    mNeurons.at(from).connectTo(&mNeurons.at(to), isPositive ? weight : -weight);

    /*bool isPositive = count >= 0;
    count = abs(count);

    while(count-- > 0)
    {
        mNeurons.at(from).connectTo(&mNeurons.at(to), isPositive);
    }*/
}

unsigned CElegans::getNeuronsCount() const
{
    return mNeurons.size();
}

void CElegans::tick(float h)
{
    for (auto& item: mNeurons)
    {
        item.second.tick(h);
    }
}

bool CElegans::spike(const std::string& name)
{
    bool result = false;
    auto item = mNeurons.find(name);
    if (item != mNeurons.end())
    {
        item->second.spike();
        result = true;
    }

    return result;
}

bool CElegans::hasNeuron(const std::string& name) const
{
    return mNeurons.find(name) != mNeurons.end();
}

bool CElegans::fired(const std::string& name) const
{
    bool result = false;
    auto item = mNeurons.find(name);
    if (item != mNeurons.end())
    {
        result = item->second.fired();
    }

    return result;
}

const Neuron& CElegans::getNeuron(const std::string& name) const
{
    return mNeurons.at(name);
}

} // namespace NSnu

