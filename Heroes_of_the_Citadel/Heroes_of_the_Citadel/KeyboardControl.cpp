




#include "KeyboardControl.h"

#include <iostream>

bool Button::clicked () {
   return m_isDown && m_timeDown == 0;
}

void KeyboardControl::update (GLFWwindow* window) {
   //if (glfwGetKey (window, GLFW_KEY_A) == GLFW_PRESS) {
   //   m_graphicsEngine.m_cameraPos -= right * cameraSpeed;
   //}

   // Time buttons.
   for (size_t ii = 0; ii < m_buttons.size(); ii++) {
      Button* b = &m_buttons[ii];
      if (glfwGetKey (window, b->m_keyCode) == GLFW_PRESS ) {
         if (b->m_isDown) {
            b->m_timeDown++; // Is down now and down last update.
         } else {
            b->m_timeDown = 0; // Is down now and up last update.
         }
         b->m_isDown = true;
      }else{
         if (b->m_isDown) {
            b->m_timeUp = 0; // Is up now and down last update.
         } else {
            b->m_timeUp++; // Is up now and up last update.
         }
         b->m_isDown = false;
      }
   }
}
