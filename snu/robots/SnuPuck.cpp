/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "SnuPuck.hpp"

#include <vector>
#include <string>
#include <iostream>

namespace NSnu
{

void SnuPuck::tick(float h)
{
    if (infraredSensor0.getDist() < 5 || infraredSensor7.getDist() < 5)
    {
        std::cout << "sens0 = " << infraredSensor0.getDist() << "; sens7 = " << infraredSensor7.getDist() << std::endl;
        std::cout << "TOUCH\n";
        mBrain.spike("ASHL");
        mBrain.spike("ASHR");
        mBrain.spike("FLPR");
        mBrain.spike("FLPL");
        mBrain.spike("OLQDL");
        mBrain.spike("OLQDR");
        mBrain.spike("OLQVR");
        mBrain.spike("OLQVL");
    }
    else
    {

        mBrain.spike("ADFL");
        mBrain.spike("ADFR");
        mBrain.spike("ASGR");
        mBrain.spike("ASGL");
        mBrain.spike("ASIL");
        mBrain.spike("ASIR");
        mBrain.spike("ASJR");
        mBrain.spike("ASJL");
        mBrain.spike("AWCL");
        mBrain.spike("AWCR");
        mBrain.spike("AWAR");
        mBrain.spike("AWAL");
    }

    mBrain.tick(h);
    drive();
}

void SnuPuck::drive()
{
    std::vector<std::string> leftNeurons;
    std::vector<std::string> rightNeurons;
    mBrain.getNeuronNames("MVL", leftNeurons);
    mBrain.getNeuronNames("MDL", leftNeurons);
    mBrain.getNeuronNames("MVR", rightNeurons);
    mBrain.getNeuronNames("MDR", rightNeurons);

    leftSpeed = 0;
    for (unsigned i = 0; i < leftNeurons.size(); ++i)
    {
        if (mBrain.spiked(leftNeurons[i]))
        {
            ++leftSpeed;
        }
    }
    rightSpeed = 0;
    for (unsigned i = 0; i < rightNeurons.size(); ++i)
    {
        if (mBrain.spiked(rightNeurons[i]))
        {
            ++rightSpeed;
        }
    }
    leftSpeed *= 10;
    rightSpeed *= 10;
if (leftSpeed > 0 || rightSpeed > 0) {
std::cout << "leftSpeed = " << leftSpeed << std::endl;
std::cout << "rightSpeed = " << rightSpeed << std::endl;

}
}

} // namespace NSnu
