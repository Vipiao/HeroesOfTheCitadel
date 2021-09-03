








#include "InputHandler.h"

#include <iostream>

void InputHandler::handleInput (Game* game) {
   //std::cout << "!!!!" << std::endl;
   // Camera controls.
   const glm::dvec3 right = game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 1,0,0 };
   const glm::dvec3 forward = game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,1,0 };
   const glm::dvec3 up = game->m_graphicsEngine.m_cameraOrientation * glm::dvec3{ 0,0,1 };
   // -Translate.
   const double cameraSpeed = 0.1 * game->m_graphicsEngine.m_cameraSpeed;
   if (game->m_keyboardControl.m_w->m_isDown) {
      game->m_graphicsEngine.m_cameraPos += forward * cameraSpeed;
   }
   if (game->m_keyboardControl.m_s->m_isDown) {
      game->m_graphicsEngine.m_cameraPos -= forward * cameraSpeed;
   }
   if (game->m_keyboardControl.m_d->m_isDown) {
      game->m_graphicsEngine.m_cameraPos += right * cameraSpeed;
   }
   if (game->m_keyboardControl.m_a->m_isDown) {
      game->m_graphicsEngine.m_cameraPos -= right * cameraSpeed;
   }
   if (game->m_keyboardControl.m_space->m_isDown) {
      game->m_graphicsEngine.m_cameraPos += up * cameraSpeed;
   }
   if (game->m_keyboardControl.m_leftShift->m_isDown) {
      game->m_graphicsEngine.m_cameraPos -= up * cameraSpeed;
   }
   // Accelerate.
   if (game->m_keyboardControl.m_c->m_isDown) {
      game->m_graphicsEngine.m_cameraSpeed *= 1.1;
   }
   if (game->m_keyboardControl.m_v->m_isDown) {
      game->m_graphicsEngine.m_cameraSpeed /= 1.1;
   }
   // -Rotate keyboard.
   constexpr double cameraRotationSpeed = 0.05;
   if (game->m_keyboardControl.m_up->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 1,0,0 }
      );
   }
   if (game->m_keyboardControl.m_down->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 1,0,0 }
      );
   }
   if (game->m_keyboardControl.m_right->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 0,0,1 }
      );
   }
   if (game->m_keyboardControl.m_left->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 0,0,1 }
      );
   }
   if (game->m_keyboardControl.m_q->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, -cameraRotationSpeed, glm::dvec3{ 0,1,0 }
      );
   }
   if (game->m_keyboardControl.m_e->m_isDown) {
      game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
         game->m_graphicsEngine.m_cameraOrientation, cameraRotationSpeed, glm::dvec3{ 0,1,0 }
      );
   }
   // -Rotate mouse.
   if (game->m_mouseIsLocked) {
      constexpr double cameraRotationSpeedMouse = 0.0007;
      double moveSize = cameraRotationSpeedMouse * glm::length (game->m_mouseControl.m_mouseMovement);
      glm::dvec3 axis{ -game->m_mouseControl.m_mouseMovement.y,0,-game->m_mouseControl.m_mouseMovement.x };
      if (moveSize > 0.0) {
         game->m_graphicsEngine.m_cameraOrientation = glm::rotate (
            game->m_graphicsEngine.m_cameraOrientation,
            moveSize,
            axis
         );
      }
   }

   // Speed up/down time.
   if (game->m_keyboardControl.m_keyPadMinus->clicked ()) {
      game->m_numSkipPhysics *= 2;
   }
   if (game->m_keyboardControl.m_keyPadPlus->clicked ()) {
      game->m_numSkipPhysics /= 2;
   }
   if (game->m_numSkipPhysics < 1) {
      game->m_numSkipPhysics = 1;
   }
   if (game->m_keyboardControl.m_keyPad0->clicked ()) {
      game->m_pausePhysics = !game->m_pausePhysics;
   }

   // Lock unlock mouse.
   if (game->m_keyboardControl.m_p->clicked ()) {
      if (game->m_mouseIsLocked) {
         game->m_mouseIsLocked = false;
         //glfwSetInputMode (m_graphicsEngine.m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
         game->m_graphicsEngine.setMouseLock (false);
      } else {
         game->m_mouseIsLocked = true;
         //glfwSetInputMode (m_graphicsEngine.m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         game->m_graphicsEngine.setMouseLock (true);
      }
   }
}
