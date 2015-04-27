/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/brain/Neuron.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

using namespace NSnu;
using namespace std;

const float TIME_STEP = 0.5f;
const float EXT_I = 30.0f;

TEST(Neuron, testConnectTo)
{
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);

    n1.connectTo(&n2, 50.0f);

    EXPECT_EQ(1, n1.getAxons().size());
    EXPECT_EQ(0, n1.getDendrites().size());
    EXPECT_EQ(1, n2.getDendrites().size());
    EXPECT_EQ(0, n2.getAxons().size());
    EXPECT_EQ(&n2, n1.getAxons()[0].target);
    EXPECT_EQ(50.0f, n1.getAxons()[0].weight);
    EXPECT_EQ(0.0f, n1.getAxons()[0].prevSynI);
    EXPECT_EQ(0.0f, n1.getAxons()[0].curSynI);
    EXPECT_EQ(&n1, n2.getDendrites()[0].source);
    EXPECT_EQ(0, n2.getDendrites()[0].axonId);
}

TEST(Neuron, testSpike)
{
    Neuron n1(EXT_I);

    n1.spike();
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() > 0);

    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() < 0);

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() < 0);

    n1.spike();
    EXPECT_TRUE(n1.getMembraneValue() > 0);

    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() < 0);
}

TEST(Neuron, testTeachSynI)
{
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);

    const float weight = 100.0f;
    n1.connectTo(&n2, weight);
    n1.spike();

    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_EQ(1.0f, n1.getAxons()[0].curSynI);
    EXPECT_EQ(weight, n2.getSynI());

    n2.step(TIME_STEP);
    EXPECT_EQ(1.0f, n1.getAxons()[0].curSynI);
    EXPECT_EQ(1.0f, n1.getAxons()[0].prevSynI);
    EXPECT_EQ(0.0f, n2.getSynI());

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(1.0f > n1.getAxons()[0].curSynI);
    EXPECT_TRUE(weight > n2.getSynI());
}

TEST(Neuron, testTeachSynWeight_00_00)
{
    const float weight = 100.0f;

    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_FALSE(n2.fired());
    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_00_01)
{
    const float weight = 100.0f;

    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_FALSE(n2.fired());

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_00_10)
{
    const float weight = 100.0f;

    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_FALSE(n2.fired());

    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_00_11)
{
    const float weight = 100.0f;

    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_00)
{
    const float weight = 100.0f;

    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_FALSE(n2.fired());
    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_01)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n2.spike();
    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynNegWeight_01_01)
{
    const float weight = -100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n2.spike();
    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_EQ(weight - 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_10)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.spike();
    n1.step(TIME_STEP);
    n2.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_FALSE(n2.fired());

    // No changes expected if no spikes occured.
    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_11)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.spike();
    n1.step(TIME_STEP);
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_10_00)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.step(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.step(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    EXPECT_EQ(weight, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_10_01)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.step(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight - 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_10_10)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.step(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    // Since no 2 spikes occured no need to increase weight.
    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_10_11)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.step(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_11_11)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.spike();
    n1.step(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.step(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testAutoSpikeWoExt)
{
    Neuron n1(0), n2(0);
    n1.connectTo(&n2);

    for (int i = 0; i < 1000000; ++i)
    {
        n1.step(TIME_STEP);
        n2.step(TIME_STEP);
        EXPECT_FALSE(n1.fired());
        EXPECT_FALSE(n2.fired());
    }
}

TEST(Neuron, testAutoSpikeWExt)
{
    Neuron n1(31.0f), n2(40.0f);
    n1.connectTo(&n2);

    bool foundSpike = false;
    for (int i = 0; i < 1000000; ++i)
    {
        n1.step(TIME_STEP);
        n2.spike();
        n2.step(TIME_STEP);
        if (n1.fired())
        {
            foundSpike = true;
        }
    }

    EXPECT_FALSE(foundSpike);
}

float getRndWeight()
{
    const float maxWeight = 100.0f;
    const float minWeight = 50.0f;

    return (rand() % ((int)(maxWeight - minWeight) * 10)) / 10.0f + minWeight;
}

TEST(Neuron, testEmptyAverageMembraneValue)
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

    bool spikeFound = false;
    for (unsigned t = 0; t < (1000/0.5f); ++t)
    {
        float mean = 0.0f;
        for (unsigned i = 0; i < neuronsCount; ++i)
        {
            neurons[i].step(TIME_STEP);
            if (neurons[i].fired())
            {
                spikeFound = true;
            }
        }
    }
    EXPECT_FALSE(spikeFound);
}

float getRndExtI()
{
    const float extIMax = 40.0f;
    return (rand() % (int) (extIMax * 10)) / 10.0f;
}

TEST(Neuron, testAverageMembraneValue)
{
    const int neuronsCount = 125;
    const int connsCount  = neuronsCount * neuronsCount * 0.1f;

    Neuron* neurons[neuronsCount];
    for (int i = 0; i < neuronsCount; ++i)
    {
        neurons[i] = new Neuron(getRndExtI());
    }

    for (int i = 0; i < connsCount; ++i)
    {
        int pre = rand() % neuronsCount;
        int post = rand() % neuronsCount;
        float w = getRndWeight();
        if (pre >= 100)
        {
            w = -w;
        }

        neurons[pre]->connectTo(neurons[post], w);
    }

    std::ofstream f;
    f.open("average.csv");

    bool spikeFound = false;
    for (unsigned t = 0; t < (1000/0.5f); ++t)
    {
        float mean = 0.0f;
        for (unsigned i = 0; i < neuronsCount; ++i)
        {
            neurons[i]->step(TIME_STEP);
            mean += neurons[i]->getMembraneValue();
            if (neurons[i]->fired())
            {
                spikeFound = true;
            }
        }
        mean /= neuronsCount;

        if (mean > 0)
        {
            spikeFound = true;
        }
        f << t * TIME_STEP << "; " << mean << "; " << std::endl;
    }

    EXPECT_TRUE(spikeFound);
    f.close();

    for (int i = 0; i < neuronsCount; ++i)
    {
        delete neurons[i];
    }
}

TEST(Neuron, testPosNegWeight)
{
    Neuron n1, n2, n3;

    n1.connectTo(&n3, 10.0f);
    n2.connectTo(&n3, -10.0f);
    for (unsigned t = 0; t < 100000; ++t)
    {
        n1.spike();
        n1.step(TIME_STEP);
        n2.spike();
        n2.step(TIME_STEP);
        n3.step(TIME_STEP);

        EXPECT_FALSE(n3.fired());
    }
}

TEST(Neuron, testMorePosThanNegWeight)
{
    Neuron n1, n2, n3;

    n1.connectTo(&n3, 40.0f);
    n2.connectTo(&n3, -5.0f);
    bool spikeFound = false;
    for (unsigned t = 0; t < 100000; ++t)
    {
        n1.spike();
        n1.step(TIME_STEP);
        n2.spike();
        n2.step(TIME_STEP);
        n3.step(TIME_STEP);

        if (n3.fired())
        {
            spikeFound = true;
        }
    }

    EXPECT_TRUE(spikeFound);
}

TEST(Neuron, testWeight)
{
    const float weights[]
        = {0.0f, 10.0f, 30.0f, 31.0f, 32.0f, 33.0f, 35.0f, 40.0f, 50.0f, 70.0f, 90.0f, 100.0f, 150.0f, 180.0f, 200.0f, 220.0f, 250.0f};
    const int iters[]
        = {-1  , -1   , -1   , -1   , 333  , 229  , 165  , 111  , 75   , 50   , 39   , 36    , 26    , 22    , 21    , 19    , 18};

    for (int i = 0; i < (sizeof(weights)/sizeof(float)); ++i)
    {
        Neuron n1, n2;

        n1.connectTo(&n2, weights[i]);
        int spikeFound = -1;
        for (int t = 0; t < 1000; ++t)
        {
            n1.spike();
            n1.step(TIME_STEP);
            n2.step(TIME_STEP);
            if (n2.fired())
            {
                spikeFound = t;
                break;
            }
        }

        EXPECT_EQ(spikeFound, iters[i]);
    }
}