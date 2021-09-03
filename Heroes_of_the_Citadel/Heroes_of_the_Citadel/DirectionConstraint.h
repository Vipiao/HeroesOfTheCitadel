#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Sphere.h"

class DirectionConstraint {
private:
public:
   Sphere* m_sA{};
   Sphere* m_sB{};
   //glm::dvec3 m_direction{};
   glm::dquat m_offset{ 1,0,0,0 };
   glm::dvec3 m_constraint{};
   size_t m_index{ 0 };
   bool m_totalLock{ false };
   glm::dvec3 m_compensator{0,0,0};

   double calculateMomentOfInertia (glm::dvec3 axis);
   void calculateMomentOfInertia (
      glm::dvec3 axis, glm::dvec3* centerOfMassOut,
      double* translationalMomentOfInertiaAOut,
      double* translationalMomentOfInertiaBOut
   );
   void measureMovementError ();
   glm::dvec3 calculateTorque ();
   void constrainMomentum (bool useCompensator, double compensatorFactor);
   void constrainPosition ();
};

