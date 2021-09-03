#pragma once

#include "Sphere.h"

class DistanceConstraint {
private:
public:
   Sphere* m_sA{};
   Sphere* m_sB{};
   double m_distance{ 0 };
   size_t m_index{ 0 };
   double m_compensator{ 0 };

   void measureMovementError ();
   double calculateForce (glm::dvec3* normalOut);
   void constrainMomentum (bool useCompensator, double compensatorFactor);
   void constrainPosition ();
};

