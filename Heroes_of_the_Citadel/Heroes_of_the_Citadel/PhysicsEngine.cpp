#include "PhysicsEngine.h"

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "GlobalConstants.h"
#include "DebugInfo.h"
#include "DebugTimer.h"



PhysicsEngine::~PhysicsEngine () {
   // Delete spheres.
   for (size_t ii = 0; ii < m_spheres.size(); ii++) {
      delete m_spheres[ii];
   }
   m_spheres.clear ();

   // Delete distance constraints.
   for (size_t ii = 0; ii < m_distanceConstraints.size (); ii++) {
      delete m_distanceConstraints[ii];
   }
   m_distanceConstraints.clear ();

   // Delete angle constraints.
   for (size_t ii = 0; ii < m_angleConstraints.size (); ii++) {
      delete m_angleConstraints[ii];
   }
   m_angleConstraints.clear ();

   // Delete direction constraints.
   for (size_t ii = 0; ii < m_directionConstraints.size (); ii++) {
      delete m_directionConstraints[ii];
   }
   m_directionConstraints.clear ();
}

glm::dvec3 PhysicsEngine::getTotalMomentum () {
   glm::dvec3 totalMomentum{ 0,0,0 };
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* ss = m_spheres[ii];
      totalMomentum += ss->m_velocity * ss->getMass ();
   }
   return totalMomentum;
}

glm::dvec3 PhysicsEngine::getTotalAngularMomentum () {
   glm::dvec3 totalAngularMomentum{ 0,0,0 };
   for (size_t ii = 0; ii < m_spheres.size(); ii++) {
      SphereWrapper* ss = m_spheres[ii];
      totalAngularMomentum += ss->getAngVel () * ss->m_momentOfInertia;
      totalAngularMomentum += glm::cross(ss->m_position, ss->m_velocity) * ss->getMass();
   }
   return totalAngularMomentum;
}

double PhysicsEngine::calcForceSphereGround (Sphere* sphere, bool doBounce) {
   constexpr double bounceFriction = 0.6;
   double normalImpulse = -sphere->m_velocity.z * sphere->getMass ();
   if (doBounce) {
      normalImpulse *= 1.0 + bounceFriction;
   }
   //ss->m_velocity.z = -ss->m_velocity.z * bounceFriction;
   return normalImpulse;
}

double PhysicsEngine::calculateForceSphereSphere (
   Sphere* aa, Sphere* bb, glm::dvec3* normalOut, bool doBounce
) {
   //SphereSphereCollisionInfo* collisionInfo {&it.second};
   double distBetweenPos = glm::length (aa->m_position - bb->m_position);
   if (distBetweenPos > 0.0) { // They are not exactly inside each others centers.
      glm::dvec3 normal = (aa->m_position - bb->m_position) / distBetweenPos;
      *normalOut = normal;
      glm::dvec3 relVel = bb->m_velocity - aa->m_velocity;
      double relVelTowards = glm::dot (relVel, normal);
      // Normal forces.
      double collisionMass = 1.0 / (1.0 / aa->getMass () + 1.0 / bb->getMass ());
      double normalImpulse = relVelTowards * collisionMass;
      //if (collisionInfo->m_collisionTime == 0) {
      if (doBounce) {
         normalImpulse *= (1.0 + aa->m_bouncingConstant * bb->m_bouncingConstant);
      }

      //if (normalImpulse > 0.0) {
         glm::dvec3 impulse = normal * normalImpulse;
         return normalImpulse;
      //}
   }
   return 0.0;
}


void PhysicsEngine::collisionDetection () {
#ifdef TIME_FUNCTIONS
   DebugTimer timer{ "collisionDetection" };
#endif // TIME_FUNCTIONS

   constexpr double margin{ 0.01 };

   // Sphere ground.
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* ss = m_spheres[ii];
      uint64_t key = ss->m_id;
      if (ss->m_position.z < ss->getRadius () + margin) {
         // Collides with ground.
         if (m_sphereGroundCollisionInfo.find (key) == m_sphereGroundCollisionInfo.end ()) {
            // Did not find key, creat new instance.
            m_sphereGroundCollisionInfo[ss->m_id] = { ss };
         } else {
            m_sphereGroundCollisionInfo[ss->m_id].m_collisionTime++;
         }
         continue;
      }
      // Sphere did not collide, remove collision information if it exists.
      if (m_sphereGroundCollisionInfo.find (key) != m_sphereGroundCollisionInfo.end ()) {
         m_sphereGroundCollisionInfo.erase (key);
      }
   }
   // Sphere sphere.
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* ssA = m_spheres[ii];
      double posAX = ssA->m_position.x;
      double posAY = ssA->m_position.y;
      double posAZ = ssA->m_position.z;
      double posAPrevX = ssA->m_prevPosition.x;
      double posAPrevY = ssA->m_prevPosition.y;
      double posAPrevZ = ssA->m_prevPosition.z;
      double rA = ssA->getRadius () + margin;
      for (size_t jj = ii + 1; jj < m_spheres.size (); jj++) {
         SphereWrapper* ssB = m_spheres[jj];
         double posBX = ssB->m_position.x;
         double posBY = ssB->m_position.y;
         double posBZ = ssB->m_position.z;
         double posBPrevX = ssB->m_prevPosition.x;
         double posBPrevY = ssB->m_prevPosition.y;
         double posBPrevZ = ssB->m_prevPosition.z;
         double rB = ssB->getRadius () + margin;

         uint64_t key = (ssA->m_id << 32) + ssB->m_id;
         if (
            !(
            posAX + rA < posBX - rB && posAPrevX + rA < posBPrevX - rB ||
            posAX - rA > posBX + rB && posAPrevX - rA > posBPrevX + rB ||
            posAY + rA < posBY - rB && posAPrevY + rA < posBPrevY - rB ||
            posAY - rA > posBY + rB && posAPrevY - rA > posBPrevY + rB ||
            posAZ + rA < posBZ - rB && posAPrevZ + rA < posBPrevZ - rB ||
            posAZ - rA > posBZ + rB && posAPrevZ - rA > posBPrevZ + rB
            )
            ) {
            /*double rSum = ssA->getRadius () + ssB->getRadius ();
            double distSqr = glm::length2 (ssA->m_position - ssB->m_position);
            if (distSqr < rSum * rSum) {
               if (m_sphereSphereCollisionInfo.find(key) == m_sphereSphereCollisionInfo.end()) {
                  // Did not find key, creat new instance.
                  m_sphereSphereCollisionInfo[key] = { ssA, ssB };
               }
               continue;
            }*/
            glm::dvec3 v =
               ssB->m_position - ssB->m_prevPosition - (ssA->m_position - ssA->m_prevPosition);
            double len2V = glm::length2 (v);
            if (len2V > 0.0) {
               glm::dvec3 bRel0 = ssB->m_prevPosition - ssA->m_prevPosition;
               double t = -glm::dot (bRel0, v) / len2V;
               t = std::min(std::max(t, 0.0), 1.0);
               double closestDist2 = glm::length2(bRel0 + v*t);
               double rSum = ssA->getRadius () + margin + ssB->getRadius () + margin;
               if (closestDist2 < rSum * rSum) {
                  if (m_sphereSphereCollisionInfo.find (key) == m_sphereSphereCollisionInfo.end ()) {
                     // Did not find key, creat new instance.
                     m_sphereSphereCollisionInfo[key] = { ssA, ssB, 0 };
                  } else {
                     m_sphereSphereCollisionInfo[key].m_collisionTime++;
                  }
                  continue;
               }
            } else {
               // Spheres have zero relative velocity. Check for simple overlap.
               double rSum = ssA->getRadius () + margin + ssB->getRadius () + margin;
               double distSqr = glm::length2 (ssA->m_position - ssB->m_position);
               if (distSqr < rSum * rSum) {
                  if (m_sphereSphereCollisionInfo.find (key) == m_sphereSphereCollisionInfo.end ()) {
                     // Did not find key, creat new instance.
                     m_sphereSphereCollisionInfo[key] = { ssA, ssB, 0 };
                  } else {
                     m_sphereSphereCollisionInfo[key].m_collisionTime++;
                  }
                  continue;
               }
            }
         }
         // Spheres did not collide, remove collision information if it exists.
         if (m_sphereSphereCollisionInfo.find (key) != m_sphereSphereCollisionInfo.end ()) {
            m_sphereSphereCollisionInfo.erase (key);
         }
      }
   }
}

void PhysicsEngine::resolveOverlap () {
#ifdef TIME_FUNCTIONS
   DebugTimer timer{"resolveOverlap"};
#endif // TIME_FUNCTIONS

   // Distance constraints.
   for (size_t ii = 0; ii < m_distanceConstraints.size (); ii++) {
      m_distanceConstraints[ii]->constrainPosition ();
   }
   // Angle constraints.
   for (size_t ii = 0; ii < m_angleConstraints.size (); ii++) {
      m_angleConstraints[ii]->constrainPosition ();
   }
   // Direction constraints.
   for (size_t ii = 0; ii < m_directionConstraints.size (); ii++) {
      m_directionConstraints[ii]->constrainPosition ();
   }

   // Sphere ground.
   for (auto& it : m_sphereGroundCollisionInfo) {
      SphereWrapper* ss = it.second.m_sphere;

      // Shift to surface.
      double penetration{ ss->m_position.z - ss->getRadius () };
      double factor{ 0.5 };
      if (penetration > 0.0) {
         factor *= 0.001;
      }
      ss->m_position.z -= penetration;
   }

   // Sphere sphere.
   for (auto& it : m_sphereSphereCollisionInfo) {
      SphereWrapper* ssA = it.second.m_sphereA;
      SphereWrapper* ssB = it.second.m_sphereB;
      glm::dvec3 oldDir = ssB->m_prevPosition - ssA->m_prevPosition;
      glm::dvec3 newDir = ssB->m_position - ssA->m_position;
      if (glm::dot(oldDir, newDir) < 0.0) { // The balls passed each other.
         double sumOfMasses{ ssA->getMass () + ssB->getMass () };
         glm::dvec3 oldCenterOfMass =
            (ssA->m_prevPosition * ssA->getMass () + ssB->m_prevPosition * ssB->getMass ()) /
            sumOfMasses;
         glm::dvec3 newCenterOfMass =
            (ssA->m_position * ssA->getMass () + ssB->m_position * ssB->getMass ()) /
            sumOfMasses;
         glm::dvec3 diff = newCenterOfMass - oldCenterOfMass;
         ssA->m_position = ssA->m_prevPosition + diff;
         ssB->m_position = ssB->m_prevPosition + diff;
      } else {
         double distBetweenPos = glm::length (ssA->m_position - ssB->m_position);
         //DebugInfo::vectors.push_back (DebugVector{ ssA->m_position, ssB->m_position-ssA->m_position});
         if (distBetweenPos == 0 /*|| distBetweenPos > ssA->getRadius() + ssB->getRadius()*/) {
            continue;
         }
         double penetration = ssA->getRadius() + ssB->getRadius() - distBetweenPos;
         double factor = 0.5; // Resolving the overlap too quickly can cause jittering.
         if (distBetweenPos > ssA->getRadius () + ssB->getRadius ()) {
            factor *= 0.001;
         }
         glm::dvec3 shift =
            (ssA->m_position - ssB->m_position) / distBetweenPos * penetration * factor;
         double collisionMass = 1.0 / (1.0 / ssA->getMass () + 1.0 / ssB->getMass ());
         ssA->m_position += shift * collisionMass / ssA->getMass ();
         ssB->m_position -= shift * collisionMass / ssB->getMass ();
      }
   }
}

void PhysicsEngine::resolveMomentum (bool useCompensator, double compensatorFactor) {
#ifdef TIME_FUNCTIONS
   DebugTimer timer{ "resolveMomentum" };
#endif // TIME_FUNCTIONS
   if (m_time == 1107) {
      int a = 0; a;
   }
   // Distance constraints.
   for (size_t ii = 0; ii < m_distanceConstraints.size (); ii++) {
      //m_distanceConstraints[ii]->constrainMomentum ();
      m_distanceConstraints[ii]->constrainMomentum (useCompensator, compensatorFactor);
   }

   // Angle constraints.
   for (size_t ii = 0; ii < m_angleConstraints.size (); ii++) {
      m_angleConstraints[ii]->constrainMomentum (useCompensator, compensatorFactor);
   }

   // Direction constraints.
   for (size_t ii = 0; ii < m_directionConstraints.size (); ii++) {
      if (m_time == 413 && ii == 2) {
         int a = 0; a;
      }
      m_directionConstraints[ii]->constrainMomentum (useCompensator, compensatorFactor);
   }

   // Sphere ground.
   for (auto& it : m_sphereGroundCollisionInfo) {
      SphereWrapper* ss = it.second.m_sphere;
      // Apply forces.
      // Normal force.
      //constexpr double bounceFriction = 0.6;
      //double normalImpulse = -ss->m_velocity.z * ss->getMass () * (1.0 + bounceFriction);
      //ss->m_velocity.z = -ss->m_velocity.z * bounceFriction;
      double normalImpulse{ calcForceSphereGround (ss, it.second.m_collisionTime == 0) };
      if (useCompensator) {
         normalImpulse += it.second.m_normalCompensation * compensatorFactor;
      }
      if (normalImpulse > 0.0) {
         ss->m_velocity.z += normalImpulse / ss->getMass();
   
         // Friction force.
         glm::dvec3 groundVel = -ss->m_velocity;
         groundVel.z = 0.0;
         if (ss->m_rotationVel > 0) {
            glm::dvec3 axisUnit = glm::normalize (ss->m_rotationAxis);
            glm::dvec3 r = glm::dvec3 (0, 0, -ss->getRadius ()) -
               axisUnit * glm::dot (axisUnit, glm::dvec3 (0, 0, -ss->getRadius ()));
            groundVel -= glm::cross (axisUnit, r) * ss->m_rotationVel;
         }
         double collisionMass = 1.0 / (1.0 / ss->getMass () + ss->getRadius () / ss->m_momentOfInertia);
         glm::dvec3 impulse = collisionMass * groundVel;
         double maxImpulse = normalImpulse * ss->m_frictionConstant;
         if (maxImpulse > 0.0 && glm::length2 (impulse) > maxImpulse * maxImpulse) {
            impulse *= maxImpulse / glm::length (impulse);
         }
         ss->m_velocity += impulse / ss->getMass ();
         glm::dvec3 angVel = ss->getAngVel ();
         glm::dvec3 torque = glm::cross (glm::dvec3{ 0,0,-ss->getRadius () }, impulse);
         angVel += torque / ss->m_momentOfInertia;
         ss->m_rotationAxis = angVel;
         ss->m_rotationVel = glm::length (angVel);

         // Rolling friction.
         double velLength2 = glm::length2 (ss->m_velocity);
         if (velLength2 > 0.0) {
            glm::dvec3 rollingFriction{ -ss->m_velocity / glm::sqrt(velLength2) * normalImpulse * 0.01 };
            glm::dvec3 deltaV{ rollingFriction / ss->getMass () };
            if (velLength2 < glm::length2(deltaV)) {
               ss->m_velocity.x = ss->m_velocity.y = 0.0;
            } else {
               ss->m_velocity += deltaV;
            }
         }
         //ss->m_rotationVel *= 0.99;
         //DebugInfo::vectors.push_back (DebugVector{ ss->m_position + glm::dvec3 {0, 0, -ss->getRadius () }, torque*100. });
         //DebugInfo::vectors.push_back (DebugVector{ ss->m_position + glm::dvec3 {2, 0, 0.0 }, impulse * 1000. });
      }
   }

   if (m_time == 1107) {
      int a = 0; a;
   }
   // Sphere sphere.
   for (auto& it : m_sphereSphereCollisionInfo) {
      if (m_time == 695) {
         int a = 0; a;
      }
      SphereWrapper* aa = it.second.m_sphereA;
      SphereWrapper* bb = it.second.m_sphereB;

      glm::dvec3 normal{};
      double normalImpulse{calculateForceSphereSphere(aa, bb, &normal, it.second.m_collisionTime == 0)};
      if (useCompensator) {
         normalImpulse += it.second.m_normalCompensation * compensatorFactor;
      }
      glm::dvec3 impulse{ normal * normalImpulse };

      if (normalImpulse > 0.0) {
         aa->m_velocity += impulse / aa->getMass ();
         bb->m_velocity -= impulse / bb->getMass ();

         // Friction.
         // Friction forces.
            // -Calculate surface velocity.
         glm::dvec3 relVel = bb->m_velocity - aa->m_velocity;
         glm::dvec3 relVelSurface = relVel;
         glm::dvec3 contactPoint = (bb->m_position - aa->m_position) *
            aa->getRadius () / (aa->getRadius () + bb->getRadius ()) + aa->m_position;
         glm::dvec3 relCA = contactPoint - aa->m_position;
         if (aa->m_rotationVel > 0) {
            glm::dvec3 r = relCA - glm::dot (aa->m_rotationAxis, relCA) /
               glm::length2 (aa->m_rotationAxis) * aa->m_rotationAxis;
            glm::dvec3 v = glm::cross (glm::normalize (aa->m_rotationAxis), r) *
               aa->m_rotationVel;
            relVelSurface -= v;
         }
         glm::dvec3 relCB = contactPoint - bb->m_position;
         if (bb->m_rotationVel > 0) {
            glm::dvec3 r = relCB - glm::dot (bb->m_rotationAxis, relCB) /
               glm::length2 (bb->m_rotationAxis) * bb->m_rotationAxis;
            glm::dvec3 v = glm::cross (glm::normalize (bb->m_rotationAxis), r) *
               bb->m_rotationVel;
            relVelSurface += v;
         }
         relVelSurface -= glm::dot (relVelSurface, normal) * normal; // Project to normal plane.
         //DebugInfo::vectors.push_back (DebugVector{ contactPoint + glm::dvec3{0,0,2.5}, relVelSurface * 10. });
         // -Apply friction impulse.
         double collisionMassRotation = 1.0 / (
            1.0 / aa->getMass () + aa->getRadius () / aa->m_momentOfInertia +
            1.0 / bb->getMass () + bb->getRadius () / bb->m_momentOfInertia
            );
         glm::dvec3 impulseFriction = collisionMassRotation * relVelSurface;
         double maxImpulse = normalImpulse * std::min (aa->m_frictionConstant, bb->m_frictionConstant);
         if (glm::length2 (impulseFriction) > maxImpulse * maxImpulse) {
            impulseFriction *= maxImpulse / glm::length (impulseFriction);
         }
         aa->m_velocity += impulseFriction / aa->getMass ();
         bb->m_velocity -= impulseFriction / bb->getMass ();
         glm::dvec3 angVelA = aa->getAngVel ();
         glm::dvec3 angVelB = bb->getAngVel ();
         glm::dvec3 torqueA = glm::cross (relCA, impulseFriction);
         glm::dvec3 torqueB = glm::cross (relCB, -impulseFriction);
         angVelA += torqueA / aa->m_momentOfInertia;
         angVelB += torqueB / bb->m_momentOfInertia;
         aa->m_rotationAxis = angVelA;
         bb->m_rotationAxis = angVelB;
         aa->m_rotationVel = glm::length (angVelA);
         bb->m_rotationVel = glm::length (angVelB);
      }
   }
}

void PhysicsEngine::handleAcceleration (double factor) {
   // Accelerate.
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* b = m_spheres[ii];
      b->m_velocity += glm::dvec3 (0, 0, -0.002725 * factor);
      //if (glm::length(b->m_position) > 100.0) {
      //   b->m_velocity -= b->m_position * 0.01;
      //}
      //b->m_velocity -= b->m_position * 0.00001;
   }
}

void PhysicsEngine::measureMovementError () {
   
   // Distance constraints.
   for (size_t ii = 0; ii < m_distanceConstraints.size (); ii++) {
      //m_distanceConstraints[ii]->constrainMomentum ();
      m_distanceConstraints[ii]->measureMovementError ();
   }

   // Angle constraints.
   //for (size_t ii = 0; ii < m_angleConstraints.size (); ii++) {
   //   m_angleConstraints[ii]->constrainMomentum ();
   //}

   // Direction constraints.
   for (size_t ii = 0; ii < m_directionConstraints.size (); ii++) {
      m_directionConstraints[ii]->measureMovementError ();
   }

   // Sphere ground.
   for (auto& it : m_sphereGroundCollisionInfo) {
      if (it.second.m_collisionTime == 0) {
         continue;
      }
      SphereWrapper* ss = it.second.m_sphere;
      double normalImpulse{ calcForceSphereGround (ss, false) };
      it.second.m_normalCompensation += normalImpulse;
   }

   // Sphere sphere.
   for (auto& it : m_sphereSphereCollisionInfo) {
      if (it.second.m_collisionTime == 0) {
         continue;
      }
      SphereWrapper* aa = it.second.m_sphereA;
      SphereWrapper* bb = it.second.m_sphereB;

      glm::dvec3 normal{};
      double normalImpulse{ calculateForceSphereSphere (aa, bb, &normal, false) };
      it.second.m_normalCompensation += normalImpulse;
   }
}

void PhysicsEngine::handleMovement () {
   // Move.
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* ss = m_spheres[ii];
      ss->m_prevPosition = ss->m_position;
   }
   for (size_t ii = 0; ii < m_spheres.size (); ii++) {
      SphereWrapper* ss = m_spheres[ii];
      ss->m_position += ss->m_velocity;
      glm::dvec3 rotationAxis{};
      double rotationAxisLength = glm::length (ss->m_rotationAxis);
      if (rotationAxisLength > 0.0) {
         rotationAxis = ss->m_rotationAxis / rotationAxisLength;
      } else {
         rotationAxis = glm::dvec3{ 1,0,0 };
      }
      ss->m_orientation = glm::angleAxis (ss->m_rotationVel, rotationAxis) * ss->m_orientation;
      //if (ss->m_id == 11) {
      //   DebugInfo::vectors.push_back (DebugVector{
      //      ss->m_position + glm::dvec3 {0, 0, ss->getRadius () * 1.5 },
      //      rotationAxis* ss->m_rotationVel * 10.
      //   });
      //}
      //DebugInfo::vectors.push_back (DebugVector{
      //   ss->m_position + glm::dvec3 {0, 0, ss->getRadius () * 1.5 },
      //   ss->m_velocity,
      //   1.0, glm::dvec3{0,1,0}
      //});
   }
}

void PhysicsEngine::savePreviousPositions () {}

void PhysicsEngine::simulate () {
   if (m_time == 1107) {
       int a = 0; a;
   }
   //std::cout << "time: " << m_time << std::endl;

   //
   int repetitions = 4;
   handleAcceleration (0.5);
   resolveMomentum (false, 1.0);
   handleAcceleration (0.5);
   resolveMomentum (repetitions == 0, 1.0);
   for (size_t ii = 0; ii < repetitions; ii++) {
      // Accelerate.
      //handleAcceleration (1.0 / ((double)repetitions));
      resolveMomentum (ii == (repetitions - 1), 1.0);
   }
   
   // Measure movement error.
   measureMovementError ();

   // Move.
   handleMovement ();

   // Collide.
   collisionDetection ();
   //savePreviousPositions ();
   //for (size_t ii = 0; ii < 1; ii++) {
   //   resolveMomentum ();
   //}
   resolveOverlap ();

   //
   m_time++;
}

SphereWrapper* PhysicsEngine::createSphere () {
   SphereWrapper* newSphere = new SphereWrapper{};
   newSphere->m_id = m_totalNumSpheres;
   m_totalNumSpheres++;
   newSphere->m_index = m_spheres.size ();
   m_spheres.push_back (newSphere);
   return newSphere;
}

void PhysicsEngine::removeSphere (Sphere* sphere) {
#ifdef DEBUG_LEVEL_0
   if (m_spheres.size() <= sphere->m_index || m_spheres[sphere->m_index] != sphere) {
      throw std::runtime_error ("ERROR: Sphere index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // First remove all constraints.
   SphereWrapper* sphereWrapper{m_spheres[sphere->m_index]};
   while (sphereWrapper->m_distanceConstraints.size () > 0) {
      removeDistanceContraint (sphereWrapper->m_distanceConstraints[0]);
   }
   while (sphereWrapper->m_angleConstraints.size () > 0) {
      removeAngleConstraint (sphereWrapper->m_angleConstraints[0]);
   }
   while (sphereWrapper->m_directionConstraints.size () > 0) {
      removeDirectionConstraint (sphereWrapper->m_directionConstraints[0]);
   }

   // Remove sphere itself.
   // Move last element and overwrite its own place in the list.
   if (sphere->m_index == m_spheres.size() - 1) {
      m_spheres.pop_back ();
   } else {
      SphereWrapper* lastSphere = m_spheres.back ();
      m_spheres.pop_back ();
      m_spheres[sphere->m_index] = lastSphere;
      lastSphere->m_index = sphere->m_index;
   }

   //
   delete sphere;
}

DistanceConstraint* PhysicsEngine::createDistanceConstraint (Sphere* sA, Sphere* sB, double distance) {
   DistanceConstraint* newDistanceConstraint = new DistanceConstraint{};
   newDistanceConstraint->m_sA = sA;
   newDistanceConstraint->m_sB = sB;
   newDistanceConstraint->m_distance = distance;
   newDistanceConstraint->m_index = m_distanceConstraints.size ();
   m_distanceConstraints.push_back (newDistanceConstraint);

   m_spheres[sA->m_index]->m_distanceConstraints.push_back (newDistanceConstraint);
   m_spheres[sB->m_index]->m_distanceConstraints.push_back (newDistanceConstraint);

   return newDistanceConstraint;
}

void PhysicsEngine::removeDistanceContraint (DistanceConstraint* distanceConstraint) {
#ifdef DEBUG_LEVEL_0
   if (
      m_distanceConstraints.size () <= distanceConstraint->m_index ||
      m_distanceConstraints[distanceConstraint->m_index] != distanceConstraint) {
      throw std::runtime_error ("ERROR: Distance constraint index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // Remove self from spheres.
   std::vector<SphereWrapper*> sws{
      m_spheres[distanceConstraint->m_sA->m_index],
      m_spheres[distanceConstraint->m_sB->m_index],
   };
   for (size_t jj = 0; jj < sws.size(); jj++) {
      bool found{false};
      for (size_t ii = 0; ii < sws[jj]->m_distanceConstraints.size (); ii++) {
         const DistanceConstraint* dc = sws[jj]->m_distanceConstraints[ii];
         if (dc == distanceConstraint) {
            if (ii == sws[jj]->m_distanceConstraints.size () - 1) {
               sws[jj]->m_distanceConstraints.pop_back ();
            } else {
               DistanceConstraint* lastDistanceConstraint = sws[jj]->m_distanceConstraints.back ();
               sws[jj]->m_distanceConstraints.pop_back ();
               sws[jj]->m_distanceConstraints[ii] = lastDistanceConstraint;
            }
            found = true;
            break;
         }
      }
      if (!found) {
         throw std::runtime_error ("ERROR: Could not find reference to constraint in sphere.");
      }
   }

   // Remove from physics engine.
   // Move last element and overwrite its own place in the list.
   if (distanceConstraint->m_index == m_distanceConstraints.size () - 1) {
      m_distanceConstraints.pop_back ();
   } else {
      DistanceConstraint* lastDistanceConstraint = m_distanceConstraints.back ();
      m_distanceConstraints.pop_back ();
      m_distanceConstraints[distanceConstraint->m_index] = lastDistanceConstraint;
      lastDistanceConstraint->m_index = distanceConstraint->m_index;
   }
   delete distanceConstraint;
}

AngleConstraint* PhysicsEngine::createAngleConstraint (
   Sphere* sA, Sphere* sB, double zConstraint, glm::dquat offsetA, glm::dquat offsetB
) {
   AngleConstraint* newAngleConstraint = new AngleConstraint{};
   newAngleConstraint->m_sA = sA;
   newAngleConstraint->m_sB = sB;
   newAngleConstraint->m_zConstraint = zConstraint;
   newAngleConstraint->m_index = m_angleConstraints.size ();
   newAngleConstraint->m_offsetA = offsetA;
   newAngleConstraint->m_offsetB = offsetB;
   m_angleConstraints.push_back (newAngleConstraint);

   m_spheres[sA->m_index]->m_angleConstraints.push_back (newAngleConstraint);
   m_spheres[sB->m_index]->m_angleConstraints.push_back (newAngleConstraint);

   return newAngleConstraint;
}

void PhysicsEngine::removeAngleConstraint (AngleConstraint* angleConstraint) {
#ifdef DEBUG_LEVEL_0
   if (
      m_angleConstraints.size () <= angleConstraint->m_index ||
      m_angleConstraints[angleConstraint->m_index] != angleConstraint) {
      throw std::runtime_error ("ERROR: Distance constraint index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // Remove self from spheres.
   std::vector<SphereWrapper*> sws{
      m_spheres[angleConstraint->m_sA->m_index],
      m_spheres[angleConstraint->m_sB->m_index],
   };
   for (size_t jj = 0; jj < sws.size (); jj++) {
      bool found{ false };
      for (size_t ii = 0; ii < sws[jj]->m_angleConstraints.size (); ii++) {
         const AngleConstraint* dc = sws[jj]->m_angleConstraints[ii];
         if (dc == angleConstraint) {
            if (ii == sws[jj]->m_angleConstraints.size () - 1) {
               sws[jj]->m_angleConstraints.pop_back ();
            } else {
               AngleConstraint* lastAngleConstraint = sws[jj]->m_angleConstraints.back ();
               sws[jj]->m_angleConstraints.pop_back ();
               sws[jj]->m_angleConstraints[ii] = lastAngleConstraint;
            }
            found = true;
            break;
         }
      }
      if (!found) {
         throw std::runtime_error ("ERROR: Could not find reference to constraint in sphere.");
      }
   }
   // Move last element and overwrite its own place in the list.
   if (angleConstraint->m_index == m_angleConstraints.size () - 1) {
      m_angleConstraints.pop_back ();
   } else {
      AngleConstraint* lastDistanceConstraint = m_angleConstraints.back ();
      m_angleConstraints.pop_back ();
      m_angleConstraints[angleConstraint->m_index] = lastDistanceConstraint;
      lastDistanceConstraint->m_index = angleConstraint->m_index;
   }
   delete angleConstraint;
}

DirectionConstraint* PhysicsEngine::createDirectionConstraint (
   Sphere* sA, Sphere* sB, glm::dquat offset, glm::dvec3 constraint, bool totalLock
) {
   DirectionConstraint* newDirectionConstraint = new DirectionConstraint{};
   newDirectionConstraint->m_sA = sA;
   newDirectionConstraint->m_sB = sB;
   newDirectionConstraint->m_offset = offset;
   newDirectionConstraint->m_index = m_directionConstraints.size ();
   newDirectionConstraint->m_constraint = constraint;
   newDirectionConstraint->m_totalLock = totalLock;
   m_directionConstraints.push_back (newDirectionConstraint);

   m_spheres[sA->m_index]->m_directionConstraints.push_back (newDirectionConstraint);
   m_spheres[sB->m_index]->m_directionConstraints.push_back (newDirectionConstraint);

   return newDirectionConstraint;
}

void PhysicsEngine::removeDirectionConstraint (DirectionConstraint* directionConstraint) {
#ifdef DEBUG_LEVEL_0
   if (
      m_directionConstraints.size () <= directionConstraint->m_index ||
      m_directionConstraints[directionConstraint->m_index] != directionConstraint) {
      throw std::runtime_error ("ERROR: Distance constraint index does not refer to itself.");
   }
#endif // DEBUG_LEVEL_0

   // Remove self from spheres.
   std::vector<SphereWrapper*> sws{
      m_spheres[directionConstraint->m_sA->m_index],
      m_spheres[directionConstraint->m_sB->m_index],
   };
   for (size_t jj = 0; jj < sws.size (); jj++) {
      bool found{ false };
      for (size_t ii = 0; ii < sws[jj]->m_directionConstraints.size (); ii++) {
         const DirectionConstraint* dc = sws[jj]->m_directionConstraints[ii];
         if (dc == directionConstraint) {
            if (ii == sws[jj]->m_directionConstraints.size () - 1) {
               sws[jj]->m_directionConstraints.pop_back ();
            } else {
               DirectionConstraint* lastDirectionConstraint = sws[jj]->m_directionConstraints.back ();
               sws[jj]->m_directionConstraints.pop_back ();
               sws[jj]->m_directionConstraints[ii] = lastDirectionConstraint;
            }
            found = true;
            break;
         }
      }
      if (!found) {
         throw std::runtime_error ("ERROR: Could not find reference to constraint in sphere.");
      }
   }
   // Move last element and overwrite its own place in the list.
   if (directionConstraint->m_index == m_directionConstraints.size () - 1) {
      m_directionConstraints.pop_back ();
   } else {
      DirectionConstraint* lastDistanceConstraint = m_directionConstraints.back ();
      m_directionConstraints.pop_back ();
      m_directionConstraints[directionConstraint->m_index] = lastDistanceConstraint;
      lastDistanceConstraint->m_index = directionConstraint->m_index;
   }
   delete directionConstraint;
}
