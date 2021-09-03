#pragma once

#include <vector>
#include <unordered_map>

#include "Sphere.h"
#include "DistanceConstraint.h"
#include "AngleConstraint.h"
#include "DirectionConstraint.h"
#include "SphereWrapper.h"


class SphereSphereCollisionInfo {
private:
public:
   SphereWrapper* m_sphereA{};
   SphereWrapper* m_sphereB{};
   int m_collisionTime{ 0 };
   double m_normalCompensation{ 0.0 };
   SphereSphereCollisionInfo () {}
   SphereSphereCollisionInfo (SphereWrapper* sphereA, SphereWrapper* sphereB, int collisionTime) {
      m_sphereA = sphereA;
      m_sphereB = sphereB;
      m_collisionTime = collisionTime;
   }
};

class SphereGroundCollisionInfo {
private:
public:
   SphereWrapper* m_sphere{};
   int m_collisionTime{ 0 };
   double m_normalCompensation{ 0.0 };
   SphereGroundCollisionInfo (){ }
   SphereGroundCollisionInfo (SphereWrapper* sphere) {
      m_sphere = sphere;
   }
};

class PhysicsEngine {
private:
   std::vector<SphereWrapper*> m_spheres{};
   int m_totalNumSpheres{ 0 };
   std::vector<DistanceConstraint*> m_distanceConstraints{};
   std::vector<AngleConstraint*> m_angleConstraints{};
   std::vector<DirectionConstraint*> m_directionConstraints{};

   // Keys are the ids of the colliding spheres.
   std::unordered_map<uint64_t, SphereGroundCollisionInfo> m_sphereGroundCollisionInfo{};
   std::unordered_map<uint64_t, SphereSphereCollisionInfo> m_sphereSphereCollisionInfo{};
public:
   bool m_isGround{ false };
   bool m_enableResolveOverlap{ true };
   uint64_t m_time{ 0 };

   ~PhysicsEngine ();
   glm::dvec3 getTotalMomentum ();
   glm::dvec3 getTotalAngularMomentum ();
   double calcForceSphereGround (Sphere* sphere, bool doBounce);
   double calculateForceSphereSphere (Sphere* aa, Sphere* bb, glm::dvec3* normalOut, bool doBounce);
   void collisionDetection ();
   void resolveOverlap ();
   void resolveMomentum (bool useCompensator, double compensatorFactor);
   void handleAcceleration (double factor);
   void measureMovementError ();
   void handleMovement ();
   void savePreviousPositions ();
   void simulate ();
   SphereWrapper* createSphere ();
   void removeSphere (Sphere* sphere);
   DistanceConstraint* createDistanceConstraint (Sphere* sA, Sphere* sB, double distance);
   void removeDistanceContraint (DistanceConstraint* distanceConstraint);
   AngleConstraint* createAngleConstraint (
      Sphere* sA, Sphere* sB, double zConstraint, glm::dquat offsetA, glm::dquat offsetB
   );
   void removeAngleConstraint (AngleConstraint* angleConstraint);
   DirectionConstraint* createDirectionConstraint (
      Sphere* sA, Sphere* sB, glm::dquat offset, glm::dvec3 constraint, bool totalLock
   );
   void removeDirectionConstraint (DirectionConstraint* directionConstraint);
};

