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
    if (infraredSensor0.getDist() < 2 || infraredSensor7.getDist() < 2)
    {
        mBrain.spike("ASHL");
        mBrain.spike("ASHR");
        mBrain.spike("FLPL");
        mBrain.spike("FLPR");
        mBrain.spike("OLQDL");
        mBrain.spike("OLQDR");
        mBrain.spike("OLQVR");
        mBrain.spike("OLQVL");
    }
    else
    {

        /*mBrain.spike("FLPR");
        mBrain.spike("FLPL");
        mBrain.spike("ASHL");
        mBrain.spike("ASHR");
        mBrain.spike("IL1VL");
        mBrain.spike("IL1VR");
        mBrain.spike("OLQDL");
        mBrain.spike("OLQDR");
        mBrain.spike("OLQVR");
        mBrain.spike("OLQVL");*/

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
        /*
// begin
        mBrain.spike("IL1L");
        mBrain.spike("IL1R");
        mBrain.spike("IL1DL");
        mBrain.spike("IL1DR");

        mBrain.spike("OLLL");
        mBrain.spike("OLLR");
        mBrain.spike("PDEL");
        mBrain.spike("PDER");
        mBrain.spike("PHCL");
        mBrain.spike("PHCR");
        mBrain.spike("PLML");
        mBrain.spike("PLMR");
        mBrain.spike("PLNR");
        mBrain.spike("PQR");

        mBrain.spike("ADEL");
        mBrain.spike("ADER");
        mBrain.spike("ALML");
        mBrain.spike("ALMR");
        mBrain.spike("ALNL");
        mBrain.spike("ALNR");
        mBrain.spike("AVM");
        mBrain.spike("CEPDL");
        mBrain.spike("CEPDR");
        mBrain.spike("CEPVL");
        mBrain.spike("CEPVR");

        mBrain.spike("ADFL");
        mBrain.spike("ADFR");
        mBrain.spike("ASGR");
        mBrain.spike("ASGL");
        mBrain.spike("ASIL");
        mBrain.spike("ASIR");
        mBrain.spike("ASJR");
        mBrain.spike("ASJL");
*/
    }

    mBrain.tick(h);
    drive();
}
static int t = 0;
void SnuPuck::driveNeuron(const std::string& name, double& speed, bool log, bool positive)
{
    if (log)
    {
        /*std::ofstream fn;
        std::string f = "neurons/" + name + ".csv";
        fn.open(f.c_str(), std::ios::out | std::ios::app);
        fn << t << "; " << mBrain.getNeuron(name).getMembraneValue() << "; " << std::endl;
        fn.close();*/
        
    }
    int v = mBrain.fired(name);
    if (!positive && v > 0)
    {
        //v = -v;
    }

    speed += v;
    
    if (mBrain.fired(name))
    {
        //std::cout << "left = " << leftSpeed << std::endl;
        //std::cout << "right = " << rightSpeed << std::endl << std::endl;

    }
    //else
        //--speed;

}

void SnuPuck::drive()
{
    int left[24][2] = {0};
    int right[24][2] = {0};
    int diff[24] = {0};

    left[0][0] = mBrain.fired("MDL01");
    left[1][0] = mBrain.fired("MDL02");
    left[2][0] = mBrain.fired("MDL03");
    left[3][0] = mBrain.fired("MDL04");
    left[4][0] = mBrain.fired("MDL05");
    left[5][0] = mBrain.fired("MDL06");
    left[6][0] = mBrain.fired("MDL07");
    left[7][0] = mBrain.fired("MDL08");
    left[8][0] = mBrain.fired("MDL09");
    left[9][0] = mBrain.fired("MDL10");
    left[10][0] = mBrain.fired("MDL11");
    left[11][0] = mBrain.fired("MDL12");
    left[12][0] = mBrain.fired("MDL13");
    left[13][0] = mBrain.fired("MDL14");
    left[14][0] = mBrain.fired("MDL15");
    left[15][0] = mBrain.fired("MDL16");
    left[16][0] = mBrain.fired("MDL17");
    left[17][0] = mBrain.fired("MDL18");
    left[18][0] = mBrain.fired("MDL19");
    left[19][0] = mBrain.fired("MDL20");
    left[20][0] = mBrain.fired("MDL21");
    left[21][0] = mBrain.fired("MDL22");
    left[22][0] = mBrain.fired("MDL23");
    left[23][0] = mBrain.fired("MDL24");

    left[0][1] = mBrain.fired("MVL01");
    left[1][1] = mBrain.fired("MVL02");
    left[2][1] = mBrain.fired("MVL03");
    left[3][1] = mBrain.fired("MVL04");
    left[4][1] = mBrain.fired("MVL05");
    left[5][1] = mBrain.fired("MVL06");
    left[6][1] = mBrain.fired("MVL07");
    left[7][1] = mBrain.fired("MVL08");
    left[8][1] = mBrain.fired("MVL09");
    left[9][1] = mBrain.fired("MVL10");
    left[10][1] = mBrain.fired("MVL11");
    left[11][1] = mBrain.fired("MVL12");
    left[12][1] = mBrain.fired("MVL13");
    left[13][1] = mBrain.fired("MVL14");
    left[14][1] = mBrain.fired("MVL15");
    left[15][1] = mBrain.fired("MVL16");
    left[16][1] = mBrain.fired("MVL17");
    left[17][1] = mBrain.fired("MVL18");
    left[18][1] = mBrain.fired("MVL19");
    left[19][1] = mBrain.fired("MVL20");
    left[20][1] = mBrain.fired("MVL21");
    left[21][1] = mBrain.fired("MVL22");
    left[22][1] = mBrain.fired("MVL23");
    left[23][1] = 0;

    right[0][0] = mBrain.fired("MDR01");
    right[1][0] = mBrain.fired("MDR02");
    right[2][0] = mBrain.fired("MDR03");
    right[3][0] = mBrain.fired("MDR04");
    right[4][0] = mBrain.fired("MDR05");
    right[5][0] = mBrain.fired("MDR06");
    right[6][0] = mBrain.fired("MDR07");
    right[7][0] = mBrain.fired("MDR08");
    right[8][0] = mBrain.fired("MDR09");
    right[9][0] = mBrain.fired("MDR10");
    right[10][0] = mBrain.fired("MDR11");
    right[11][0] = mBrain.fired("MDR12");
    right[12][0] = mBrain.fired("MDR13");
    right[13][0] = mBrain.fired("MDR14");
    right[14][0] = mBrain.fired("MDR15");
    right[15][0] = mBrain.fired("MDR16");
    right[16][0] = mBrain.fired("MDR17");
    right[17][0] = mBrain.fired("MDR18");
    right[18][0] = mBrain.fired("MDR19");
    right[19][0] = mBrain.fired("MDR20");
    right[20][0] = mBrain.fired("MDR21");
    right[21][0] = mBrain.fired("MDR22");
    right[22][0] = mBrain.fired("MDR23");
    right[23][0] = mBrain.fired("MDR24");

    right[0][1] = mBrain.fired("MVR01");
    right[1][1] = mBrain.fired("MVR02");
    right[2][1] = mBrain.fired("MVR03");
    right[3][1] = mBrain.fired("MVR04");
    right[4][1] = mBrain.fired("MVR05");
    right[5][1] = mBrain.fired("MVR06");
    right[6][1] = mBrain.fired("MVR07");
    right[7][1] = mBrain.fired("MVR08");
    right[8][1] = mBrain.fired("MVR09");
    right[9][1] = mBrain.fired("MVR10");
    right[10][1] = mBrain.fired("MVR11");
    right[11][1] = mBrain.fired("MVR12");
    right[12][1] = mBrain.fired("MVR13");
    right[13][1] = mBrain.fired("MVR14");
    right[14][1] = mBrain.fired("MVR15");
    right[15][1] = mBrain.fired("MVR16");
    right[16][1] = mBrain.fired("MVR17");
    right[17][1] = mBrain.fired("MVR18");
    right[18][1] = mBrain.fired("MVR19");
    right[19][1] = mBrain.fired("MVR20");
    right[20][1] = mBrain.fired("MVR21");
    right[21][1] = mBrain.fired("MVR22");
    right[22][1] = mBrain.fired("MVR23");
    right[23][1] = mBrain.fired("MVR23");

    for (int i = 0; i < 24; ++i)
    {
        diff[i] = left[i][0] + left[i][1] - right[i][0] - right[i][1];
        if (i > 0)
        {
            if (diff[i] < 0)
                std::cout << " ";
            else
                std::cout << "  ";
        }
        else
        {
            if (diff[i] >= 0)
                std::cout << " ";

        }
        std::cout << diff[i];
    }
    std::cout << std::endl;

    int f = 0, t = 0;
    for (int i = 0; i < 24; ++i)
    {
        if (i < 12)
        {
            f += diff[i];
        }
        else
        {
            t += diff[i];
        }
    }

    bool forward = f >= t;
    int ls = 0;
    int rs = 0;
    for (int i = 0; i < 24; ++i)
    {
        //if (forward)
        {
            if (diff[i] < 0)
            {
                ++ls;
            }
            if (diff[i] > 0)
            {
                ++rs;
            }
        }
        /*else
        {
            if (diff[i] < 0)
            {
                --ls;
            }
            if (diff[i] > 0)
            {
                --rs;
            }
        }*/

    }
    leftSpeed = ls;
    rightSpeed = rs;

        //std::cout << "left = " << leftSpeed << std::endl;
        //std::cout << "right = " << rightSpeed << std::endl << std::endl;


   /* ++t;
leftSpeed = rightSpeed = 0;
driveNeuron("MDL01", leftSpeed, true, false);
driveNeuron("MDL02", leftSpeed, true, false);
driveNeuron("MDL03", leftSpeed, true, false);
driveNeuron("MDL04", leftSpeed, true, false);
driveNeuron("MDL05", leftSpeed, true, false);
driveNeuron("MDL06", leftSpeed, true, false);

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
    driveNeuron("MDL20", leftSpeed, true, false);
    driveNeuron("MDL21", leftSpeed, true, false);
    driveNeuron("MDL22", leftSpeed, true, false);
    driveNeuron("MDL23", leftSpeed, true, false);
    driveNeuron("MDL24", leftSpeed, true, false);

driveNeuron("MANAL", leftSpeed, true, false);
driveNeuron("MANAL", rightSpeed, true, false);
driveNeuron("MVULVA", leftSpeed, true, false);
driveNeuron("MVULVA", rightSpeed, true, false);

driveNeuron("MVL01", leftSpeed, true, false);
driveNeuron("MVL02", leftSpeed, true, false);
driveNeuron("MVL03", leftSpeed, true, false);
driveNeuron("MVL04", leftSpeed, true, false);
driveNeuron("MVL05", leftSpeed, true, false);
driveNeuron("MVL06", leftSpeed, true, false);

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
    driveNeuron("MVL20", leftSpeed, true, false);
    driveNeuron("MVL21", leftSpeed, true, false);
    driveNeuron("MVL22", leftSpeed, true, false);
    driveNeuron("MVL23", leftSpeed, true, false);


driveNeuron("MDR01", rightSpeed, true, false);
driveNeuron("MDR02", rightSpeed, true, false);
driveNeuron("MDR03", rightSpeed, true, false);
driveNeuron("MDR04", rightSpeed, true, false);
driveNeuron("MDR05", rightSpeed, true, false);
driveNeuron("MDR06", rightSpeed, true, false);

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
    driveNeuron("MDR20", rightSpeed, true, false);
    driveNeuron("MDR21", rightSpeed, true, false);
    driveNeuron("MDR22", rightSpeed, true, false);
    driveNeuron("MDR23", rightSpeed, true, false);
    driveNeuron("MDR24", rightSpeed, true, false);

driveNeuron("MVR01", rightSpeed, true, false);
driveNeuron("MVR02", rightSpeed, true, false);
driveNeuron("MVR03", rightSpeed, true, false);
driveNeuron("MVR04", rightSpeed, true, false);
driveNeuron("MVR05", rightSpeed, true, false);
driveNeuron("MVR06", rightSpeed, true, false);

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
    driveNeuron("MVR20", rightSpeed, true, false);
    driveNeuron("MVR21", rightSpeed, true, false);
    driveNeuron("MVR22", rightSpeed, true, false);
    driveNeuron("MVR23", rightSpeed, true, false);
    driveNeuron("MVR24", rightSpeed, true, false);*/
}
} // namespace NSnu
