/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

/**
 * Creates few files with membrane potentials for visualization.
 */

#include <snu/worm/CElegans.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace NSnu;

const float TIME_STEP = 0.5f;
const unsigned PERIOD = 1800;

class Worm : public CElegans
{
public:
    float getAverageVA()
    {
        return (
            mBrain.getNeuron("VA1").getMembraneValue() +
            mBrain.getNeuron("VA2").getMembraneValue() +
            mBrain.getNeuron("VA3").getMembraneValue() +
            mBrain.getNeuron("VA4").getMembraneValue() +
            mBrain.getNeuron("VA5").getMembraneValue() +
            mBrain.getNeuron("VA6").getMembraneValue() +
            mBrain.getNeuron("VA7").getMembraneValue() +
            mBrain.getNeuron("VA8").getMembraneValue() +
            mBrain.getNeuron("VA9").getMembraneValue() +
            mBrain.getNeuron("VA10").getMembraneValue() +
            mBrain.getNeuron("VA11").getMembraneValue() +
            mBrain.getNeuron("VA12").getMembraneValue()
            ) / 12.0f;
    }
};

int main()
{
    Worm worm;
    std::ofstream fn1, fn2, fn3, fn4;
    fn1.open("VA.csv");
    fn2.open("ventralMagnitudes.csv");
    fn3.open("dorsalMagnitudes.csv");
    fn4.open("ava.csv");

    float dorsalMagnitudes[MUSCLE_MAGNITUDES] = {0};
    float ventralMagnitudes[MUSCLE_MAGNITUDES] = {0};
    for (int t = 0; t < 10000000; ++t)
    {
        worm.teachFoodDirection(TIME_STEP, PERIOD);

        worm.getDorsalMagnitudes(dorsalMagnitudes);
        worm.getVentralMagnitudes(ventralMagnitudes);
        float dm = 0, vm = 0;
        for (int i = 0; i < 12; ++i)
        {
            dm += dorsalMagnitudes[i];
            vm += ventralMagnitudes[i];
        }
        dm = dm / MUSCLE_MAGNITUDES;
        vm =  vm / MUSCLE_MAGNITUDES; 
        fn1 << t << "; " << worm.getAverageVA() << "; " << std::endl;
        fn2 << t << "; " << vm << "; " << std::endl;
        fn3 << t << "; " << dm << "; " << std::endl;
        fn4 << t << "; " << worm.getFoodPeriodFrequency() << "; " << std::endl;
    }

    fn1.close();
    fn2.close();
    fn3.close();
    fn4.close();
}