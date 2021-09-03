



#include "DirectionConstraint.h"

#include "DebugInfo.h"

#include <glm/gtx/projection.hpp>



double DirectionConstraint::calculateMomentOfInertia (glm::dvec3 axis) {
   glm::dvec3 centerOfMass{
      (m_sA->m_position * m_sA->getMass () + m_sB->m_position * m_sB->getMass ()) /
      (m_sA->getMass () + m_sB->getMass ())
   };
   // -Using parallel axis theorem.
   double translationalMomentOfInertiaA{ m_sA->getMass () * glm::length2 (
      m_sA->m_position - centerOfMass - glm::proj (m_sA->m_position - centerOfMass, axis)
   ) };
   double translationalMomentOfInertiaB{ m_sB->getMass () * glm::length2 (
      m_sB->m_position - centerOfMass - glm::proj (m_sB->m_position - centerOfMass, axis)
   ) };
   return translationalMomentOfInertiaA + translationalMomentOfInertiaB;
}

void DirectionConstraint::calculateMomentOfInertia (
   glm::dvec3 axis, glm::dvec3* centerOfMassOut,
   double* translationalMomentOfInertiaAOut,
   double* translationalMomentOfInertiaBOut
) {
   *centerOfMassOut =
      (m_sA->m_position * m_sA->getMass () + m_sB->m_position * m_sB->getMass ()) /
      (m_sA->getMass () + m_sB->getMass ())
   ;
   // -Using parallel axis theorem.
   *translationalMomentOfInertiaAOut = m_sA->getMass () * glm::length2 (
      m_sA->m_position - *centerOfMassOut - glm::proj(m_sA->m_position - *centerOfMassOut, axis)
   );
   *translationalMomentOfInertiaBOut = m_sB->getMass () * glm::length2 (
      m_sB->m_position - *centerOfMassOut - glm::proj (m_sB->m_position - *centerOfMassOut, axis)
   );
}

void DirectionConstraint::measureMovementError () {
   glm::dvec3 torque{ calculateTorque () };
   torque = glm::conjugate(m_sA->m_orientation * m_offset) * torque;
   //if (glm::dot(m_compensator, torque) < 0.0) {
   //   m_compensator += torque;
   //} else {
   //   m_compensator += torque * 0.5;
   //}
   m_compensator += torque;
   m_compensator.z = 0.0;
}

glm::dvec3 DirectionConstraint::calculateTorque () {
   glm::dquat aOffsetOrientation = m_sA->m_orientation * m_offset;

   glm::dvec3 bRelPos{ m_sB->m_position - m_sA->m_position };
   double bRelPosSqr{ glm::length2 (bRelPos) };
   if (bRelPosSqr == 0.0) {
      return glm::dvec3{0,0,0};
   }
   glm::dvec3 bTransAngVel{
      glm::cross (bRelPos, m_sB->m_velocity - m_sA->m_velocity) / bRelPosSqr
   };
   glm::dvec3 angVelA{ m_sA->getAngVel () };
   glm::dvec3 torque{};
   if (m_totalLock) {
      glm::dvec3 deltaWTotal{ angVelA - bTransAngVel };
      glm::dvec3 deltaW{ deltaWTotal - glm::dot (deltaWTotal, bRelPos) * bRelPos / bRelPosSqr };

      if (glm::length2(deltaW) == 0.0) {
         return glm::dvec3{ 0,0,0 };
      }

      double translationalMomentOfInteria{ calculateMomentOfInertia(deltaW) };
      double collisionMass{
         1.0 / (1.0/m_sA->m_momentOfInertia + 1.0/ translationalMomentOfInteria)
      };

      torque = -deltaW * collisionMass;
   } else {
      glm::dvec3 deltaW{ bTransAngVel - angVelA };

      glm::dvec3 localBRelPos{ glm::conjugate (aOffsetOrientation) * bRelPos };
      glm::dvec3 localDeltaW{ glm::conjugate (aOffsetOrientation) * deltaW };
      glm::dvec3 localTorque{ 0,0,0 };
      double bRelPosLength{ sqrt (bRelPosSqr) };

      // Y direction:
      if (m_constraint.y > 0.0) {
         double angle{ glm::acos (localBRelPos.y / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{0,1,0}) };
         glm::dvec3 angVel{ glm::proj (
            localDeltaW,
            towardsAxis
         ) };
         if (angle < m_constraint.y && glm::dot (angVel, towardsAxis) > 0.0) {
            localTorque += angVel;
         } else if (glm::pi<double> () - angle < m_constraint.y && glm::dot (angVel, towardsAxis) < 0.0) {
            localTorque += angVel;
         }
      }

      // X direction:
      if (m_constraint.x > 0.0) {
         double angle{ glm::acos (localBRelPos.x / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{1,0,0}) };
         glm::dvec3 angVel{ glm::proj (
            localDeltaW,
            towardsAxis
         ) };
         if (angle < m_constraint.x && glm::dot (angVel, towardsAxis) > 0.0) {
            localTorque += angVel;
         } else if (glm::pi<double> () - angle < m_constraint.x && glm::dot (angVel, towardsAxis) < 0.0) {
            localTorque += angVel;
         }
      }

      // Z direction:
      if (m_constraint.z > 0.0) {
         double angle{ glm::acos (localBRelPos.z / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{0,0,1}) };
         glm::dvec3 angVel{ glm::proj (
            localDeltaW,
            towardsAxis
         ) };
         if (glm::pi<double> () - angle < m_constraint.z && glm::dot (angVel, towardsAxis) < 0.0) {
            localTorque += angVel;
         }
      }

      torque = aOffsetOrientation * (localTorque * m_sA->m_momentOfInertia);
   }
   //glm::dvec3 up = m_sA->m_orientation * glm::dvec3{ 0,0,1 }; up;
   //glm::dvec3 up2 = m_sA->m_orientation * m_offset * glm::dvec3{ 0,0,1 }; up2;
   return torque;
}

void DirectionConstraint::constrainMomentum (bool useCompensator, double compensatorFactor) {

   // Rotate A towards B.
   glm::dvec3 angVelA{ m_sA->getAngVel () };
   glm::dvec3 torque{ calculateTorque() };

   if (useCompensator) {
      torque += m_sA->m_orientation * m_offset * m_compensator * compensatorFactor;
   }


   /*glm::dvec3 bRelPos{ m_sB->m_position - m_sA->m_position };
   double bRelPosLength2{ glm::length2 (bRelPos) };
   if (bRelPosLength2 == 0.0) {
      return;
   }
   torque = torque - glm::proj (torque, bRelPos);*/
   torque = glm::conjugate(m_sA->m_orientation * m_offset) * torque;
   torque.z = 0.0;
   torque = m_sA->m_orientation * m_offset * torque;
   //glm::dvec3 test{ m_sA->m_orientation * m_offset * glm::dvec3{0,0,1} };

   //if (m_sA->m_id == 11 && useCompensator) {
   //   DebugInfo::vectors.push_back (DebugVector{
   //      m_sA->m_position + glm::dvec3 {0, 0, m_sA->getRadius () * 2.5 },
   //      m_sA->m_orientation* m_offset * m_compensator * 1000000.
   //      });
   //}

   //DebugInfo::vectors.push_back (DebugVector{ m_sA->m_position + glm::dvec3 {m_sA->getRadius () * 1.5, 0, 0.0 }, m_sA->m_orientation * m_offset * m_compensator * 1000. });

   // Apply torque.
   //DebugInfo::vectors.push_back (DebugVector{ m_sA->m_position + glm::dvec3 {2, 0, 0.0 }, torque * 1000. });

   angVelA += torque / m_sA->m_momentOfInertia;
   m_sA->m_rotationAxis = angVelA; // TOOD, don't have to do this twice.
   m_sA->m_rotationVel = glm::length (angVelA);

   // Apply opposite torque on the system.
   double torqueLength2{ glm::length2 (torque) };
   if (torqueLength2 == 0.0) {
      return;
   }
   glm::dvec3 centerOfMass{};
   double translationalMomentOfInertiaA, translationalMomentOfInertiaB;
   calculateMomentOfInertia (
      torque, &centerOfMass, &translationalMomentOfInertiaA, &translationalMomentOfInertiaB
   );
   double momentOfIntertia{
      //m_sA->m_momentOfInertia + m_sB->m_momentOfInertia +
      translationalMomentOfInertiaA + translationalMomentOfInertiaB
   };
   glm::dvec3 angVelB{ m_sB->getAngVel () };

   //momentOfIntertia;

   //angVelA += -torque / momentOfIntertia;
   //m_sA->m_rotationAxis = angVelA;
   //m_sA->m_rotationVel = glm::length (angVelA);
   //angVelB += -torque / momentOfIntertia;
   //m_sB->m_rotationAxis = angVelB;
   //m_sB->m_rotationVel = glm::length (angVelB);

   m_sA->m_velocity +=
      // Force:
      glm::cross (-torque, m_sA->m_position - centerOfMass) /
      glm::length2 (m_sA->m_position - centerOfMass) *
      // Fraction to be applied to this object.
      translationalMomentOfInertiaA / momentOfIntertia /
      // Divide with mass to get delta velocity.
      m_sA->getMass ();

   m_sB->m_velocity +=
      // Force:
      glm::cross (-torque, m_sB->m_position - centerOfMass) /
      glm::length2 (m_sB->m_position - centerOfMass) *
      // Fraction to be applied to this object.
      translationalMomentOfInertiaB / momentOfIntertia /
      // Divide with mass to get delta velocity.
      m_sB->getMass ();
}

void DirectionConstraint::constrainPosition () {
   // Rotate towards target.
   glm::dquat aOffsetOrientation = m_sA->m_orientation * m_offset;

   double rotationAngle{};
   double axisLength{};
   glm::dvec3 axisUnit{};
   double factor{ 1.0 }; // Reduces jittering.
   glm::dvec3 bRelPos{ m_sB->m_position - m_sA->m_position };
   if (m_totalLock) {
      glm::dvec3 directionGlobal{ glm::dvec3{0,0,1} };
      glm::dvec3 direction{ aOffsetOrientation * directionGlobal };
      glm::dvec3 axis{ glm::cross (direction, bRelPos) };

      axis = glm::conjugate (m_sA->m_orientation * m_offset) * axis;
      axis.z = 0.0;
      axis = m_sA->m_orientation * m_offset * axis;

      double axisLength2{ glm::length2 (axis) };
      if (axisLength2 == 0.0) {
         return;
      }
      axisLength = glm::sqrt (axisLength2);
      rotationAngle = glm::asin (axisLength / glm::length (direction) / glm::length (bRelPos));
      axisUnit = axis / axisLength;
   } else {
      glm::dvec3 localBRelPos{ glm::conjugate (aOffsetOrientation) * bRelPos };
      double bRelPosSqr{ glm::length2 (bRelPos) };
      if (bRelPosSqr == 0.0) {
         return;
      }
      double bRelPosLength{ sqrt (bRelPosSqr) };
      
      glm::dvec3 rotation{0,0,0};

      // Y direction:
      if (m_constraint.y > 0.0) {
         double angle{ glm::acos (localBRelPos.y / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{0,1,0}) };
         double towardsAxisLength{ glm::length (towardsAxis) };
         if (angle < m_constraint.y && angle > 0.0 && towardsAxisLength > 0.0) {
            rotation += towardsAxis / towardsAxisLength * (m_constraint.y - angle);
         } else if (glm::pi<double> () - angle < m_constraint.y && towardsAxisLength > 0.0) {
            rotation -= towardsAxis / towardsAxisLength * (m_constraint.y - (glm::pi<double> () - angle));
         }
      }

      // X direction:
      if (m_constraint.x > 0.0) {
         double angle{ glm::acos (localBRelPos.x / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{1,0,0}) };
         double towardsAxisLength{ glm::length (towardsAxis) };
         if (angle < m_constraint.x && angle > 0.0 && towardsAxisLength > 0.0) {
            rotation += towardsAxis / towardsAxisLength * (m_constraint.x - angle);
         } else if (glm::pi<double> () - angle < m_constraint.x && towardsAxisLength > 0.0) {
            rotation -= towardsAxis / towardsAxisLength * (m_constraint.x - (glm::pi<double> () - angle));
         }
      }

      // Z direction:
      if (m_constraint.z > 0.0) {
         double angle{ glm::acos (localBRelPos.z / bRelPosLength) };
         glm::dvec3 towardsAxis{ glm::cross (localBRelPos, glm::dvec3{0,0,1}) };
         double towardsAxisLength{ glm::length (towardsAxis) };
         if (glm::pi<double> () - angle < m_constraint.z && towardsAxisLength > 0.0) {
            rotation -= towardsAxis / towardsAxisLength * (m_constraint.z - (glm::pi<double> () - angle));
         }
      }

      rotation = aOffsetOrientation * rotation;

      rotation = glm::conjugate (m_sA->m_orientation * m_offset) * rotation;
      rotation.z = 0.0;
      rotation = m_sA->m_orientation * m_offset * rotation;

      rotationAngle = glm::length (rotation);
      if (rotationAngle > 0.0) {
         axisUnit = rotation / rotationAngle;
      } else {
         return;
      }
   }

   double translationalMomentOfInteria{ calculateMomentOfInertia (axisUnit) };
   double collisionMass{
      1.0 / (1.0 / m_sA->m_momentOfInertia + 1.0 / translationalMomentOfInteria)
   };
   double torque{ rotationAngle * collisionMass };
   torque *= factor;
   m_sA->m_orientation = glm::angleAxis (torque / m_sA->m_momentOfInertia, axisUnit) * m_sA->m_orientation;

   // Apply opposite rotation to system.
   if (axisLength == 0.0) {
      return;
   }

   glm::dvec3 centerOfMass{};
   double translationalMomentOfInertiaA, translationalMomentOfInertiaB;
   calculateMomentOfInertia (
      axisUnit, &centerOfMass, &translationalMomentOfInertiaA, &translationalMomentOfInertiaB
   );
   double momentOfIntertia{
      //m_sA->m_momentOfInertia + m_sB->m_momentOfInertia +
      translationalMomentOfInertiaA + translationalMomentOfInertiaB
   };

   //double torqueLength{ rotationAngle * factor * m_sA->m_momentOfInertia };
   double torqueLength{ torque };
   glm::dvec3 torqueVec = axisUnit * torqueLength;

   //m_sA->m_orientation = glm::angleAxis (-torqueLength / momentOfIntertia, axisUnit) * m_sA->m_orientation;
   //m_sB->m_orientation = glm::angleAxis (-torqueLength / momentOfIntertia, axisUnit) * m_sB->m_orientation;

   m_sA->m_position +=
      // Force:
      glm::cross (-torqueVec, m_sA->m_position - centerOfMass) /
      glm::length2 (m_sA->m_position - centerOfMass) *
      // Fraction to be applied to this object.
      translationalMomentOfInertiaA / momentOfIntertia /
      // Divide with mass to get delta displacement.
      m_sA->getMass ();
   m_sB->m_position +=
      // Force:
      glm::cross (-torqueVec, m_sB->m_position - centerOfMass) /
      glm::length2 (m_sB->m_position - centerOfMass) *
      // Fraction to be applied to this object.
      translationalMomentOfInertiaB / momentOfIntertia /
      // Divide with mass to get delta displacement.
      m_sB->getMass ();
}
