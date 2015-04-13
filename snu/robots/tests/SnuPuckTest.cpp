/**
 * @author VaL Doroshchuk
 * @license GNU GPL v2
 */

#include <snu/robots/SnuPuck.hpp>
#include <enki/PhysicalEngine.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace NSnu;
using namespace std;

TEST(SnuPack, test)
{
    // Create the world
    Enki::World world(200, 200);

    // Create a Khepera and position it
    SnuPuck *snu = new SnuPuck;
    snu->pos = Enki::Point(100, 100);
    //snu->leftSpeed = 20;
    //snu->rightSpeed = 20;

    // objects are garbage collected by the world on destruction
    world.addObject(snu);

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
    o->pos = Enki::Point(115, 100);
    o->setColor(Enki::Color(0.4,0.6,0.8));
    world.addObject(o);

    // Run for some times
    for (unsigned i=0; i<1000; i++)
    {
        snu->tick(0.5);
        // step of 50 ms
        world.step(0.2);
        std::cout << "\n[" << i << "]\nObject pos is (" << o->pos.x << "," << o->pos.y << ")" << std::endl;
        std::cout << "Snu pos is (" << snu->pos.x << "," << snu->pos.y << ")" << std::endl;

    }

}

