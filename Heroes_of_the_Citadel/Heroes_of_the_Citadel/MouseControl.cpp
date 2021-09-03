










#include "MouseControl.h"

#include <iostream>

glm::dvec2 MouseControl::m_mousePosition{ 0,0 };
bool MouseControl::m_isDown{ false };
double MouseControl::m_scrollPosition{ 0 };

MouseControl::MouseControl (GLFWwindow* window) {
   glfwSetCursorPosCallback (window, cursor_position_callback);
   glfwSetMouseButtonCallback (window, mouse_button_callback);
   glfwSetScrollCallback (window, scroll_callback);
   glfwSetCursorPos (window, 0, 0);
}

void MouseControl::mouse_button_callback (GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
   m_isDown = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
}

void MouseControl::cursor_position_callback (GLFWwindow* /*window*/, double xpos, double ypos) {
   //std::cout << "xpos: " << xpos << std::endl << "ypos: " << ypos << std::endl << std::endl;
   MouseControl::m_mousePosition.x = xpos;
   MouseControl::m_mousePosition.y = ypos;
}

void MouseControl::scroll_callback (GLFWwindow* /*window*/, double /*xoffset*/, double yoffset) {
   //std::cout << yoffset << std::endl;
   m_scrollPosition += yoffset;
}

bool MouseControl::clicked () {
   return m_isDown && m_timeDown == 0;
}

void MouseControl::update () {
   if (m_firstUpdateCall) {
      m_firstUpdateCall = false;

      // Mouse movement.
      m_mouseMovement = glm::dvec2 (0, 0);

      // Scroll movement.
      m_scrollMovement = 0;
   } else {
      // Mouse movement.
      m_mouseMovement = m_mousePosition - m_prevMousePos;

      // Scroll movement.
      m_scrollMovement = m_scrollPosition - m_prevScrollPos;
   }
   m_prevMousePos = m_mousePosition;
   m_prevScrollPos = m_scrollPosition;

   // Mouse click.
   if (m_prevMouseDown) {
      if (m_isDown) {
         m_timeDown++;
      } else {
         m_timeUp = 0;
      }
   } else {
      if (m_isDown) {
         m_timeDown = 0;
      } else {
         m_timeUp++;
      }
   }
   m_prevMouseDown = m_isDown;
}
