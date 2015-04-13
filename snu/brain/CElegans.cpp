/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "CElegans.hpp"
#include "connectome.hpp"
#include <stdlib.h>

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

    std::string fromAlias = from.substr(0, 3);
    if (mNeuronAliases.find(fromAlias) == mNeuronAliases.end())
    {
        mNeuronAliases.insert(std::make_pair(fromAlias, std::vector<std::string>()));
    }

    {
        auto& fromAliases = mNeuronAliases.at(fromAlias);
        bool found = false;
        for (unsigned i = 0; i < fromAliases.size(); ++i)
        {
            if (fromAliases[i] == from)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            fromAliases.push_back(from);
        }
    }

    std::string toAlias = to.substr(0, 3);
    if (mNeuronAliases.find(toAlias) == mNeuronAliases.end())
    {
        mNeuronAliases.insert(std::make_pair(toAlias, std::vector<std::string>()));
    }

    {
        auto& toAliases = mNeuronAliases.at(toAlias);
        bool found = false;
        for (unsigned i = 0; i < toAliases.size(); ++i)
        {
            if (toAliases[i] == to)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            toAliases.push_back(to);
        }
    }

    bool isPositive = count >= 0;
    count = abs(count);

    while(count-- > 0)
    {
        mNeurons.at(from).connectTo(&mNeurons.at(to), isPositive);
    }
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

bool CElegans::getNeuronNames(const std::string& alias, std::vector<std::string>& result) const
{
    bool found = false;
    auto item = mNeuronAliases.find(alias);
    if (item != mNeuronAliases.end())
    {
        for (unsigned i = 0; i < item->second.size(); ++i)
        {
            result.push_back(item->second[i]);
        }

        found = true;
    }

    return found;
}

bool CElegans::spiked(const std::string& name) const
{
    bool result = false;
    auto item = mNeurons.find(name);
    if (item != mNeurons.end())
    {
        result = item->second.spiked();
    }

    return result;
}

} // namespace NSnu

