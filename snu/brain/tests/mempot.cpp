/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

/**
 * Creates few files with membrane potentials for visualization.
 */

#include <snu/brain/Neuron.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace NSnu;

const float TIME_STEP = 0.5f;

int main()
{
    std::ofstream fn1, fn2, fn3, fn4;
    fn1.open("neuron1.csv");
    fn2.open("neuron2.csv");
    fn3.open("neuron3.csv");
    fn4.open("neuron4.csv");

    Neuron n1;
    Neuron n2;
    Neuron n3;
    Neuron n4;
    n1.connectTo(&n2, false);
    n1.connectTo(&n3, true);
    n1.connectTo(&n4, true);
    n2.connectTo(&n1, false);
    n2.connectTo(&n3, true);
    n2.connectTo(&n4, true);
    n3.connectTo(&n4, false);

    for (unsigned t = 0; t < 2000; ++t)
    {
        fn1 << t * TIME_STEP << "; " << n1.getMembraneValue() << "; " << std::endl;
        fn2 << t * TIME_STEP << "; " << n2.getMembraneValue() << "; " << std::endl;
        fn3 << t * TIME_STEP << "; " << n3.getMembraneValue() << "; " << std::endl;
        fn4 << t * TIME_STEP << "; " << n4.getMembraneValue() << "; " << std::endl;

        if (n1.tick(TIME_STEP)) std::cout << "n1 spiked\n";
        if (n2.tick(TIME_STEP)) {std::cout << "n2 spiked\n";}
        if (n3.tick(TIME_STEP)) {std::cout << "n3 spiked\n";}
        if (n4.tick(TIME_STEP)) std::cout << "n4 spiked\n";

        if (t % 100 == 0)
            n1.spike();
        if (t % 500 == 0)
            n2.spike();
    }

    fn1.close();
    fn2.close();
    fn3.close();
    fn4.close();
}