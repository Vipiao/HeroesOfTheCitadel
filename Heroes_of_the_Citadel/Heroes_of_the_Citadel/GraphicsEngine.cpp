#include "GraphicsEngine.h"

#include <iostream>

#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include"MeshGenerator.h"
#include "GlobalConstants.h"
#include "Hash.h"

// settings
const unsigned int SCR_WIDTH = 1200; // 800;// 1728;
const unsigned int SCR_HEIGHT = 900;//600;// 972;

const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;

//glm::dvec2 GraphicsEngine::mousePosition{ 0,0 };
//bool GraphicsEngine::m_mouseIsDown{ false };
//glm::dvec2 GraphicsEngine::prevMousePosition{ 0,0 };
//bool GraphicsEngine::prevMousePositionIsInitialized{ false };

void GraphicsEngine::framebuffer_size_callback (GLFWwindow* /*window*/, int width, int height) {
   // make sure the viewport matches the new window dimensions; note that width and 
   // height will be significantly larger than specified on retina displays.
   glViewport (0, 0, width, height);
}

//void GraphicsEngine::cursor_position_callback (GLFWwindow* /*window*/, double xpos, double ypos) {
//   //std::cout << "xpos: " << xpos << std::endl << "ypos: " << ypos << std::endl << std::endl;
//   GraphicsEngine::mousePosition.x = xpos;
//   GraphicsEngine::mousePosition.y = ypos;
//}
//
//void GraphicsEngine::mouse_button_callback (GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
//   m_mouseIsDown = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
//}

GraphicsEngine::GraphicsEngine () {
   glfwInit ();
   glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
   glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   // glfw window creation
   // --------------------
   m_window = glfwCreateWindow (SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
   if (m_window == NULL) {
      glfwTerminate ();
      throw std::runtime_error ("ERROR: Failed to create GLFW window.");
   }
   glfwMakeContextCurrent (m_window);
   glfwSetFramebufferSizeCallback (m_window, framebuffer_size_callback);

   // Track cursor movement.
   //glfwSetCursorPosCallback (m_window, cursor_position_callback);
   //glfwSetMouseButtonCallback (m_window, mouse_button_callback);
   // -Disable mouse movement.
   glfwSetInputMode (m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   if (glfwRawMouseMotionSupported ()) {
      glfwSetInputMode (m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
   } else {
      glfwTerminate ();
      throw std::runtime_error ("ERROR: Raw mouse input is not supported.");
   }

   // glad: load all OpenGL function pointers
   // ---------------------------------------
   if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
      glfwTerminate ();
      throw std::runtime_error ("ERROR: Failed to initialize GLAD.");
   }

   // configure global opengl state
   // -----------------------------
   glEnable (GL_DEPTH_TEST);
   //glEnable (GL_CULL_FACE);

   //
   glfwSwapInterval (1);

   // Create shaddows.
   // -Create frame buffer.
   glGenFramebuffers (1, &m_depthMapFBO1);

   // -Create 2d texture.
   glGenTextures (1, &m_depthMap1);
   glBindTexture (GL_TEXTURE_2D, m_depthMap1);
   glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
      SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   // -Attach depth map and texture 1.
   glBindFramebuffer (GL_FRAMEBUFFER, m_depthMapFBO1);
   glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap1, 0);
   glDrawBuffer (GL_NONE);
   glReadBuffer (GL_NONE);
   glBindFramebuffer (GL_FRAMEBUFFER, 0);

   // -Create frame buffer 2.
   glGenFramebuffers (1, &m_depthMapFBO2);

   // -Create 2d texture.
   glGenTextures (1, &m_depthMap2);
   glBindTexture (GL_TEXTURE_2D, m_depthMap2);
   glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
      SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   // -Attach depth map and texture 2.
   glBindFramebuffer (GL_FRAMEBUFFER, m_depthMapFBO2);
   glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap2, 0);
   glDrawBuffer (GL_NONE);
   glReadBuffer (GL_NONE);
   glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

GraphicsEngine::~GraphicsEngine () {
   for (size_t ii = 0; ii < m_models.size (); ii++) {
      delete m_models[ii];
   }
   m_models.clear ();
   for (size_t ii = 0; ii < m_meshes.size (); ii++) {
      delete m_meshes[ii];
   }
   m_meshes.clear ();
   // optional: de-allocate all resources once they've outlived their purpose:
      // ------------------------------------------------------------------------

   // glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
   glfwTerminate ();
}

void GraphicsEngine::setMouseLock (bool doLock) {
   if (doLock && !m_mouseIsLocked) {
      m_mouseIsLocked = true;
      glfwSetInputMode (m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   } else if (!doLock && m_mouseIsLocked) {
      m_mouseIsLocked = false;
      glfwSetInputMode (m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   }
}

void GraphicsEngine::render (int time) {


   // View matrix.
   glm::dmat4 viewMatrix = glm::dmat4 (1.0);
   // note that we're translating the scene in the reverse direction of where we want to move
   viewMatrix = glm::rotate (viewMatrix, glm::radians (-90.0), glm::dvec3 (1.0, 0.0, 0.0));
   viewMatrix = viewMatrix * glm::toMat4 (glm::conjugate (m_cameraOrientation));
   viewMatrix = glm::translate (viewMatrix, -m_cameraPos);

   // Projection matrix.
   glm::dmat4 projectionMatrix;
   projectionMatrix =
      glm::perspective (glm::radians (90.0), (double)SCR_WIDTH / (double)SCR_HEIGHT, m_nearPlane, m_farPlane);

   // Shadow view matrix.
   constexpr double radius1 = 5.;
   constexpr double radius2 = 160.;
   //glm::dvec3 sunPos = m_cameraPos;
   glm::dvec3 randomShift{ glm::dvec3{
      Hash::pcgUnit ((uint64_t)(time) * 3),
      Hash::pcgUnit ((uint64_t)(time) * 3 + 1),
      Hash::pcgUnit ((uint64_t)(time) * 3 + 2),
   } *3. };
   glm::dvec3 camPosRandom{ m_cameraPos + randomShift };
   glm::dvec3 viewCenterPos = camPosRandom - glm::normalize (m_sunDirection) * radius2;
   glm::dmat4 shadowLightViewMatrix1 = glm::lookAt (viewCenterPos,
      viewCenterPos + m_sunDirection,
      glm::dvec3{ 0.0, 0.0, 1.0 });
   glm::dvec3 viewCenterPos2 = camPosRandom - glm::normalize (m_sunDirection) * radius2;
   glm::dmat4 shadowLightViewMatrix2 = glm::lookAt (viewCenterPos2,
      viewCenterPos2 + m_sunDirection,
      glm::dvec3{ 0.0, 0.0, 1.0 });

   // Shadow projection matrix.
   glm::dmat4 shadowLightProjectionMatrix1 = glm::ortho (-radius1, radius1, -radius1, radius1, 0.0, radius2 + radius1);
   //glm::dmat4 shadowLightProjectionMatrix = glm::ortho (-radius1, radius1, -radius1, radius1, 0.0, radius1 * 2.0);
   glm::dmat4 shadowLightProjectionMatrix2 = glm::ortho (-radius2, radius2, -radius2, radius2, 0.0, radius2 * 2.0);

   //
   //glm::dmat4 shadowLightSpaceMatrix = shadowLightProjectionMatrix * shadowLightViewMatrix;

   // Render shaddow map.
   glViewport (0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
   glBindFramebuffer (GL_FRAMEBUFFER, m_depthMapFBO1);
   glClear (GL_DEPTH_BUFFER_BIT);
   for (size_t ii = 0; ii < m_meshes.size (); ii++) {
      Mesh* mesh = m_meshes[ii];
      Model* model = mesh->m_model;
      glm::dmat4 modelMatrix = glm::dmat4 (1.0);
      modelMatrix = glm::translate (modelMatrix, mesh->m_position);
      modelMatrix = modelMatrix * glm::toMat4 (mesh->m_orientation);
      modelMatrix = glm::scale (modelMatrix, mesh->m_scale);

      model->renderShadow (modelMatrix, shadowLightViewMatrix1, shadowLightProjectionMatrix1);
   }

   // Render shaddow map 2.
   glViewport (0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
   glBindFramebuffer (GL_FRAMEBUFFER, m_depthMapFBO2);
   glClear (GL_DEPTH_BUFFER_BIT);
   for (size_t ii = 0; ii < m_meshes.size (); ii++) {
      Mesh* mesh = m_meshes[ii];
      Model* model = mesh->m_model;
      glm::dmat4 modelMatrix = glm::dmat4 (1.0);
      modelMatrix = glm::translate (modelMatrix, mesh->m_position);
      modelMatrix = modelMatrix * glm::toMat4 (mesh->m_orientation);
      modelMatrix = glm::scale (modelMatrix, mesh->m_scale);

      model->renderShadow (modelMatrix, shadowLightViewMatrix2, shadowLightProjectionMatrix2);
   }

   // Render meshes.
   glBindFramebuffer (GL_FRAMEBUFFER, 0);
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glViewport (0, 0, SCR_WIDTH, SCR_HEIGHT);
   glClearColor (0.2f, 0.3f, 0.3f, 1.0f);
   for (size_t ii = 0; ii < m_meshes.size (); ii++) {
      Mesh* mesh = m_meshes[ii];
      Model* model = mesh->m_model;
      glm::dmat4 modelMatrix = glm::dmat4 (1.0);
      modelMatrix = glm::translate (modelMatrix, mesh->m_position);
      modelMatrix = modelMatrix * glm::toMat4 (mesh->m_orientation);
      modelMatrix = glm::scale (modelMatrix, mesh->m_scale);

      //model->m_renderMode |= Model::RENDER_MODE_USE_TEXTURE;
      if (mesh->m_useTexture) {
         if (model->m_hasTexture) {
            model->m_useTextures = true;
         } else {
            throw std::runtime_error (
               "ERROR: Tried to use textures on model that does not have textures."
            );
         }
      } else {
         model->m_useTextures = false;
         model->m_color = mesh->m_color;
      }

      model->render (
         modelMatrix, viewMatrix, projectionMatrix, m_cameraPos,
         shadowLightProjectionMatrix1 * shadowLightViewMatrix1, m_depthMap1,
         shadowLightProjectionMatrix2 * shadowLightViewMatrix2, m_depthMap2
      );
   }

   // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
   // -------------------------------------------------------------------------------
   glfwSwapBuffers (m_window);
}

Model* GraphicsEngine::createModel (std::vector<double>* vertexData, std::vector<int>* indices, std::string texturePath) {
   Model* newModel = new Model ();
   newModel->loadGeometry (vertexData, indices, texturePath);
   m_models.push_back (newModel);
   
   return newModel;
}

Model* GraphicsEngine::createModel (std::string modelPath, std::string texturePath) {
   Model* newModel = new Model ();
   newModel->loadGeometry (modelPath, texturePath);
   m_models.push_back (newModel);

   return newModel;
}

Mesh* GraphicsEngine::createMesh (Model* model) {
   Mesh* newMesh = new Mesh ();
   newMesh->m_model = model;
   newMesh->m_index = m_meshes.size ();
   newMesh->m_useTexture = model->m_hasTexture;
   m_meshes.push_back (newMesh);

   return newMesh;
}

void GraphicsEngine::removeMesh (Mesh* mesh) {
#ifdef DEBUG_LEVEL_0
   if (m_meshes.size () <= mesh->m_index || m_meshes[mesh->m_index] != mesh) {
      throw std::runtime_error ("ERROR: Sphere index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0
   // Move last element and overwrite its own place in the list.
   if (m_meshes.size () -1 == mesh->m_index) {
      m_meshes.pop_back ();
   } else {
      Mesh* lastMesh = m_meshes.back ();
      m_meshes.pop_back ();
      m_meshes[mesh->m_index] = lastMesh;
      lastMesh->m_index = mesh->m_index;
   }
   delete mesh;
}


