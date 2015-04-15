/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/brain/Neuron.hpp>
#include <gtest/gtest.h>
#include <iostream>

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

    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() < 0);

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n1.getMembraneValue() < 0);

    n1.spike();
    EXPECT_TRUE(n1.getMembraneValue() > 0);

    n1.tick(TIME_STEP);
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

    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_EQ(1.0f, n1.getAxons()[0].curSynI);
    EXPECT_EQ(weight, n2.getSynI());

    n2.tick(TIME_STEP);
    EXPECT_EQ(1.0f, n1.getAxons()[0].curSynI);
    EXPECT_EQ(1.0f, n1.getAxons()[0].prevSynI);
    EXPECT_EQ(0.0f, n2.getSynI());

    n1.tick(TIME_STEP);
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

    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
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

    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_FALSE(n2.fired());

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
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

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
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

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_10)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.spike();
    n1.tick(TIME_STEP);
    n2.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_FALSE(n2.fired());

    EXPECT_EQ(weight - 1, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_01_11)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n2.fired());

    n1.spike();
    n1.tick(TIME_STEP);
    n2.spike();
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.tick(TIME_STEP);
    EXPECT_FALSE(n2.fired());

    // @todo Check if it is needed to be 0
    EXPECT_EQ(weight + 2, n1.getAxons()[0].weight);
}

TEST(Neuron, testTeachSynWeight_10_11)
{
    const float weight = 100.0f;
    Neuron n1(EXT_I);
    Neuron n2(EXT_I);
    n1.connectTo(&n2, weight);

    n1.spike();
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
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
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);

    n1.spike();
    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    n2.spike();
    n2.tick(TIME_STEP);
    EXPECT_TRUE(n2.fired());

    EXPECT_EQ(weight + 1, n1.getAxons()[0].weight);
}
