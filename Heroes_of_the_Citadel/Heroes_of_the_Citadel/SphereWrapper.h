#pragma once

#include <vector>

#include "Sphere.h"
#include "DistanceConstraint.h"
#include "AngleConstraint.h"
#include "DirectionConstraint.h"

class SphereWrapper : public Sphere {
   friend class Sphere;
public:
   std::vector<DistanceConstraint*> m_distanceConstraints{};
   std::vector<AngleConstraint*> m_angleConstraints{};
   std::vector<DirectionConstraint*> m_directionConstraints{};
private:

};

