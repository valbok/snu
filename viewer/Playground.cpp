/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include "Viewer.h"
#include <enki/PhysicalEngine.h>
#include <snu/robots/e-puck/SnuPuck.hpp>
#include <enki/robots/marxbot/Marxbot.h>
#include <enki/robots/thymio2/Thymio2.h>
#include <QApplication>
#include <QtGui>
#include <iostream>

#ifdef USE_SDL
#include <SDL.h>
#endif

/*!	\file Studio.cpp
	\brief Test of the Qt-based viewer widget
*/

using namespace Enki;
using namespace std;
using namespace NSnu;

class EnkiPlayground : public ViewerWidget
{
protected:
	#ifdef USE_SDL
	QVector<SDL_Joystick *> joysticks;
	#endif
	QVector<SnuPuck*> epucks;
	QMap<PhysicalObject*, int> bullets;

public:
	EnkiPlayground(World *world, QWidget *parent = 0) :
		ViewerWidget(world, parent)
	{
		#define PROBLEM_CENTERED_THYMIO2
		#define PROBLEM_GENERIC_TOY
		#define PROBLEM_BALL_LINE

		#ifdef PROBLEM_GENERIC_TOY
		{
			const double amount = 9;
			const double radius = 5;
			const double height = 20;
			Polygone p;
			for (double a = 0; a < 2*M_PI; a += 2*M_PI/amount)
				p.push_back(Point(radius * cos(a), radius * sin(a)));

			PhysicalObject* o = new PhysicalObject;
			PhysicalObject::Hull hull(Enki::PhysicalObject::Part(p, height));
			o->setCustomHull(hull, -1);
			o->setColor(Color(0.4,0.6,0.8));
			o->pos = Point(100, 100);
			world->addObject(o);
		}

		for (int i = 0; i < 20; i++)
		{
			PhysicalObject* o = new PhysicalObject;
			o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			o->setCylindric(1, 1, 10);
			o->setColor(Color(0.9, 0.2, 0.2));
			o->dryFrictionCoefficient = 0.01;
			world->addObject(o);
		}

		Polygone p2;
		p2.push_back(Point(5,1));
		p2.push_back(Point(-5,1));
		p2.push_back(Point(-5,-1));
		p2.push_back(Point(5,-1));
		for (int i = 0; i < 5; i++)
		{
			PhysicalObject* o = new PhysicalObject;
			PhysicalObject::Hull hull(Enki::PhysicalObject::Part(p2, 3));
			o->setCustomHull(hull, 30);
			o->setColor(Color(0.2, 0.1, 0.6));
			o->collisionElasticity = 0.2;
			o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			world->addObject(o);
		}

		// cross shape
		{
			PhysicalObject* o = new PhysicalObject;
			PhysicalObject::Hull hull;
			hull.push_back(Enki::PhysicalObject::Part(Polygone() << Point(5,1) << Point(-5,1) << Point(-5,-1) << Point(5,-1), 2));
			hull.push_back(Enki::PhysicalObject::Part(Polygone() << Point(1,5) << Point(-1,5) << Point(-1,-5) << Point(1,-5), 4));
			o->setCustomHull(hull, 60);
			o->setColor(Color(0.2, 0.4, 0.6));
			o->collisionElasticity = 0.2;
			o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			world->addObject(o);
		}
		#endif // PROBLEM_GENERIC_TOY

		#ifdef PROBLEM_BALL_LINE
		for (double d = 40; d < 60; d += 8)
		{
			PhysicalObject* o = new PhysicalObject;
			o->pos = Point(d, 20);
			o->setCylindric(4, 2, 10);
			o->setColor(Color(0.2, 0.2, 0.6));
			o->dryFrictionCoefficient = 0.;
			world->addObject(o);
		}
		#endif // PROBLEM_BALL_LINE

		#ifdef PROBLEM_LONE_EPUCK
		addDefaultsRobots(world);
		#endif // PROBLEM_LONE_EPUCK

		#ifdef USE_SDL
		if((SDL_Init(SDL_INIT_JOYSTICK)==-1))
		{
			cerr << "Error : Could not initialize SDL: " << SDL_GetError() << endl;
			addDefaultsRobots(world);
			return;
		}

		int joystickCount = SDL_NumJoysticks();
		for (int i = 0; i < joystickCount; ++i)
		{
			SDL_Joystick* joystick = SDL_JoystickOpen(i);
			if (!joystick)
			{
				cerr << "Error: Can't open joystick " << i << endl;
				continue;
			}
			if (SDL_JoystickNumAxes(joystick) < 2)
			{
				cerr << "Error: not enough axis on joystick" << i<< endl;
				SDL_JoystickClose(joystick);
				continue;
			}
			joysticks.push_back(joystick);

			addDefaultsRobots(world);
		}
		cout << "Added " << joystickCount << " controlled e-pucks." << endl;
		#else // USE_SDL
		addDefaultsRobots(world);
		#endif // USE_SDL
	}

	void addDefaultsRobots(World *world)
	{
		SnuPuck* snu = new SnuPuck;
		snu->pos = Point(70, 80);
		snu->leftSpeed = 5;
		snu->rightSpeed = 5;
		epucks.push_back(snu);
		world->addObject(snu);

		// Gotten from tests. Avoiding any teaching.
		const float empiricNoseFrequency = 0.12469697;
		const float empiricFoodFrequency = 0.094898984;
		snu->setNosePeriodFrequency(empiricNoseFrequency);
		snu->setFoodPeriodFrequency(empiricFoodFrequency);
	}

	~EnkiPlayground()
	{
		#ifdef USE_SDL
		for (int i = 0; i < joysticks.size(); ++i)
			SDL_JoystickClose(joysticks[i]);
		SDL_Quit();
		#endif
	}

	virtual void timerEvent(QTimerEvent * event)
	{
		SnuPuck* snu = epucks[0];
		snu->stop();
		const float TIME_STEP = 0.5;
		const unsigned PERIOD = 1800;
		for (int i = 0; i < 100; ++i)
		{
			snu->step(TIME_STEP, PERIOD);
		}
		snu->move();

		doDumpFrames = false;
		QMap<PhysicalObject*, int>::iterator i = bullets.begin();
		while (i != bullets.end())
		{
			QMap<PhysicalObject*, int>::iterator oi = i;
			++i;
			if (oi.value())
			{
				oi.value()--;
			}
			else
			{
				PhysicalObject* o = oi.key();
				world->removeObject(o);
				bullets.erase(oi);
				delete o;
			}
		}
		ViewerWidget::timerEvent(event);
	}

	virtual void sceneCompletedHook()
	{
		glColor3d(0,0,0);
		renderText(10, height()-50, tr("rotate camera by moving mouse while pressing ctrl+left mouse button"));
		renderText(10, height()-30, tr("move camera on x/y by moving mouse while pressing ctrl+shift+left mouse button"));
		renderText(10, height()-10, tr("move camera on z by moving mouse while pressing ctrl+shift+right mouse button"));
		for (unsigned i = 0; i < epucks.size(); ++i)
		{
			string s = "front-front-right = " + std::to_string(epucks[i]->infraredSensor0.getDist());
			renderText(10, 20*(i+1), s.c_str());
			s = "front-front-left = " + std::to_string(epucks[i]->infraredSensor7.getDist());
			renderText(10, 40*(i+1), s.c_str());

			s = "back-right = " + std::to_string(epucks[i]->infraredSensor3.getDist());
			renderText(10, 60*(i+1), s.c_str());

			s = "back-left = " + std::to_string(epucks[i]->infraredSensor4.getDist());
			renderText(10, 80*(i+1), s.c_str());

			s = "right = " + std::to_string(epucks[i]->infraredSensor2.getDist());
			renderText(10, 100*(i+1), s.c_str());

			s = "left = " + std::to_string(epucks[i]->infraredSensor5.getDist());
			renderText(10, 120*(i+1), s.c_str());

			s = "leftSpeed = " + std::to_string(epucks[i]->leftSpeed);
			renderText(10, 140*(i+1), s.c_str());

			s = "rightSpeed = " + std::to_string(epucks[i]->rightSpeed);
			renderText(10, 160*(i+1), s.c_str());
		}
	}
};

// http://qtnode.net/wiki?title=Qt_with_cmake
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// Create the world and the viewer
	bool igt(app.arguments().size() > 1);
	QImage gt;
	if (igt)
		gt = QGLWidget::convertToGLFormat(QImage(app.arguments().last()));
	igt = !gt.isNull();
	#if QT_VERSION >= QT_VERSION_CHECK(4,7,0)
	const uint32_t *bits = (const uint32_t*)gt.constBits();
	#else
	uint32_t *bits = (uint32_t*)gt.bits();
	#endif
	World world(120, Color(0.9, 0.9, 0.9), igt ? World::GroundTexture(gt.width(), gt.height(), bits) : World::GroundTexture());
	EnkiPlayground viewer(&world);

	viewer.show();

	return app.exec();
}

