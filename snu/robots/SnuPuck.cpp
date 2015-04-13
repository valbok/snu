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
#include <fstream>
namespace NSnu
{

void SnuPuck::tick(float h)
{
    if (infraredSensor0.getDist() < 5 || infraredSensor7.getDist() < 5)
    {
        //std::cout << "sens0 = " << infraredSensor0.getDist() << "; sens7 = " << infraredSensor7.getDist() << std::endl;
        std::cout << "TOUCH\n";
        leftSpeed = rightSpeed = 0;
        mBrain.spike("FLPR");
        mBrain.spike("FLPL");
        mBrain.spike("ASHL");
        mBrain.spike("ASHR");
        mBrain.spike("IL1VL");
        mBrain.spike("IL1VR");
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
    }

    mBrain.tick(h);
    drive();
}

void SnuPuck::driveNeuron(const std::string& name, double& speed, bool log)
{
    if (log)
    {
        static int t = 0;
        std::ofstream fn;
        std::string f = "neurons/" + name + ".csv";
        fn.open(f.c_str(), std::ios::out | std::ios::app);
        fn << t * 2 << "; " << mBrain.getNeuron(name).getMembraneValue() << "; " << std::endl;
        fn.close();
        ++t;
    }
    speed += mBrain.spiked(name);
}

void SnuPuck::drive()
{
    driveNeuron("MDL07", leftSpeed);
    driveNeuron("MDL08", leftSpeed);
    driveNeuron("MDL09", leftSpeed);
    driveNeuron("MDL10", leftSpeed);
    driveNeuron("MDL11", leftSpeed);
    driveNeuron("MDL12", leftSpeed);
    driveNeuron("MDL13", leftSpeed);
    driveNeuron("MDL14", leftSpeed);
    driveNeuron("MDL15", leftSpeed);
    driveNeuron("MDL16", leftSpeed);
    driveNeuron("MDL17", leftSpeed);
    driveNeuron("MDL18", leftSpeed);
    driveNeuron("MDL19", leftSpeed);
    driveNeuron("MDL20", leftSpeed);
    driveNeuron("MDL21", leftSpeed);
    driveNeuron("MDL22", leftSpeed);
    driveNeuron("MDL23", leftSpeed);

    driveNeuron("MVL07", leftSpeed);
    driveNeuron("MVL08", leftSpeed);
    driveNeuron("MVL09", leftSpeed);
    driveNeuron("MVL10", leftSpeed);
    driveNeuron("MVL11", leftSpeed);
    driveNeuron("MVL12", leftSpeed);
    driveNeuron("MVL13", leftSpeed);
    driveNeuron("MVL14", leftSpeed);
    driveNeuron("MVL15", leftSpeed);
    driveNeuron("MVL16", leftSpeed);
    driveNeuron("MVL17", leftSpeed);
    driveNeuron("MVL18", leftSpeed);
    driveNeuron("MVL19", leftSpeed);
    driveNeuron("MVL20", leftSpeed);
    driveNeuron("MVL21", leftSpeed);
    driveNeuron("MVL22", leftSpeed);
    driveNeuron("MVL23", leftSpeed);

    driveNeuron("MDR07", rightSpeed);
    driveNeuron("MDR08", rightSpeed);
    driveNeuron("MDR09", rightSpeed);
    driveNeuron("MDR10", rightSpeed);
    driveNeuron("MDR11", rightSpeed);
    driveNeuron("MDR12", rightSpeed);
    driveNeuron("MDR13", rightSpeed);
    driveNeuron("MDR14", rightSpeed);
    driveNeuron("MDR15", rightSpeed);
    driveNeuron("MDR16", rightSpeed);
    driveNeuron("MDR17", rightSpeed);
    driveNeuron("MDR18", rightSpeed);
    driveNeuron("MDR19", rightSpeed);
    driveNeuron("MDR20", rightSpeed);
    driveNeuron("MDR21", rightSpeed);
    driveNeuron("MDR22", rightSpeed);
    driveNeuron("MDR23", rightSpeed);

    driveNeuron("MVR07", rightSpeed);
    driveNeuron("MVR08", rightSpeed);
    driveNeuron("MVR09", rightSpeed);
    driveNeuron("MVR10", rightSpeed);
    driveNeuron("MVR11", rightSpeed);
    driveNeuron("MVR12", rightSpeed);
    driveNeuron("MVR13", rightSpeed);
    driveNeuron("MVR14", rightSpeed);
    driveNeuron("MVR15", rightSpeed);
    driveNeuron("MVR16", rightSpeed);
    driveNeuron("MVR17", rightSpeed);
    driveNeuron("MVR18", rightSpeed);
    driveNeuron("MVR19", rightSpeed);
    driveNeuron("MVR20", rightSpeed);
    driveNeuron("MVR21", rightSpeed);
    driveNeuron("MVR22", rightSpeed);
    driveNeuron("MVR23", rightSpeed);
}
} // namespace NSnu
