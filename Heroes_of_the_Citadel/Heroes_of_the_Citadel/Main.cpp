#include "Game.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <chrono>
#include <fenv.h>
#include <unordered_map>

#include "FixedPoint.h"
#include "IntFloat.h"
#include "GlobalConstants.h"
#include "DebugInfo.h"
#include "DebugTimer.h"
#include "Evolution.h"

inline int numLeadingZeroes1 (int64_t v) {
   double ff = (double)(v | 1);

   uint32_t tmp;
   memcpy (&tmp, ((const char*)& ff) + sizeof (uint32_t), sizeof (uint32_t));
   return (tmp >> 20) - 1023;
}

inline int numLeadingZeroes2 (int64_t v) {
   int count;
   std::frexp (v, &count);
   return count - 1;
}

/*void timeCode () {
   int64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now ().time_since_epoch ()
      ).count ();

   //IntFloat a{ 10.0 };
   //IntFloat b{ -5.0 };
   //IntFloat c;
   FixedPoint a{ 1234.456 };
   FixedPoint b{ -114.654 };
   FixedPoint c;
   //double a = 10.;
   //double b = -5.;
   //double c;
   for (size_t ii = 0; ii < 100000000; ii++) {
      c = a + b;
   }
   std::cout << c.toDouble() << std::endl;
   //std::cout << c << std::endl;

   int64_t timespan = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now ().time_since_epoch ()
      ).count () - start;
   std::cout << "timespan: " << timespan << std::endl;
}*/

void configure (Game* game) {
   // Create balls.
   Ball* ball1 = game->createBall ();
   ball1->m_sphere->m_position = glm::dvec3 (0., 0., 1.);
   //ball1->m_sphere->m_velocity.x = -0.01;
   ball1->m_sphere->m_rotationAxis = glm::dvec3 (0., 1., 0.);
   ball1->m_sphere->m_rotationVel = glm::radians (1.0);
   ball1->m_sphere->m_orientation = ball1->m_sphere->m_orientation * glm::dquat{ 0,1,0,0 };

   Ball* ball2 = game->createBall ();
   ball2->m_sphere->m_position = glm::dvec3 (0., 0., 2.);
   ball2->m_sphere->m_orientation = glm::dquat{ 0 ,1,0,0 };
   //ball2->m_sphere->m_velocity.x = -0.1;
   //ball2->m_mesh->m_color = glm::dvec3 (1.0, 0.5, 0.2);
   //ball2->m_mesh->m_useTexture = false;
   ball2->m_sphere->setMass (100.);

   DistanceConstraint* distC{ game->m_physicsEngine.createDistanceConstraint (ball1->m_sphere, ball2->m_sphere, 3.0) }; distC;
   //m_physicsEngine.removeDistanceContraint (distC);
   //AngleConstraint* ac{ m_physicsEngine.createAngleConstraint (ball1->m_sphere, ball2->m_sphere, 0, glm::dquat{1,0,0,0}, glm::dquat{0,1,0,0}) }; ac;
   //m_physicsEngine.removeAngleConstraint (ac);
   DirectionConstraint* dc{ game->m_physicsEngine.createDirectionConstraint (
      ball1->m_sphere, ball2->m_sphere, glm::dquat{0,1,0,0}, glm::dvec3{
         glm::half_pi<double> () * 1.0,
         glm::half_pi<double> () * 0.0,
         glm::half_pi<double> () * 1.0
      }, true
   ) }; dc;
   DirectionConstraint* dc2{ game->m_physicsEngine.createDirectionConstraint (
      ball2->m_sphere, ball1->m_sphere, glm::dquat{1,0,0,0}, glm::dvec3{
         glm::half_pi<double> () * 0.0,
         glm::half_pi<double> () * 0.0,
         glm::half_pi<double> () * 0.0
      }, true
   ) }; dc2;
   //m_physicsEngine.removeDirectionConstraint (dc);

   VisualConnection* vc = game->createVisualConnection (ball1, ball2, 0.5); vc;

   for (size_t ii = 0; ii < 500; ii++) {
      //game->createBall ();
   }

   game->m_graphicsEngine.m_cameraPos.z = 4.0;
}

int main () {
   // https://randomascii.wordpress.com/2013/07/16/floating-point-determinism/
   // https://stackoverflow.com/questions/6867693/change-floating-point-rounding-mode
   const int currentRoundingMode = fegetround ();
   if (currentRoundingMode != FE_TONEAREST) {
      fesetround (FE_TONEAREST);
   }

   //Game* game = new Game{};

   // Configure:
   //configure (game);


   //

   //game->run ();
   std::string asd{"asdf"};
   if (asd == 2) {
      int a = 0; a;
   }

   //delete game;

   Evolution* evolution = new Evolution{};
   
   evolution->run ();

#ifdef  TIME_FUNCTIONS
   std::cout << "Timer result:\n" << DebugTimer::getResultsString () << std::endl;
#endif //  TIME_FUNCTIONS

   return 0;
}




/*{
   std::vector<int> v{};
   for (size_t ii = 0; ii < 1e2; ii++) {
      v.push_back ((int)ii);
   }
   DebugTimer t{ "test" };
   for (size_t ii = 0; ii < 1e5; ii++) {
      for (size_t jj = 0; jj < v.size (); jj++) {
         int a = v[jj];
         a++;
      }
   }
}
{
   std::unordered_map<int, int> v{};
   for (size_t ii = 0; ii < 1e2; ii++) {
      v[(int)ii] = (int)ii;
   }
   DebugTimer t{ "test2" };
   for (size_t ii = 0; ii < 1e5; ii++) {
      for (auto& it : v) {
         int a = it.first;
         a++;
      }
   }
}*/

