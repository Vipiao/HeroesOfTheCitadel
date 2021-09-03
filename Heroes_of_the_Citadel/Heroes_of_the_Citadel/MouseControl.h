#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MouseControl {
private:
   bool m_prevMouseDown{ false };
   glm::dvec2 m_prevMousePos{ 0,0 };
   double m_prevScrollPos{ 0 };
public:
   static glm::dvec2 m_mousePosition;
   static bool m_isDown;
   bool m_firstUpdateCall{ false };
   glm::dvec2 m_mouseMovement{ 0,0 };
   int m_timeDown{ 0 };
   int m_timeUp{ 0 };
   static double m_scrollPosition;
   double m_scrollMovement{ 0 };

   MouseControl (GLFWwindow* window);
   static void mouse_button_callback (GLFWwindow* window, int button, int action, int mods);
   static void cursor_position_callback (GLFWwindow* window, double xpos, double ypos);
   static void scroll_callback (GLFWwindow* window, double xoffset, double yoffset);
   bool clicked ();
   void update ();
};

