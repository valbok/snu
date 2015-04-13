/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/brain/Neuron.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace NSnu;

const float TIME_STEP = .5f;

float getRndWeight()
{
    const float maxWeight = 100.0f;
    const float minWeight = 50.0f;

    return (rand() % ((int)(maxWeight - minWeight) * 10)) / 10.0f + minWeight;
}

int main()
{
    const int neuronsCount = 125;
    const int connsCount  = neuronsCount * neuronsCount * 0.1f;

    std::vector<Neuron> neurons(neuronsCount);

    for (int i = 0; i < connsCount; ++i)
    {
        int pre = rand() % neuronsCount;
        int post = rand() % neuronsCount;
        float w = getRndWeight();
        if (pre >= 100)
        {
            w = -w;
        }

        neurons[pre].connectTo(&neurons[post], w);
    }

    std::ofstream osc;
    osc.open("oscill.csv");

    for (unsigned t = 0; t < (1000/0.5f); ++t)
    {
        float mean = 0.0f;
        for (unsigned i = 0; i < neurons.size(); ++i)
        {
            neurons[i].tick(TIME_STEP);
            mean += neurons[i].getMembraneValue();
        }
        mean /= neurons.size();

        osc << t * TIME_STEP << "; " << mean << "; " << std::endl;
    }

    osc.close();
}
