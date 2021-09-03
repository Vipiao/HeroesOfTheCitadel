#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Sphere.h"


class AngleConstraint {
private:
public:
   Sphere* m_sA{};
   Sphere* m_sB{};
   double m_zConstraint{};
   glm::dquat m_offsetA{};
   glm::dquat m_offsetB{};
   size_t m_index{ 0 };

   glm::dvec3 calculateTorque ();
   void constrainMomentum (bool useCompensator, double compensatorFactor);
   void constrainPosition ();


};



