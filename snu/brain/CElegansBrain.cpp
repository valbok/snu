/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "CElegansBrain.hpp"
#include "connectome.hpp"

namespace NSnu
{

CElegansBrain::CElegansBrain() throw()
{
    NConnectome::init(*this);
}

void CElegansBrain::connect(const std::string& from, const std::string& to, int count)
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

    const float maxSynapses = 21.0f;
    const float minWeight = 50.0f;
    float k = minWeight / maxSynapses;
    float weight = count * k + minWeight;
    mNeurons.at(from).connectTo(&mNeurons.at(to), isPositive ? weight : -weight);
}

unsigned CElegansBrain::getNeuronsCount() const
{
    return mNeurons.size();
}

void CElegansBrain::step(float h)
{
    for (auto& item: mNeurons)
    {
        item.second.step(h);
    }
}

bool CElegansBrain::spike(const std::string& name)
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

bool CElegansBrain::hasNeuron(const std::string& name) const
{
    return mNeurons.find(name) != mNeurons.end();
}

bool CElegansBrain::fired(const std::string& name) const
{
    bool result = false;
    auto item = mNeurons.find(name);
    if (item != mNeurons.end())
    {
        result = item->second.fired();
    }

    return result;
}

const Neuron& CElegansBrain::getNeuron(const std::string& name) const
{
    return mNeurons.at(name);
}

} // namespace NSnu

