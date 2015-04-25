/*
    Enki - a fast 2D robot simulator
    Copyright (C) 1999-2013 Stephane Magnenat <stephane at magnenat dot net>
    Copyright (C) 2004-2005 Markus Waibel <markus dot waibel at epfl dot ch>
    Copyright (c) 2004-2005 Antoine Beyeler <abeyeler at ab-ware dot com>
    Copyright (C) 2005-2006 Laboratory of Intelligent Systems, EPFL, Lausanne
    Copyright (C) 2006-2008 Laboratory of Robotics Systems, EPFL, Lausanne
    See AUTHORS for details

    This program is free software; the authors of any publication
    arising from research using this software are asked to add the
    following reference:
    Enki - a fast 2D robot simulator
    http://home.gna.org/enki
    Stephane Magnenat <stephane at magnenat dot net>,
    Markus Waibel <markus dot waibel at epfl dot ch>
    Laboratory of Intelligent Systems, EPFL, Lausanne.

    You can redistribute this program and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Viewer.h"
#include <enki/PhysicalEngine.h>
#include <snu/robots/e-puck/SnuPuck.hpp>
#include <enki/robots/marxbot/Marxbot.h>
#include <QApplication>
#include <QtGui>
#include <iostream>

using namespace Enki;
using namespace std;
using namespace NSnu;

const float TIME_STEP = 0.5;
const unsigned PERIOD = 1800;

class EnkiPlayground : public ViewerWidget
{
protected:
	bool subjectiveView;
	QVector<SnuPuck*> epucks;
	QMap<PhysicalObject*, int> bullets;

public:
	EnkiPlayground(World *world, QWidget *parent = 0) :
		ViewerWidget(world, parent),
		subjectiveView(false)
	{
		#define PROBLEM_GENERIC_TOY
		#define PROBLEM_BALL_LINE
		//#define PROBLEM_LONE_EPUCK

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

		for (int i = 0; i < 10; i++)
		{
			PhysicalObject* o = new PhysicalObject;
			o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			//o->pos = Point(80, 50);
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
			o->setCustomHull(hull, 20);
			o->setColor(Color(0.2, 0.1, 0.6));
			o->collisionElasticity = 0.2;
			o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			//o->pos = Point(75, 50);
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
			//o->pos = Point(UniformRand(20, 100)(), UniformRand(20, 100)());
			o->pos = Point(70, 50);
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

		addDefaultsRobots(world);

		altitude = 150;
		pos = QPointF(0,60);
	}

	void addDefaultsRobots(World *world)
	{
		SnuPuck* snu = new SnuPuck;
		snu->pos = Point(60, 80);
		snu->leftSpeed = 5;
		snu->rightSpeed = 5;
		epucks.push_back(snu);
		world->addObject(snu);

		// Gotten from tests. Avoiding any teaching.
		const float empiricNoseFrequency = 0.12227273;
		const float empiricFoodFrequency = 0.093585856;
		snu->setNosePeriodFrequency(empiricNoseFrequency);
		snu->setFoodPeriodFrequency(empiricFoodFrequency);
	}

	~EnkiPlayground()
	{
	}

	virtual void timerEvent(QTimerEvent * event)
	{
		SnuPuck* snu = epucks[0];
		snu->stop();
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

	virtual void keyPressEvent ( QKeyEvent * event )
	{
		if (event->key() == Qt::Key_C)
		{
			subjectiveView = !subjectiveView;
			if (subjectiveView)
				pitch = M_PI/8;
			event->accept();
		}
		else
			ViewerWidget::keyPressEvent(event);
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
	World world(120, Color(0.9, 0.9, 0.9), igt ? World::GroundTexture(gt.width(), gt.height(), (const uint32_t*)gt.constBits()) : World::GroundTexture());
	#else
	World world(120, Color(0.9, 0.9, 0.9), igt ? World::GroundTexture(gt.width(), gt.height(), (uint32_t*)gt.bits()) : World::GroundTexture());
	#endif
	EnkiPlayground viewer(&world);

	viewer.show();

	return app.exec();
}

