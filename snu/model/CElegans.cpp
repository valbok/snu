/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "CElegans.hpp"
#include <stdlib.h>

namespace NSnu
{

CElegans::CElegans(const ConnectomeParser::TConnectome& connectome) throw()
{
    for (unsigned i = 0; i < connectome.size(); ++i)
    {
        connect(
            std::get<0>(connectome[i]), 
            std::get<1>(connectome[i]), 
            std::get<2>(connectome[i])
        );
    }
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

