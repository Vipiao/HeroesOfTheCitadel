#pragma once




#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Sphere {
private:
   static uint64_t m_totalNumSpheres;
   double m_radius{ 1 };
   double m_mass{ 4.0 / 3.0 * glm::pi<double> () * m_radius * m_radius * m_radius };
   double m_momentOfInertiaMultiplier{ 1.0 };

   void calculateMomentOfInertia ();
public:
   uint64_t m_id;
   glm::dvec3 m_position{ 0,0,0 };
   glm::dvec3 m_prevPosition{ 0,0,0 };
   glm::dvec3 m_velocity{ 0,0,0 };
   glm::dquat m_orientation{ 1,0,0,0 };
   glm::dvec3 m_rotationAxis{ 1,0,0 };
   double m_rotationVel{ 0 };
   double m_momentOfInertia{ 0 }; // Wille be calcuated in setRadius();
   size_t m_index{0};
   double m_frictionConstant{ 0.4 };
   double m_bouncingConstant{ 0.75 };

   Sphere ();
   void setRadius (double radius);
   void setRadiusDensity (double radius, double density);
   double getRadius ();
   void setMass (double mass);
   double getMass ();
   void setMomentOfInertiaMultiplier (double mm);
   double getMomentOfInertiaMultiplier ();
   glm::dvec3 getAngVel ();
};

