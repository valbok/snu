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

void CElegans::connect(std::string from, std::string to, int count)
{
    if (mNeurons.find(from) == mNeurons.end())
    {
        mNeurons.insert(std::make_pair(from, Neuron()));
    }

    if (mNeurons.find(to) == mNeurons.end())
    {
        mNeurons.insert(std::make_pair(to, Neuron()));
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

} // namespace NSnu

