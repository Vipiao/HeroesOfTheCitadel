







#include "Sphere.h"

uint64_t Sphere::m_totalNumSpheres{ 0 };

Sphere::Sphere () {
   m_id = m_totalNumSpheres++;
   calculateMomentOfInertia ();
}

void Sphere::calculateMomentOfInertia () {
   m_momentOfInertia = 2.0 / 5.0 * m_mass * m_radius * m_radius * m_momentOfInertiaMultiplier;
}

void Sphere::setRadius (double radius) {
   m_radius = radius;
   calculateMomentOfInertia ();
}

void Sphere::setRadiusDensity (double radius, double density) {
   m_radius = radius;
   m_mass = 4.0/3.0 * glm::pi<double>() * m_radius * m_radius * m_radius * density;
   calculateMomentOfInertia ();
}

double Sphere::getRadius () {
   return m_radius;
}

void Sphere::setMass (double mass) {
   m_mass = mass;
   calculateMomentOfInertia ();
}

double Sphere::getMass () {
   return m_mass;
}

void Sphere::setMomentOfInertiaMultiplier (double mm) {
   m_momentOfInertiaMultiplier = mm;
   calculateMomentOfInertia ();
}

double Sphere::getMomentOfInertiaMultiplier () {
   return m_momentOfInertiaMultiplier;
}

glm::dvec3 Sphere::getAngVel () {
   if (m_rotationVel == 0.0) {
      return glm::dvec3{ 0 };
   }
   return glm::normalize(m_rotationAxis) * m_rotationVel;
}
