#pragma once

#include "ShaderProgram.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicsEngine.h"
#include "PhysicsEngine.h"
#include "BallWrapper.h"
#include "KeyboardControl.h"
#include "MouseControl.h"
#include "VisualConnection.h"
#include "UpdateCallback.h"

class Game {
private:
   unsigned int m_time{ 0 };
   std::vector< VisualConnection*> m_visualConnections{};
   Model* m_sphereModel{};
   Model* m_groundModel{};
   Model* m_arrowModel{};
   Model* m_cylinderModel{};
   //glm::dvec2 m_prevMousePos{ 0,0 };
   //glm::dvec2 m_mouseMovement{ 0,0 };
   //int m_mouseTimeDown{ 0 };
   //int m_mouseTimeUp{ 0 };
   //bool m_prevMouseDown{ false };
   Mesh* m_crosshair;
   std::vector<Mesh*> m_removeMeshQueue{};
   
   // DragTool.
   //Ball* m_dragBall{ nullptr };
   //double m_dragBallDist{ 0 };
   //glm::dvec3 m_prevTargetPos{};
   //int m_grabTime{ 0 };

   void processInput (GLFWwindow* window);
   void applyTools ();
   void runDebugRender ();
public:
   uint64_t fastForwardTo{0};
   std::vector< BallWrapper*> m_balls;
   bool m_mouseIsLocked{ true };
   int m_numSkipPhysics{ 1 };
   bool m_pausePhysics{ false };
   GraphicsEngine m_graphicsEngine;
   PhysicsEngine m_physicsEngine;
   MouseControl m_mouseControl{ m_graphicsEngine.m_window };
   KeyboardControl m_keyboardControl{};
   UpdateCallback* updateCallbackPointer {nullptr};

   Game ();
   ~Game ();
   void run ();
   Ball* createBall ();
   void removeBall (Ball* ball);
   VisualConnection* createVisualConnection (Ball* aa, Ball* bb, double radius);
   void removeVisualConnection (VisualConnection* ball);
   DistanceConstraint* createDistanceConstraint (Ball* sA, Ball* sB, double distance);
   void removeDistanceContraint (DistanceConstraint* distanceConstraint);
   AngleConstraint* createAngleConstraint (Ball* sA, Ball* sB, double zConstraint, glm::dquat offsetA, glm::dquat offsetB);
   void removeAngleConstraint (AngleConstraint* angleConstraint);
   DirectionConstraint* createDirectionConstraint (Ball* sA, Ball* sB, glm::dquat offset, glm::dvec3 constraint, bool totalLock);
   void removeDirectionConstraint (DirectionConstraint* directionConstraint);
};

