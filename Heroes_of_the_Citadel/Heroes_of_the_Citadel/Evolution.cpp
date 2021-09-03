#include "Evolution.h"

#include <iostream>

#include "RagdollCreator.h"




Evolution::Evolution () {
   m_game = new Game ();
   m_game->updateCallbackPointer = this;

   // Configure.
   // Create balls.
   //Ball* ball1 = m_game->createBall ();
   //ball1->m_sphere->m_position = glm::dvec3 (0.1, 0., 3.);
   //ball1->m_sphere->m_velocity.x = -0.01;
   //ball1->m_sphere->m_rotationAxis = glm::dvec3 (0., 1., 0.);
   //ball1->m_sphere->m_rotationVel = glm::radians (1.0);
   //ball1->m_sphere->m_orientation = ball1->m_sphere->m_orientation * glm::dquat{ 0,1,0,0 };

   //Ball* ball2 = m_game->createBall ();
   //ball2->m_sphere->m_position = glm::dvec3 (0., 0., 2.);
   //ball2->m_sphere->m_orientation = glm::dquat{ 0 ,1,0,0 };
   //ball2->m_sphere->m_velocity.x = -0.1;
   //ball2->m_mesh->m_color = glm::dvec3 (1.0, 0.5, 0.2);
   //ball2->m_mesh->m_useTexture = false;
   //ball2->m_sphere->setMass (1.);

   //DistanceConstraint* distC{ m_game->m_physicsEngine.createDistanceConstraint (ball1->m_sphere, ball2->m_sphere, 3.0) }; distC;
   //m_physicsEngine.removeDistanceContraint (distC);
   //AngleConstraint* ac{ m_physicsEngine.createAngleConstraint (ball1->m_sphere, ball2->m_sphere, 0, glm::dquat{1,0,0,0}, glm::dquat{0,1,0,0}) }; ac;
   //m_physicsEngine.removeAngleConstraint (ac);
   //DirectionConstraint* dc{ m_game->m_physicsEngine.createDirectionConstraint (
   //   ball1->m_sphere, ball2->m_sphere, glm::dquat{0,1,0,0}, glm::dvec3{
   //      glm::half_pi<double> () * 1.0,
   //      glm::half_pi<double> () * 0.0,
   //      glm::half_pi<double> () * 1.0
   //   }, true
   //) }; dc;
   //DirectionConstraint* dc2{ m_game->m_physicsEngine.createDirectionConstraint (
   //   ball2->m_sphere, ball1->m_sphere, glm::dquat{1,0,0,0}, glm::dvec3{
   //      glm::half_pi<double> () * 0.0,
   //      glm::half_pi<double> () * 0.0,
   //      glm::half_pi<double> () * 0.0
   //   }, true
   //) }; dc2;
   //m_physicsEngine.removeDirectionConstraint (dc);

   //VisualConnection* vc = m_game->createVisualConnection (ball1, ball2, 0.5); vc;

   for (size_t ii = 0; ii < 500; ii++) {
      //m_game->createBall ()->m_sphere->setRadiusDensity(1.0, 1.0);
   }

   m_game->m_graphicsEngine.m_cameraPos.z = 4.0;

   RagdollCreator::createHuman (m_game, glm::dvec3{0,0,20}, 1.0);



   //
   //m_game->m_graphicsEngine.m_cameraOrientation = glm::dquat (glm::sqrt(0.5), -glm::sqrt(0.5), 0, 0);
   m_game->m_graphicsEngine.m_cameraPos = glm::dvec3{ -1,-2,3 };
   m_game->fastForwardTo = 1;
   m_game->m_numSkipPhysics = 0;
}

Evolution::~Evolution () {
   delete m_game;
}

void Evolution::run () {
   m_game->run ();
}

void Evolution::updateCallback () {
   m_inputHandler.handleInput (m_game);
   
   // Detect next ball to be dragged.
   glm::dvec3 forward = m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 };
   if (m_game->m_mouseControl.clicked () || m_game->m_keyboardControl.m_f->clicked ()) {
      if (m_dragBall == nullptr) {
         // No ball selected. Find new one to drag.
         double closestAngle = std::numeric_limits<double>::max ();
         m_dragBall = nullptr;
         for (size_t ii = 0; ii < m_game->m_balls.size (); ii++) {
            Ball* b = m_game->m_balls[ii];
            glm::dvec3 relBallPos = b->m_sphere->m_position - m_game->m_graphicsEngine.m_cameraPos;
            //double dist = glm::length (relBallPos - forward * glm::dot (forward, relBallPos));
            double angle = 1.0 - glm::dot (forward, glm::normalize (relBallPos));
            if (angle < closestAngle && glm::dot (forward, relBallPos) > 0) {
               closestAngle = angle;
               m_dragBallDist = glm::length (relBallPos);
               m_dragBall = b;
            }
         }
         m_grabTime = 0;
      } else {
         // A ball is already selected. Clicking now makes you unselect it.
         m_dragBall = nullptr;
      }
   }

   // Apply drag force.
   if (m_dragBall != nullptr) {
      // Move closer/further.
      m_dragBallDist += m_game->m_mouseControl.m_scrollMovement * 0.05 * m_dragBallDist;

      // Shift position.
      glm::dvec3 targetPos = m_game->m_graphicsEngine.m_cameraPos + forward * m_dragBallDist;
      //m_dragBall->m_sphere->m_position = targetPos;

      // Set velocity.
      if (m_grabTime > 0) {
         glm::dvec3 diff = targetPos - m_prevTargetPos;
         //m_dragBall->m_sphere->m_velocity = diff;
         m_dragBall->m_sphere->m_velocity +=
            (targetPos - m_dragBall->m_sphere->m_position) * 0.05 -
            m_dragBall->m_sphere->m_velocity * 0.1;

      }
      m_prevTargetPos = targetPos;

      // Correct position by velicity.
      //m_dragBall->m_sphere->m_position -= m_dragBall->m_sphere->m_velocity;

      // Slow rotation.
      double ll = m_dragBall->m_sphere->m_rotationVel;
      constexpr double pace{ 0.001 };
      if (ll < pace) {
         m_dragBall->m_sphere->m_rotationVel = 0;
      } else {
         //m_dragBall->m_sphere->m_rotationVel -= m_dragBall->m_sphere->m_rotationVel/ll*pace;
         m_dragBall->m_sphere->m_rotationVel *= 0.90;
      }

      // Add rotation.
      glm::dvec3 angVel = m_dragBall->m_sphere->getAngVel ();
      constexpr double acceleration = 0.003;
      constexpr double scale = 0.1;
      if (m_game->m_keyboardControl.m_home->m_isDown) {
         angVel -= m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 } *acceleration *
            (m_game->m_keyboardControl.m_home->m_timeDown * scale + 1.0);
      }
      if (m_game->m_keyboardControl.m_end->m_isDown) {
         angVel += m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 } *acceleration *
            (m_game->m_keyboardControl.m_end->m_timeDown * scale + 1.0);
      }
      if (m_game->m_keyboardControl.m_delete->m_isDown) {
         angVel -= m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 } *acceleration *
            (m_game->m_keyboardControl.m_delete->m_timeDown * scale + 1.0);
      }
      if (m_game->m_keyboardControl.m_pageDown->m_isDown) {
         angVel += m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 } *acceleration *
            (m_game->m_keyboardControl.m_pageDown->m_timeDown * scale + 1.0);
      }
      if (m_game->m_keyboardControl.m_pageUp->m_isDown) {
         angVel += m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 } *acceleration *
            (m_game->m_keyboardControl.m_pageUp->m_timeDown * scale + 1.0);
      }
      if (m_game->m_keyboardControl.m_insert->m_isDown) {
         angVel -= m_game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 } *acceleration *
            (m_game->m_keyboardControl.m_insert->m_timeDown * scale + 1.0);
      }
      m_dragBall->m_sphere->m_rotationAxis = angVel;
      m_dragBall->m_sphere->m_rotationVel = glm::length (angVel);

      //
      m_grabTime++;
   }
}
