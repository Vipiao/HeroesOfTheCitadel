

#include "Game.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "MeshGenerator.h"
#include "AssimpLoader.h"
#include "DebugInfo.h"

//void framebuffer_size_callback (GLFWwindow* window, int width, int height);
//void processInput (GLFWwindow* window);

// settings

void Game::processInput (GLFWwindow* window) {
   // Quit application.
   if (glfwGetKey (window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose (window, true);
   }

   //
   m_keyboardControl.update (window);

   // Mouse control calculations.
   m_mouseControl.update ();
   //std::cout << "TimeDown: " << m_mouseControl.m_mouseTimeDown << std::endl;
   //std::cout << "TimeUp: " << m_mouseControl.m_mouseTimeUp << std::endl << std::endl;

   /*// Camera controls.
   const glm::dvec3 right = m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 };
   const glm::dvec3 forward = m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 };
   const glm::dvec3 up = m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 };
   // -Translate.
   const double cameraSpeed = 0.1 * m_graphicsEngine.m_cameraSpeed;
   if (m_keyboardControl.m_w->m_isDown) {
      m_graphicsEngine.m_cameraPos += forward * cameraSpeed;
   }
   if (m_keyboardControl.m_s->m_isDown) {
      m_graphicsEngine.m_cameraPos -= forward * cameraSpeed;
   }
   if (m_keyboardControl.m_d->m_isDown) {
      m_graphicsEngine.m_cameraPos += right * cameraSpeed;
   }
   if (m_keyboardControl.m_a->m_isDown) {
      m_graphicsEngine.m_cameraPos -= right * cameraSpeed;
   }
   if (m_keyboardControl.m_space->m_isDown) {
      m_graphicsEngine.m_cameraPos += up * cameraSpeed;
   }
   if (m_keyboardControl.m_leftShift->m_isDown) {
      m_graphicsEngine.m_cameraPos -= up * cameraSpeed;
   }
   // Accelerate.
   if (m_keyboardControl.m_c->m_isDown) {
      m_graphicsEngine.m_cameraSpeed *= 1.1;
   }
   if (m_keyboardControl.m_v->m_isDown) {
      m_graphicsEngine.m_cameraSpeed /= 1.1;
   }
   // -Rotate keyboard.
   constexpr double cameraRotationSpeed = 0.05;
   if (m_keyboardControl.m_up->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 1,0,0 }
      );
   }
   if (m_keyboardControl.m_down->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 1,0,0 }
      );
   }
   if (m_keyboardControl.m_right->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 0,0,1 }
      );
   }
   if (m_keyboardControl.m_left->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 0,0,1 }
      );
   }
   if (m_keyboardControl.m_q->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 0,1,0 }
      );
   }
   if (m_keyboardControl.m_e->m_isDown) {
      m_graphicsEngine.m_cameraOrientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 0,1,0 }
      );
   }
   // -Rotate mouse.
   if (m_mouseIsLocked) {
      constexpr double cameraRotationSpeedMouse = 0.0007;
      double moveSize = cameraRotationSpeedMouse * glm::length (m_mouseControl.m_mouseMovement);
      glm::dvec3 axis{ -m_mouseControl.m_mouseMovement.y,0,-m_mouseControl.m_mouseMovement.x };
      if (moveSize > 0.0) {
         m_graphicsEngine.m_cameraOrientation = glm::rotate (
            m_graphicsEngine.m_cameraOrientation,
            moveSize,
            axis
         );
      }
   }

   // Speed up/down time.
   if (m_keyboardControl.m_keyPadMinus->clicked ()) {
      m_numSkipPhysics *= 2;
   }
   if (m_keyboardControl.m_keyPadPlus->clicked ()) {
      m_numSkipPhysics /= 2;
   }
   if (m_numSkipPhysics < 1) {
      m_numSkipPhysics = 1;
   }
   if (m_keyboardControl.m_keyPad0->clicked()) {
      m_pausePhysics = !m_pausePhysics;
   }

   //
   if (m_keyboardControl.m_p->clicked()) {
      if (m_mouseIsLocked) {
         m_mouseIsLocked = false;
         //glfwSetInputMode (m_graphicsEngine.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         m_graphicsEngine.setMouseLock (false);
      } else {
         m_mouseIsLocked = true;
         //glfwSetInputMode (m_graphicsEngine.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         m_graphicsEngine.setMouseLock (true);
      }
   }*/
   //
   if (m_time % 60 == 0) {

   }
}

void Game::applyTools () {
   /*// Detect next ball to be dragged.
   glm::dvec3 forward = m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 };
   if (m_mouseControl.clicked() || m_keyboardControl.m_f->clicked()) {
      if (m_dragBall == nullptr) {
         // No ball selected. Find new one to drag.
         double closestAngle = std::numeric_limits<double>::max ();
         m_dragBall = nullptr;
         for (size_t ii = 0; ii < m_balls.size (); ii++) {
            Ball* b = m_balls[ii];
            glm::dvec3 relBallPos = b->m_sphere->m_position - m_graphicsEngine.m_cameraPos;
            //double dist = glm::length (relBallPos - forward * glm::dot (forward, relBallPos));
            double angle = 1.0 - glm::dot (forward, glm::normalize (relBallPos));
            if (angle < closestAngle && glm::dot (forward, relBallPos) > 0) {
               closestAngle = angle;
               m_dragBallDist = glm::length(relBallPos);
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
      m_dragBallDist += m_mouseControl.m_scrollMovement * 0.05 * m_dragBallDist;

      // Shift position.
      glm::dvec3 targetPos = m_graphicsEngine.m_cameraPos + forward * m_dragBallDist;
      m_dragBall->m_sphere->m_position = targetPos;

      // Set velocity.
      if (m_grabTime > 0) {
         glm::dvec3 diff = targetPos - m_prevTargetPos;
         m_dragBall->m_sphere->m_velocity = diff;

      }
      m_prevTargetPos = targetPos;

      // Correct position by velicity.
      m_dragBall->m_sphere->m_position -= m_dragBall->m_sphere->m_velocity;

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
      if (m_keyboardControl.m_home->m_isDown) {
         angVel -= m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 } *acceleration *
            (m_keyboardControl.m_home->m_timeDown * scale + 1.0);
      }
      if (m_keyboardControl.m_end->m_isDown) {
         angVel += m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 } *acceleration *
            (m_keyboardControl.m_end->m_timeDown * scale + 1.0);
      }
      if (m_keyboardControl.m_delete->m_isDown) {
         angVel -= m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 } *acceleration *
            (m_keyboardControl.m_delete->m_timeDown * scale + 1.0);
      }
      if (m_keyboardControl.m_pageDown->m_isDown) {
         angVel += m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 } *acceleration *
            (m_keyboardControl.m_pageDown->m_timeDown * scale + 1.0);
      }
      if (m_keyboardControl.m_pageUp->m_isDown) {
         angVel += m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 } *acceleration *
            (m_keyboardControl.m_pageUp->m_timeDown * scale + 1.0);
      }
      if (m_keyboardControl.m_insert->m_isDown) {
         angVel -= m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 } *acceleration *
            (m_keyboardControl.m_insert->m_timeDown * scale + 1.0);
      }
      m_dragBall->m_sphere->m_rotationAxis = angVel;
      m_dragBall->m_sphere->m_rotationVel = glm::length (angVel);

      //
      m_grabTime++;
   }*/
}

void Game::runDebugRender () {
   //std::vector<Mesh*> debugMeshes{};
   for (size_t ii = 0; ii < DebugInfo::vectors.size(); ii++) {
      DebugVector* v = &DebugInfo::vectors[ii];
      Mesh* m = m_graphicsEngine.createMesh(m_arrowModel);
      constexpr double width = 0.3;
      double lengthOfVec = glm::length (v->m_direction);
      m->m_scale = width * glm::dvec3{ v->m_size, lengthOfVec / width, v->m_size };
      m->m_position = v->m_startPosition;
      //glm::angleAxis (b->m_rotationVel, b->m_rotationAxis)* b->m_orientation;
      glm::dvec3 up;
      if (abs(v->m_direction.x) > abs(v->m_direction.y)) {
         up = glm::dvec3{ 0,1,0 };
      } else {
         up = glm::dvec3{ 1,0,0 };
      }
      m->m_orientation = glm::quatLookAt (glm::normalize(v->m_direction), up);
      m->m_orientation = glm::rotate (m->m_orientation, glm::radians (-90.0), glm::dvec3 (1.0, 0.0, 0.0));
      m->m_useTexture = false;
      m->m_color = v->m_color;
      
      m_removeMeshQueue.push_back (m);
   }
   if (!m_pausePhysics && (m_time+1) % m_numSkipPhysics == 0) {
      DebugInfo::vectors.clear ();
   }
}

Game::Game () {

   //
   m_graphicsEngine.m_cameraPos = glm::dvec3 (0,-10,1);

   //

   // Load models.
   // -Sphere.
   {
      //std::vector<double> vertexData;
      //std::vector<int> indices;
      //MeshGenerator::sphere (&vertexData, &indices, 4, 1.0);
      //m_sphereModel = m_graphicsEngine.createModel (&vertexData, &indices);

      m_sphereModel = m_graphicsEngine.createModel (
         "..\\resources\\models\\debug_model.obj",
         "..\\resources\\textures\\debug_texture.jpg"
      );
   }
   // -Ground plane.
   {
      //std::vector<double> vertexData;
      //std::vector<int> indices;
      //MeshGenerator::plane (&vertexData, &indices);
      //m_groundModel = m_graphicsEngine.createModel (&vertexData, &indices, "");
      //Mesh* mesh = m_graphicsEngine.createMesh (m_groundModel);
      //mesh->m_scale = glm::dvec3{ 1000.0 };
      //mesh->m_useTexture = false;
      //mesh->m_color = glm::dvec3 (0.5,1,1);
      m_groundModel = m_graphicsEngine.createModel (
         "..\\resources\\models\\large_grass_plane.obj",
         "..\\resources\\textures\\grass_texture.jpg"
      );
      Mesh* mesh = m_graphicsEngine.createMesh (m_groundModel);
      mesh->m_scale = glm::dvec3{ 1000.0 };
   }

   // Crosshair.
   {
      std::vector<double> vertexData;
      std::vector<int> indices;
      MeshGenerator::crosshair (&vertexData, &indices);
      Model* model = m_graphicsEngine.createModel (&vertexData, &indices, "");
      //model->m_renderMode |= Model::RENDER_MODE_SELF_ILLUMINATION;
      model->m_selfIllumination = true;
      m_crosshair = m_graphicsEngine.createMesh (model);
      m_crosshair->m_color = glm::dvec3{ 48, 255, 255 } / 255.0;
      m_crosshair->m_scale = glm::dvec3{ 0.0004 };
   }

   // Debug arrows.
   {
      m_arrowModel = m_graphicsEngine.createModel (
         "..\\resources\\models\\arrow.obj", ""
      );
   }

   // Cylinder.
   {
      m_cylinderModel = m_graphicsEngine.createModel (
         "..\\resources\\models\\cylinder.obj", ""
      );
   }
}

Game::~Game () {
   // Remove balls.
   for (size_t ii = 0; ii < m_balls.size (); ii++) {
      delete m_balls[ii];
   }
   m_balls.clear ();

   // Remove visual connections.
   for (size_t ii = 0; ii < m_visualConnections.size (); ii++) {
      delete m_visualConnections[ii];
   }
   m_visualConnections.clear ();
}

void Game::run () {

   // render loop
   // -----------
   double start{ 0 };
   double largestFrameTime{ 0 };
   while (!glfwWindowShouldClose (m_graphicsEngine.m_window)) {
      // DEBUG START.
      /*glm::dquat ooRef = m_balls[1]->m_sphere->m_orientation;
      glm::dquat oo = m_balls[0]->m_sphere->m_orientation * glm::conjugate (ooRef);
      double angle = 2.0 * acos (oo.w);
      glm::dvec3 axis;
      if (oo.w == 1.0) {
         axis = glm::dvec3{ 0,0,0 };
      } else {
         axis = {
            oo.x / sqrt (1.0 - oo.w * oo.w),
            oo.y / sqrt (1.0 - oo.w * oo.w),
            oo.z / sqrt (1.0 - oo.w * oo.w),
         };
      }
      double ll = glm::length (axis);
      if (ll > 0.0) {
         axis *= angle / ll;
      }
      DebugInfo::vectors.push_back (DebugVector{ m_balls[0]->m_sphere->m_position + glm::dvec3{0,0,3}, axis });
      if (m_time % 30 == 0) {
         std::cout << "angle: " << angle << std::endl;
      }
      if (m_keyboardControl.m_f->clicked()) {
         std::cout << "!!!" << std::endl;
         if (axis.x != 0 || axis.y != 0 || axis.z != 0) {
            axis = glm::normalize (axis);
         } else {
            axis = glm::dvec3{ 1,0,0 };
         }
         m_balls[0]->m_sphere->m_orientation = glm::angleAxis (angle, axis);
      }*/
      //if (m_time%30 == 0) {
      //   std::cout << "Angular momentum: " <<
      //      "x: " << m_physicsEngine.getTotalAngularMomentum ().x << std::endl <<
      //      "y: " << m_physicsEngine.getTotalAngularMomentum ().y << std::endl <<
      //      "z: " << m_physicsEngine.getTotalAngularMomentum ().z << std::endl <<
      //      std::endl;
      //   std::cout << "Momentum: " <<
      //      "x: " << m_physicsEngine.getTotalMomentum ().x << std::endl <<
      //      "y: " << m_physicsEngine.getTotalMomentum ().y << std::endl <<
      //      "z: " << m_physicsEngine.getTotalMomentum ().z << std::endl <<
      //      std::endl;
      //   std::cout << std::endl;
      //}
      //std::cout << "time: " << m_time << std::endl;
      //std::cout << "d1: " << m_balls[0]->m_sphere->m_position.x << std::endl;
      //std::cout << "d2: " << m_balls[0]->m_mesh->m_position.x << std::endl;
      //std::cout << "d3: " << m_balls[0]->m_sphere->m_orientation.w << std::endl;
      //std::cout << "d4: " << m_balls[0]->m_mesh->m_orientation.w << std::endl;
      if (m_time == 1170) {
         
      }
      //DEBUG END.

      // Time a frame.
      double frameTime = glfwGetTime () - start;
      if (frameTime > largestFrameTime) {
         largestFrameTime = frameTime;
      }
      if (m_time%60==0) {
         //std::cout << "Largest frame time (ms): " << largestFrameTime*1000. << std::endl;
         largestFrameTime = 0.;
      }
      start = glfwGetTime ();
      // input
      // -----
      processInput (m_graphicsEngine.m_window);

      // Callback.
      if (updateCallbackPointer != nullptr) {
         updateCallbackPointer->updateCallback ();
      }

      // Tools.
      applyTools ();

      // Run physics.
      if (!m_pausePhysics && m_time % m_numSkipPhysics == 0) {
         m_physicsEngine.simulate ();
      }

      // render
      // Bind physics and graphics.
      for (size_t ii = 0; ii < m_balls.size (); ii++) {
         Ball* ball = m_balls[ii];
         ball->m_mesh->m_position = ball->m_sphere->m_position;
         ball->m_mesh->m_orientation = ball->m_sphere->m_orientation;
         ball->m_mesh->m_scale = glm::dvec3{ ball->m_sphere->getRadius () };
      }
      for (size_t ii = 0; ii < m_visualConnections.size(); ii++) {
         VisualConnection* vc = m_visualConnections[ii];
         Sphere* aa = vc->aa->m_sphere;
         Sphere* bb = vc->bb->m_sphere;
         glm::dvec3 center = (aa->m_position + bb->m_position) * 0.5;
         vc->m_mesh->m_position = center;
         glm::dvec3 diff{ bb->m_position - aa->m_position };
         double scale = glm::length (diff);
         vc->m_mesh->m_scale.y = scale * 0.5;
         glm::dquat orientation{ glm::lookAt (
            aa->m_position, bb->m_position,
            abs(diff.z) > abs(diff.x)? glm::dvec3{1,0,0} : glm::dvec3{0,0,1}
         ) };
         orientation = glm::conjugate (orientation) * glm::angleAxis(glm::radians(-90.0), glm::dvec3(1,0,0));
         vc->m_mesh->m_orientation = orientation;
         
         vc->m_mesh->m_scale.x = vc->radius;
         vc->m_mesh->m_scale.z = vc->radius;
      }

      // Adjust crosshair.
      m_crosshair->m_orientation = glm::rotate (
         m_graphicsEngine.m_cameraOrientation, glm::radians (90.0), glm::dvec3 (1, 0, 0)
         //m_graphicsEngine.m_cameraOrientation, glm::pi<double>()/4., glm::dvec3 (1, 0, 0)
      );
      m_crosshair->m_position =
         m_graphicsEngine.m_cameraPos +
         m_graphicsEngine.m_cameraOrientation *
         glm::dvec3{ 0, m_graphicsEngine.m_nearPlane + 1e-3, 0 };

      // ------

      runDebugRender ();

      if (m_time >= fastForwardTo) {
         m_graphicsEngine.render (m_time);
      }

      for (size_t ii = 0; ii < m_removeMeshQueue.size(); ii++) {
         m_graphicsEngine.removeMesh (m_removeMeshQueue[ii]);
      }
      m_removeMeshQueue.clear ();

      glfwPollEvents ();

      m_time++;
   }
}

Ball* Game::createBall () {
   BallWrapper* newBall = new BallWrapper ();
   Sphere* newSphere = m_physicsEngine.createSphere ();
   Mesh* newMesh = m_graphicsEngine.createMesh (m_sphereModel);
   newBall->m_sphere = newSphere;
   newBall->m_mesh = newMesh;
   newBall->m_index = m_balls.size ();
   m_balls.push_back (newBall);

   return newBall;
}

void Game::removeBall (Ball* ball) {
#ifdef DEBUG_LEVEL_0
   if (m_balls.size () <= ball->m_index || m_balls[ball->m_index] != ball) {
      throw std::runtime_error ("ERROR: Ball index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // Remove visual connections.
   BallWrapper* ballW{ m_balls[ball->m_index] };
   while (ballW->m_visualConnections.size() > 0) {
      removeVisualConnection (ballW->m_visualConnections[0]);
   }

   // Move last element and overwrite its own place in the list.
   if (ball->m_index == m_balls.size () - 1) {
      m_balls.pop_back ();
   } else {
      BallWrapper* lastBall = m_balls.back ();
      m_balls.pop_back ();
      m_balls[ball->m_index] = lastBall;
      lastBall->m_index = ball->m_index;
   }

   m_physicsEngine.removeSphere (ball->m_sphere);
   m_graphicsEngine.removeMesh (ball->m_mesh);

   //
   delete ball;
}

VisualConnection* Game::createVisualConnection (Ball* aa, Ball* bb, double radius) {
   VisualConnection* newVisualConnection{ new VisualConnection () };
   newVisualConnection->m_index = m_visualConnections.size ();
   m_visualConnections.push_back (newVisualConnection);
   newVisualConnection->aa = aa;
   newVisualConnection->bb = bb;
   m_balls[aa->m_index]->m_visualConnections.push_back (newVisualConnection);
   m_balls[bb->m_index]->m_visualConnections.push_back (newVisualConnection);
   newVisualConnection->radius = radius;

   Mesh* newMesh = m_graphicsEngine.createMesh (m_cylinderModel);
   newVisualConnection->m_mesh = newMesh;
   newMesh->m_color = glm::dvec3{1,1,1};

   return newVisualConnection;
}

void Game::removeVisualConnection (VisualConnection* visualConnection) {
#ifdef DEBUG_LEVEL_0
   if (
      m_visualConnections.size () <= visualConnection->m_index ||
      m_visualConnections[visualConnection->m_index] != visualConnection
      ) {
      throw std::runtime_error ("ERROR: Visual connection index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // Remove from balls.
   std::vector<BallWrapper*> bws{
      m_balls[visualConnection->aa->m_index],
      m_balls[visualConnection->bb->m_index],
   };
   for (size_t jj = 0; jj < bws.size (); jj++) {
      bool found{ false };
      for (size_t ii = 0; ii < bws[jj]->m_visualConnections.size (); ii++) {
         const VisualConnection* vc = bws[jj]->m_visualConnections[ii];
         if (vc == visualConnection) {
            if (ii == bws[jj]->m_visualConnections.size () - 1) {
               bws[jj]->m_visualConnections.pop_back ();
            } else {
               VisualConnection* lastVisualConnection = bws[jj]->m_visualConnections.back ();
               bws[jj]->m_visualConnections.pop_back ();
               bws[jj]->m_visualConnections[ii] = lastVisualConnection;
            }
            found = true;
            break;
         }
      }
      if (!found) {
         throw std::runtime_error ("ERROR: Could not find reference to constraint in sphere.");
      }
   }

   // Move last element and overwrite its own place in the list.
   if (visualConnection->m_index == m_visualConnections.size () - 1) {
      m_visualConnections.pop_back ();
   } else {
      VisualConnection* lastVisualConnection = m_visualConnections.back ();
      m_visualConnections.pop_back ();
      m_visualConnections[visualConnection->m_index] = lastVisualConnection;
      lastVisualConnection->m_index = visualConnection->m_index;
   }

   m_graphicsEngine.removeMesh (visualConnection->m_mesh);

   //
   delete visualConnection;
}

DistanceConstraint* Game::createDistanceConstraint (Ball* sA, Ball* sB, double distance) {
   return m_physicsEngine.createDistanceConstraint(sA->m_sphere, sB->m_sphere, distance);
}

void Game::removeDistanceContraint (DistanceConstraint* distanceConstraint) {
   m_physicsEngine.removeDistanceContraint (distanceConstraint);
}

AngleConstraint* Game::createAngleConstraint (
   Ball* sA, Ball* sB, double zConstraint, glm::dquat offsetA, glm::dquat offsetB
) {
   return m_physicsEngine.createAngleConstraint(
      sA->m_sphere, sB->m_sphere, zConstraint, offsetA, offsetB
   );
}

void Game::removeAngleConstraint (AngleConstraint* angleConstraint) {
   m_physicsEngine.removeAngleConstraint (angleConstraint);
}

DirectionConstraint* Game::createDirectionConstraint (Ball* sA, Ball* sB, glm::dquat offset, glm::dvec3 constraint, bool totalLock) {
   return m_physicsEngine.createDirectionConstraint(sA->m_sphere, sB->m_sphere, offset, constraint, totalLock);
}

void Game::removeDirectionConstraint (DirectionConstraint* directionConstraint) {
   m_physicsEngine.removeDirectionConstraint (directionConstraint);
}




