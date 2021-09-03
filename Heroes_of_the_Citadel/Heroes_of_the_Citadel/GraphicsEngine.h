#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Model.h"
#include "Mesh.h"

class GraphicsEngine {
private:
   std::vector<Model*> m_models;
   std::vector<Mesh*> m_meshes;
   bool m_mouseIsLocked{ true };

   static void framebuffer_size_callback (GLFWwindow*, int width, int height);
   //static void cursor_position_callback (GLFWwindow* window, double xpos, double ypos);
   //static void mouse_button_callback (GLFWwindow* window, int button, int action, int mods);

public:
   glm::dvec3 m_sunDirection{-1,-1,-1};
   glm::dvec3 m_cameraPos{ 0.0, 0.0, 0.0 };
   glm::dquat m_cameraOrientation{ 1., 0., 0., 0. };
   double m_cameraSpeed = 1.0;
   GLFWwindow* m_window;
   //static glm::dvec2 mousePosition;
   //static bool m_mouseIsDown;
   //static glm::dvec2 prevMousePosition;
   //static bool prevMousePositionIsInitialized;
   double m_nearPlane{0.01};
   double m_farPlane{1000.0};

   unsigned int m_depthMapFBO1{};
   unsigned int m_depthMapFBO2{};
   unsigned int m_depthMap1{};
   unsigned int m_depthMap2{};

   GraphicsEngine ();
   ~GraphicsEngine ();
   void setMouseLock (bool doLock);
   void render (int time);
   Model* createModel (
      std::vector<double>* vertexData, std::vector<int>* indices, std::string texturePath
   );
   Model* createModel (
      std::string modelPath, std::string texturePath
   );
   Mesh* createMesh (Model* model);
   void removeMesh (Mesh* mesh);
};

