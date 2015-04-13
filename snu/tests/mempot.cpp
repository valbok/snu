/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

/**
 * Creates few files with membrane potentials for visualization.
 */

#include <snu/Neuron.hpp>
#include <iostream>
#include <cstdlib>
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
    std::ofstream fn1, fn2;
    fn1.open("neuron_src.csv");
    fn2.open("neuron_dst.csv");

    Neuron n1;
    Neuron n2;
    Neuron n3;
    Neuron n4;
    n1.connectTo(&n4, getRndWeight());
    n2.connectTo(&n4, getRndWeight());
    n3.connectTo(&n4, -getRndWeight());

    for (unsigned t = 0; t < 2000; ++t)
    {
        if (n1.tick(TIME_STEP))
            std::cout << "n1 spiked\n";

        if (n2.tick(TIME_STEP))
            std::cout << "n2 spiked\n";

        if (n3.tick(TIME_STEP))
            std::cout << "n3 spiked\n";

        if (n4.tick(TIME_STEP))
            std::cout << "n4 spiked\n";

        fn1 << t * TIME_STEP << "; " << n1.getMembraneValue() << "; " << std::endl;
        fn2 << t * TIME_STEP << "; " << n4.getMembraneValue() << "; " << std::endl;

        if (t % 100 == 0)
            n1.spike();
        if (t % 50 == 0) 
            n2.spike();
        if (t % 200 == 0) 
            n3.spike();
    }

    fn1.close();
    fn2.close();
}