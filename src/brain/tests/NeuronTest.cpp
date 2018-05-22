/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include "Neuron.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

using namespace std;

const float TIME_STEP = 1;

TEST(Connection, testWeight)
{
    EXPECT_EQ(9, int(Connection(0, 10).weight()));
    EXPECT_EQ(39, int(Connection(0, 50).weight()));
    EXPECT_EQ(-39, int(Connection(0, -50).weight()));
    EXPECT_EQ(63, int(Connection(0, 100).weight()));
    EXPECT_EQ(-63, int(Connection(0, -100).weight()));
    EXPECT_EQ(100, int(Connection(0, 10000).weight()));
    EXPECT_EQ(99, int(Connection(0, 100, 100, 10).weight()));
    EXPECT_EQ(86, int(Connection(0, 100, 100, 50).weight()));
    EXPECT_EQ(81, int(Connection(0, 100, 100, 60).weight()));
    EXPECT_EQ(67, int(Connection(0, 100, 100, 90).weight()));
}

TEST(Connection, testTeach)
{
    Connection s(0, 10);
    s.teach(1);
    EXPECT_EQ(Connection(0, 11).weight(), s.weight());
    s.teach(10);
    EXPECT_EQ(Connection(0, 21).weight(), s.weight());
}

TEST(Connection, testSpike)
{
    Neuron n;
    EXPECT_EQ(0, n.syn());
    Connection s(&n, 10);
    s.spike();
    EXPECT_EQ(Connection(0, 10).weight(), n.syn());
}

TEST(Neuron, testConnectTo)
{
    Neuron n1;
    Neuron n2;

    EXPECT_EQ(0, n1.connections().size());

    n1.connectTo(&n2, 50.0f);

    EXPECT_EQ(1, n1.connections().size());
    EXPECT_EQ(0, n2.connections().size());
}

TEST(Neuron, testSpike)
{
    Neuron n1;

    n1.spike();
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n1.mem() > 0);

    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n1.mem() < 0);

    n1.tick(TIME_STEP);
    EXPECT_FALSE(n1.fired());
    EXPECT_TRUE(n1.mem() < 0);

    n1.spike();
    EXPECT_TRUE(n1.mem() > 0);

    n1.tick(TIME_STEP);
    EXPECT_TRUE(n1.fired());
    EXPECT_TRUE(n1.mem() < 0);
}

TEST(Neuron, testApply)
{
    Neuron n;
    EXPECT_EQ(0, n.syn());
    EXPECT_TRUE(n.mem() < 0);
    n.apply(100);
    EXPECT_EQ(100, n.syn());
}

TEST(Neuron, testApplyInTime)
{
    std::ofstream fn, fns;
    fn.open("testApply.csv");
    fns.open("testApply.txt");

    Neuron n(10, 1, 50);

    for (int t = 0; t < 1000; ++t) {
        float mem = n.mem();
        fn << t << "; " << mem << "; " << std::endl;
        fns << ">" << t << " U=" << mem << " I="<< n.syn() << std::endl;

        bool fired = n.tick(TIME_STEP);
        if (fired)
            fns << "\t\t\t****** spike\n";

        if (t == 10)
            n.apply(50);

        if (t == 48)
            EXPECT_TRUE(n.mem() > 0);

        if (t == 49)
            EXPECT_TRUE(fired);

        if (t == 100)
            EXPECT_FALSE(n.apply(80));

        if (t == 200)
            EXPECT_TRUE(n.apply(50));

        if (t == 230)
            EXPECT_TRUE(n.apply(-50));

        if (t == 260)
            EXPECT_TRUE(n.apply(-50));

        if (t == 340)
            EXPECT_TRUE(n.apply(10));

        if (t == 400)
            EXPECT_TRUE(n.apply(160));

        if (t == 460)
            EXPECT_FALSE(n.apply(160));

        if (t >=50 && t < 414)
            EXPECT_FALSE(fired);

        if (t == 580)
            EXPECT_TRUE(n.apply(50));

        if (t > 650)
            EXPECT_FALSE(fired);
    }

}

TEST(Neuron, testTeachNegative)
{
    Neuron n1;
    Neuron n2(30);
    n1.connectTo(&n2, -50.0f);
    float w = n1.connections()[0].weight();
    std::ofstream fn;
    fn.open("testTeachNegative.csv");

    n1.spike();
    n2.apply(50);
    int spiked = 0;
    for (int t = 0; t < 600; ++t) {
        float mem = n2.mem();
        fn << t << "; " << mem << "; " << std::endl;
        n1.tick(TIME_STEP);
        if (n2.tick(TIME_STEP)) {
            EXPECT_TRUE(n1.connections()[0].weight() < w);
            w = n1.connections()[0].weight();
            ++spiked;
        }

        if (t == 180)
            EXPECT_TRUE(n2.apply(50));

        if (t == 200)
            n1.spike();

        if (t == 320)
            EXPECT_TRUE(n2.apply(50));

        if (t == 330)
            n1.spike();

    }
    EXPECT_EQ(2, spiked);
}

TEST(Neuron, testTeachPositive)
{
    Neuron n1;
    Neuron n2;
    n1.connectTo(&n2, 20.0f);
    float w = n1.connections()[0].weight();
    std::ofstream fn;
    fn.open("testTeachPositive.csv");

    int spiked = 0;
    for (int t = 0; t < 600; ++t) {
        float mem = n2.mem();
        fn << t << "; " << mem << "; " << std::endl;
        n1.tick(TIME_STEP);
        if (n2.tick(TIME_STEP)) {
            EXPECT_TRUE(n1.connections()[0].weight() > w);
            w = n1.connections()[0].weight();
            ++spiked;
        }

        if (t == 0 || t == 25 || t == 50 || t == 75 || t == 100
            || t == 210 || t == 220 || t == 230 || t == 240 || t == 250
            || t == 370 || t == 375 || t == 400 || t == 425) {
            n1.spike();
        }
    }

    EXPECT_TRUE(n1.connections()[0].weight() < w);
    EXPECT_EQ(2, spiked);
}

TEST(Neuron, testIncomingSpikes)
{
    Neuron n1;
    Neuron n2;
    Neuron n3;

    n1.connectTo(&n3, 150.0f);
    float w1 = n1.connections()[0].weight();
    n2.connectTo(&n3, -100.0f);
    float w2 = n2.connections()[0].weight();
    std::ofstream fn;
    fn.open("testIncomingSpikes.csv");

    for (int t = 0; t < 1000; ++t) {
        float mem = n3.mem();
        fn << t << "; " << mem << "; " << std::endl;
        n1.tick(TIME_STEP);
        n2.tick(TIME_STEP);
        n3.tick(TIME_STEP);

        if (t == 100)
            n1.spike();

        if (t == 105)
            EXPECT_EQ(1, n3.incomingSpikes().size());

        if (t == 110)
            n2.spike();

        if (t == 120)
            EXPECT_EQ(2, n3.incomingSpikes().size());

        // No spike
        if (t == 200) {
            EXPECT_EQ(0, n3.incomingSpikes().size());
            EXPECT_TRUE(n1.connections()[0].weight() < w1);
            // Negative connection increased
            EXPECT_TRUE(n2.connections()[0].weight() > w2);
            w1 = n1.connections()[0].weight();
            w2 = n2.connections()[0].weight();

            n1.spike();
        }

        if (t == 212)
            n2.spike();

        if (t == 215)
            EXPECT_EQ(2, n3.incomingSpikes().size());

        // Spike
        if (t == 240) {
            EXPECT_EQ(0, n3.incomingSpikes().size());
            EXPECT_TRUE(n1.connections()[0].weight() > w1);
            EXPECT_TRUE(n2.connections()[0].weight() < w2);
        }

        if (t == 241)
            n1.spike();

        if (t == 243)
            EXPECT_EQ(0, n3.incomingSpikes().size());

        if (t == 280)
            EXPECT_EQ(0, n3.incomingSpikes().size());

        if (t == 360)
            n2.spike();

        if (t == 480)
            EXPECT_EQ(1, n3.incomingSpikes().size());

    }
}




static float getRndExtI()
{
    const float extIMax = 40.0f;
    return (rand() % (int) (extIMax * 10)) / 10.0f;
}

static float getRndWeight()
{
    const float maxWeight = 100.f;
    const float minWeight = 50.f;

    return (rand() % ((int)(maxWeight - minWeight) * 10)) / 10.0f + minWeight;
}

TEST(Neuron, testAverageMembraneValue)
{
    const int neuronsCount = 5;
    const int connsCount  = neuronsCount * neuronsCount;
    std::srand(std::time(0));

    Neuron neurons[neuronsCount];
    for (int i = 0; i < connsCount; ++i)
    {
        int pre = rand() % neuronsCount;
        int post = rand() % neuronsCount;
        float w = getRndWeight();
        if (pre == post)
            --pre;
        if (pre < 0)
            pre = neuronsCount - 1;
        if (pre >= 2)
            w = -w;

        neurons[pre].connectTo(&neurons[post], w);
    }

    std::ofstream f, f0, f1, f2, f3, f4;
    f.open("average.csv");
    f0.open("average_n0.csv");
    f1.open("average_n1.csv");
    f2.open("average_n2.csv");
    f3.open("average_n3.csv");
    f4.open("average_n4.csv");

    bool spikeFound = false;
    for (int t = 0; t < 1000; ++t) {
        float mean = 0.0f;
        if (t % 100 == 0) {
            int p = rand() % neuronsCount;
            neurons[p].spike();
        }

        for (int i = 0; i < neuronsCount; ++i) {
            if (neurons[i].tick(TIME_STEP) && t % 100)
                spikeFound = true;
            mean += neurons[i].mem();
        }
        mean /= neuronsCount;
        f << t  << "; " << mean << "; " << std::endl;
        f0 << t << "; " << neurons[0].mem() << "; " << std::endl;
        f1 << t << "; " << neurons[1].mem() << "; " << std::endl;
        f2 << t << "; " << neurons[2].mem() << "; " << std::endl;
        f3 << t << "; " << neurons[3].mem() << "; " << std::endl;
        f4 << t << "; " << neurons[4].mem() << "; " << std::endl;
    }

    EXPECT_TRUE(spikeFound);
}

TEST(Neuron, testCube)
{
}
