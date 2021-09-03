#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>


class Button {
private:
public:
   int m_keyCode{ 0 };
   bool m_isDown{ false };
   int m_timeDown{ 0 };
   int m_timeUp{ 0 };

   bool clicked ();
};

class KeyboardControl {
private:
   std::vector<Button> m_buttons{
      Button{ GLFW_KEY_W },
      Button{ GLFW_KEY_A },
      Button{ GLFW_KEY_S },
      Button{ GLFW_KEY_D },
      Button{ GLFW_KEY_UP },
      Button{ GLFW_KEY_DOWN },
      Button{ GLFW_KEY_RIGHT },
      Button{ GLFW_KEY_LEFT },
      Button{ GLFW_KEY_Q },
      Button{ GLFW_KEY_E },
      Button{ GLFW_KEY_P },
      Button{ GLFW_KEY_SPACE },
      Button{ GLFW_KEY_LEFT_SHIFT },
      Button{ GLFW_KEY_C },
      Button{ GLFW_KEY_V },
      Button{ GLFW_KEY_KP_SUBTRACT },
      Button{ GLFW_KEY_KP_ADD },
      Button{ GLFW_KEY_KP_0 },
      Button{ GLFW_KEY_F },
      Button{ GLFW_KEY_INSERT },
      Button{ GLFW_KEY_HOME },
      Button{ GLFW_KEY_PAGE_UP },
      Button{ GLFW_KEY_DELETE },
      Button{ GLFW_KEY_END },
      Button{ GLFW_KEY_PAGE_DOWN },
   };
public:
   void update(GLFWwindow* window);
   Button* m_w{ &m_buttons[0] };
   Button* m_a{ &m_buttons[1] };
   Button* m_s{ &m_buttons[2] };
   Button* m_d{ &m_buttons[3] };
   Button* m_up{ &m_buttons[4] };
   Button* m_down{ &m_buttons[5] };
   Button* m_right{ &m_buttons[6] };
   Button* m_left{ &m_buttons[7] };
   Button* m_q{ &m_buttons[8] };
   Button* m_e{ &m_buttons[9] };
   Button* m_p{ &m_buttons[10] };
   Button* m_space{ &m_buttons[11] };
   Button* m_leftShift{ &m_buttons[12] };
   Button* m_c{ &m_buttons[13] };
   Button* m_v{ &m_buttons[14] };
   Button* m_keyPadMinus{ &m_buttons[15] };
   Button* m_keyPadPlus{ &m_buttons[16] };
   Button* m_keyPad0{ &m_buttons[17] };
   Button* m_f{ &m_buttons[18] };
   Button* m_insert{ &m_buttons[19] };
   Button* m_home{ &m_buttons[20] };
   Button* m_pageUp{ &m_buttons[21] };
   Button* m_delete{ &m_buttons[22] };
   Button* m_end{ &m_buttons[23] };
   Button* m_pageDown{ &m_buttons[24] };
};

