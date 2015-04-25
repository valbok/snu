/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/robots/e-puck/SnuPuck.hpp>
#include <enki/PhysicalEngine.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;
using namespace std;

const float TIME_STEP = 0.5;
const unsigned PERIOD = 1800;

TEST(SnuPack, test)
{
    // Create the world
    Enki::World world(1000, 1000);

    SnuPuck *snu = new SnuPuck;
    snu->pos = Enki::Point(100, 100);

    // objects are garbage collected by the world on destruction
    world.addObject(snu);
    const float empiricNoseFrequency = 0.12227273;
    const float empiricFoodFrequency = 0.093585856;
    snu->setNosePeriodFrequency(empiricNoseFrequency);
    snu->setFoodPeriodFrequency(empiricFoodFrequency);

    Enki::Polygone p;
    const double amount = 9;
    const double radius = 5;
    const double height = 20;
    for (double a = 0; a < 2*M_PI; a += 2*M_PI/amount)
    {
        p.push_back(Enki::Point(radius * cos(a), radius * sin(a)));
    }

    Enki::PhysicalObject* o = new Enki::PhysicalObject;
    Enki::PhysicalObject::Hull hull(Enki::PhysicalObject::Part(p, height));
    o->setCustomHull(hull, 1);
    o->pos = Enki::Point(150, 100);
    o->setColor(Enki::Color(0.4,0.6,0.8));
    world.addObject(o);

    // Run for some times
    for (unsigned i = 0; i<10000; ++i)
    {
        float speed = snu->leftSpeed;
        snu->stop();
        for (int i = 0; i < 250; ++i)
        {
            snu->step(TIME_STEP, PERIOD);
        }
        snu->move();

        // step of 50 ms
        world.step(0.5);
        if (snu->frontOrBackTouched())
        {
            if (speed > 0)
            {
                EXPECT_TRUE(snu->leftSpeed < 0);
            }
            else
            {
                EXPECT_TRUE(snu->leftSpeed > 0);   
            }

        }
    }

}

