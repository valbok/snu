/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date Apr 2015
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 * @package Snu
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <EasyGL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "gettime.h"

#include <snu/worm/CElegans.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace NSnu;

CElegans Worm;
int   BehaviorSequenceLength = -1;
int   BehaviorSequenceIndex  = 0;
int   MotorOutputDelay       = -1;
int   MotorDelayCount        = 0;
float InitialPhase           = 0.0f;

// Dimensions.
#define WINDOW_WIDTH     550
#define WINDOW_HEIGHT    550
float WindowWidth      = (float)WINDOW_WIDTH;
float WindowHeight     = (float)WINDOW_HEIGHT;
float TouchHeightRatio = 0.1f;
float RunHeightRatio   = 0.1f;

// Controls.
class EventsHandler : public GUIEventListener
{
public:
   virtual void actionPerformed(GUIEvent& evt);
};

EventsHandler Handler;
GUICheckBox   *TouchCheck;
GUICheckBox   *PauseCheck;
GUIFrame      *GuiFrame = NULL;
bool          Run = false;
bool          Step = true;

// Body sinusoid.
int   BodyJoints     = 12;
float BodyAmplitude  = 0.05f;
float BodyPeriod     = 5.0f;
float BodyRadius     = 0.02f;

// Body animation timer.
#define MS_ANIMATION_TIMER    100
TIME msAnimationTimer = INVALID_TIME;

/*
 *  Available fonts:
 *  GLUT_BITMAP_8_BY_13
 *  GLUT_BITMAP_9_BY_15
 *  GLUT_BITMAP_TIMES_ROMAN_10
 *  GLUT_BITMAP_TIMES_ROMAN_24
 *  GLUT_BITMAP_HELVETICA_10
 *  GLUT_BITMAP_HELVETICA_12
 *  GLUT_BITMAP_HELVETICA_18
 */
#define SMALL_FONT    GLUT_BITMAP_8_BY_13
#define FONT          GLUT_BITMAP_9_BY_15
#define BIG_FONT      GLUT_BITMAP_TIMES_ROMAN_24
#define LINE_SPACE    15

// Pi.
float M_PI_X2 = M_PI * 2.0f;

class Point2D
{
public:
   float x;
   float y;

   Point2D()
   {
      x = y = 0.0f;
   }


   Point2D(float x, float y)
   {
      this->x = x;
      this->y = y;
   }
};

// Draw a circle.
void drawCircle(float x, float y, float radius, bool solid)
{
   if (solid)
   {
      glBegin(GL_POLYGON);
   }
   else
   {
      glBegin(GL_LINE_LOOP);
   }
   float rx = radius;
   float ry = radius;
   glVertex2f(x + rx, y);
   int   sides = 20;
   float ad    = M_PI_X2 / (float)sides;
   float a     = ad;
   for (int i = 1; i < sides; i++, a += ad)
   {
      glVertex2f(x + (rx * cos(a)), y + (ry * sin(a)));
   }
   glEnd();
}


// Window reshape.
void reshape(int width, int height)
{
   glViewport(0, 0, width, height);
   WindowWidth  = (float)width;
   WindowHeight = (float)height;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, WindowWidth, 0.0f, WindowHeight);
   glScalef(1.0f, -1.0f, 1.0f);
   glTranslatef(0.0f, -WindowHeight, 0.0f);
   GuiFrame->setDimensions(WindowWidth, WindowHeight);
   GuiFrame->forceUpdate(true);
   glutPostRedisplay();
}

// Idle.
void idle()
{
   if (!PauseCheck->isChecked() || Step)
   {
      Step = false;

      // Time to run?
      TIME t = gettime();
      if ((t - msAnimationTimer) >= MS_ANIMATION_TIMER)
      {
         msAnimationTimer = t;
         Run = true;
      }
   }
   glutPostRedisplay();
}


// Keyboard input.
void keyInput(unsigned char key, int x, int y)
{
   if (key == 'q')
   {
      exit(0);
   }
}

// Mouse callbacks.
void mouseClicked(int button, int state, int x, int y)
{
   if (button != GLUT_LEFT_BUTTON)
   {
      return;
   }
   MouseEvent event = MouseEvent(MB_BUTTON1, x, y, GuiFrame->getHeight() - y);
   GuiFrame->checkMouseEvents(event, (state == GLUT_DOWN) ? ME_CLICKED : ME_RELEASED);
}


void mouseDragged(int x, int y)
{
   MouseEvent event = MouseEvent(MB_UNKNOWN_BUTTON, x, y, GuiFrame->getHeight() - y);

   GuiFrame->checkMouseEvents(event, ME_DRAGGED);
}


void mouseMoved(int x, int y)
{
   MouseEvent event = MouseEvent(MB_UNKNOWN_BUTTON, x, y, GuiFrame->getHeight() - y);

   GuiFrame->checkMouseEvents(event, ME_MOVED);
}


// GUI event handler.
void EventsHandler::actionPerformed(GUIEvent& evt)
{
   const std::string& callbackString   = evt.getCallbackString();
   GUIRectangle       *sourceRectangle = evt.getEventSource(),
   *parent        = sourceRectangle ? sourceRectangle->getParent() : NULL;
   int widgetType = sourceRectangle->getWidgetType();

   if (widgetType == WT_CHECK_BOX)
   {
      GUICheckBox *checkbox = (GUICheckBox *)sourceRectangle;

      // Touch?
      if (callbackString == (char *)"touch")
      {
         return;
      }

      // Pause?
      if (callbackString == (char *)"pause")
      {
         return;
      }
   }

   if (widgetType == WT_BUTTON)
   {
      GUIButton *button = (GUIButton *)sourceRectangle;

      if (callbackString == "step")
      {
         if (button->isClicked())
         {
            Step = true;
         }
         return;
      }
   }
}


// Print string on screen at specified location.
void renderBitmapString(float x, float y, void *font, char *string)
{
   char *c;

   glRasterPos2f(x, y);
   for (c = string; *c != '\0'; c++)
   {
      glutBitmapCharacter(font, *c);
   }
}

// Display.
void display()
{
   int   i, j;
   bool  isTouched;
   float bodyOffset, bodyHeight, d;
   float angle, xdelta, ydelta, radius;

   vector<Point2D> centers, dorsals, ventrals;
   
   float ventralMagnitudes[24];
   float dorsalMagnitudes[24];
   Point2D         point;
   char            buf[50];

   glClear(GL_COLOR_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glColor3f(1.0f, 1.0f, 1.0f);

   bodyOffset = WindowHeight * TouchHeightRatio;
   bodyHeight = WindowHeight - bodyOffset - (WindowHeight * RunHeightRatio);
   glBegin(GL_LINES);
   d = bodyOffset * 0.95f;
   glVertex2f(WindowWidth * 0.25f, d);
   glVertex2f(WindowWidth * 0.75f, d);
   glVertex2f(WindowWidth * 0.25f, 0.0f);
   d = WindowHeight * (1.0f - RunHeightRatio) * 1.005f;
   glVertex2f(WindowWidth * 0.25f, d);
   glVertex2f(WindowWidth * 0.75f, 0.0f);
   glVertex2f(WindowWidth * 0.75f, d);
   glVertex2f(0.0f, d);
   glVertex2f(WindowWidth, d);
   glEnd();

   // Controls.
   GuiFrame->render((float)gettime());

   Worm.getDorsalMagnitudes(dorsalMagnitudes);
   Worm.getVentralMagnitudes(ventralMagnitudes);

   isTouched = TouchCheck->isChecked();
   xdelta = M_PI_X2 / (float)(BodyJoints - 1);
   if (isTouched)
   {
      Worm.touchNose();
   }
   else
   {
      Worm.findFood();
   }

   radius  = WindowWidth * BodyRadius;
   point.y = bodyOffset + radius;
   ydelta  = (bodyHeight - (radius * 2.0f)) / (float)(BodyJoints - 1);
   for (i = 0, angle = 0.0f; i < BodyJoints; i++, angle += xdelta)
   {
      point.x = dorsalMagnitudes[i] - ventralMagnitudes[i];

      point.x *= (BodyAmplitude * WindowWidth);
      point.x += (WindowWidth * 0.5f);
      centers.push_back(point);
      point.x += (WindowWidth * BodyRadius);
      dorsals.push_back(point);
      point.x -= (WindowWidth * BodyRadius * 2.0f);
      ventrals.push_back(point);
      point.y += ydelta;
   }

   glColor3f(0.5f, 0.5f, 0.5f);
   for (i = 0, j = (int)dorsals.size() - 1; i < j; i++)
   {
      glBegin(GL_POLYGON);
      point = dorsals[i];
      glVertex2f(point.x, point.y);
      point = dorsals[i + 1];
      glVertex2f(point.x, point.y);
      point = ventrals[i + 1];
      glVertex2f(point.x, point.y);
      point = ventrals[i];
      glVertex2f(point.x, point.y);
      glEnd();
   }
   radius = WindowWidth * (BodyRadius * 0.9f);
   point  = centers[0];
   drawCircle(point.x, point.y, radius, true);
   point = centers[centers.size() - 1];
   drawCircle(point.x, point.y, radius, true);
   glColor3f(1.0f, 1.0f, 1.0f);
   glBegin(GL_LINE_STRIP);
   for (i = 0, j = (int)centers.size(); i < j; i++)
   {
      point = centers[i];
      glVertex2f(point.x, point.y);
   }
   glEnd();

   for (i = 0, j = (int)centers.size(); i < j; i++)
   {
      point = centers[i];

      glColor3f(1.0f, 1.0f, 1.0f);
      drawCircle(point.x, point.y, radius, false);
      glColor3f(dorsalMagnitudes[i], 0.0f, 0.0f);
      drawCircle((WindowWidth * 0.75f) + (radius * 1.5f), point.y, radius, true);
      glColor3f(1.0f, 1.0f, 1.0f);
      sprintf(buf, (char *)"%0.2f", dorsalMagnitudes[i]);
      renderBitmapString((WindowWidth * 0.75f) - (radius * 5.0f), point.y + (radius * 0.4f), FONT, buf);
      glColor3f(ventralMagnitudes[i], 0.0f, 0.0f);
      drawCircle((WindowWidth * 0.25f) - (radius * 1.5f), point.y, radius, true);
      glColor3f(1.0f, 1.0f, 1.0f);
      sprintf(buf, (char *)"%0.2f", ventralMagnitudes[i]);
      renderBitmapString((WindowWidth * 0.25f) + (radius), point.y + (radius * 0.4f), FONT, buf);

      switch (i)
      {
      case 0:
         sprintf(buf, (char *)"1,2");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 1:
         sprintf(buf, (char *)"3,4");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 2:
         sprintf(buf, (char *)"5,6");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 3:
         sprintf(buf, (char *)"7,8");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 4:
         sprintf(buf, (char *)"9,10");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 5:
         sprintf(buf, (char *)"11,12");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 6:
         sprintf(buf, (char *)"13,14");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 7:
         sprintf(buf, (char *)"15,16");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 8:
         sprintf(buf, (char *)"17,18");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 9:
         sprintf(buf, (char *)"19,20");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 10:
         sprintf(buf, (char *)"21,22");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;

      case 11:
         sprintf(buf, (char *)"23,24");
         renderBitmapString((WindowWidth * 0.25f) - (radius * 11.5f), point.y + (radius * 0.4f), FONT, buf);
         renderBitmapString((WindowWidth * 0.75f) + (radius * 3.5f), point.y + (radius * 0.4f), FONT, buf);
         break;
      }
   }

   renderBitmapString(10, 15, FONT, (char *)"MV R/L");
   renderBitmapString((int)(WindowWidth * 0.75f) + 10, 15, FONT, (char *)"MD R/L");

   glColor3f(0.0f, 0.0f, 0.0f);

   glutSwapBuffers();
   glFlush();

   if (Run)
   {
      Run = false;
      BehaviorSequenceIndex++;
      Worm.step(0.5, 1800);
   }
}

int main(int argc, char *argv[])
{
   MotorOutputDelay = 0;

   // Initialize.
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize((int)WindowWidth, (int)WindowHeight);
   glutCreateWindow("C. elegans");
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutKeyboardFunc(keyInput);
   glutMouseFunc(mouseClicked);
   glutMotionFunc(mouseDragged);
   glutPassiveMotionFunc(mouseMoved);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0f, WindowWidth, 0.0f, WindowHeight);
   glScalef(1.0f, -1.0f, 1.0f);
   glTranslatef(0.0f, -WindowHeight, 0.0f);
   msAnimationTimer = gettime();
   GLeeInit();
   MediaPathManager::registerPath("resource/");
   GuiFrame = new GUIFrame();
   assert(GuiFrame != NULL);
   GuiFrame->GUIPanel::loadXMLSettings("GUILayout.xml");
   GuiFrame->setGUIEventListener(&Handler);
   TouchCheck = (GUICheckBox *)GuiFrame->getWidgetByCallbackString("touch");
   TouchCheck->setAlphaFadeScale(1000.0);
   PauseCheck = (GUICheckBox *)GuiFrame->getWidgetByCallbackString("pause");
   PauseCheck->setAlphaFadeScale(1000.0);
   Run  = false;
   Step = false;

   // Start up.
   glutReshapeWindow(WindowWidth, WindowHeight);
   glutMainLoop();
   
   return 0;
}
