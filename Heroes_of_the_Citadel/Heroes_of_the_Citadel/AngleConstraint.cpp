#include "AngleConstraint.h"


#include "DebugInfo.h"






glm::dvec3 AngleConstraint::calculateTorque () {
   //glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate (glm::angleAxis (glm::half_pi<double> ()*0.0, glm::dvec3 (1, 0, 0)));
   glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate (m_offsetA);
   glm::dquat sBOrientation = m_sB->m_orientation * glm::conjugate (m_offsetB);
   // A's orientation relative to B.
   glm::dquat relOrientation{ sAOrientation * glm::conjugate (sBOrientation) };
   double angle = 2.0 * acos (relOrientation.w);
   glm::dvec3 axis{};
   double squareW = relOrientation.w * relOrientation.w;
   double div{};
   if (squareW >= 1.0 || (div = sqrt (1.0 - squareW)) == 0 || div == 0.0) { // Larger or equal just in case length of the quaternion is not exacly 1.
      axis = glm::dvec3{ 0,0,0 };
   } else {
      axis = {
            relOrientation.x / div,
            relOrientation.y / div,
            relOrientation.z / div,
      };
   }
   double ll = glm::length (axis);
   if (ll > 0.0) {
      axis *= angle / ll;
   }
   glm::dvec3 relAxis = glm::conjugate (sBOrientation) * axis;
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{0,0,3}, axis });
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{-3,0,3}, relAxis });
   if (relAxis.z > m_zConstraint || relAxis.z < -m_zConstraint) {
      glm::dvec3 angVelA{ m_sA->getAngVel () };
      glm::dvec3 angVelB{ m_sB->getAngVel () };
      glm::dvec3 relAngVel = angVelA - angVelB;
      relAngVel = glm::conjugate (sBOrientation) * relAngVel;
      double collisionMass = 1.0 / (1.0 / m_sA->m_momentOfInertia + 1.0 / m_sB->m_momentOfInertia);
      if (relAxis.z > m_zConstraint && relAngVel.z <= 0.0) {
         return glm::dvec3{ 0,0,0 };
      } else if (relAxis.z < -m_zConstraint && relAngVel.z >= 0.0) {
         return glm::dvec3{ 0,0,0 };
      }
      glm::dvec3 torque{ 0,0, -relAngVel.z * collisionMass };
      torque = sBOrientation * torque;
      return torque;
   }
   return glm::dvec3{0,0,0};
}

void AngleConstraint::constrainMomentum (bool useCompensator, double /*compensatorFactor*/) {
   glm::dvec3 torque{ calculateTorque () };
   if (useCompensator) {
      //torque += m_compensator;
   }
   if (glm::length2(torque) > 0.0) {
      glm::dvec3 angVelA{ m_sA->getAngVel () };
      glm::dvec3 angVelB{ m_sB->getAngVel () };
      angVelA += torque / m_sA->m_momentOfInertia;
      angVelB -= torque / m_sB->m_momentOfInertia;
      m_sA->m_rotationAxis = angVelA;
      m_sB->m_rotationAxis = angVelB;
      m_sA->m_rotationVel = glm::length (angVelA);
      m_sB->m_rotationVel = glm::length (angVelB);
   }

   /*
   //glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate (glm::angleAxis (glm::half_pi<double> ()*0.0, glm::dvec3 (1, 0, 0)));
   glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate (m_offsetA);
   glm::dquat sBOrientation = m_sB->m_orientation * glm::conjugate (m_offsetB);
   // A's orientation relative to B.
   glm::dquat relOrientation{ sAOrientation * glm::conjugate (sBOrientation) };
   double angle = 2.0 * acos (relOrientation.w);
   glm::dvec3 axis{};
   double squareW = relOrientation.w * relOrientation.w;
   double div{};
   if (squareW >= 1.0 || (div = sqrt (1.0 - squareW)) == 0 || div == 0.0) { // Larger or equal just in case length of the quaternion is not exacly 1.
      axis = glm::dvec3{ 0,0,0 };
   } else {
      axis = {
            relOrientation.x / div,
            relOrientation.y / div,
            relOrientation.z / div,
      };
   }
   double ll = glm::length (axis);
   if (ll > 0.0) {
      axis *= angle / ll;
   }
   glm::dvec3 relAxis = glm::conjugate (sBOrientation) * axis;
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{0,0,3}, axis });
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{-3,0,3}, relAxis });
   if (relAxis.z > m_zConstraint || relAxis.z < -m_zConstraint) {
      glm::dvec3 angVelA{ m_sA->getAngVel () };
      glm::dvec3 angVelB{ m_sB->getAngVel () };
      glm::dvec3 relAngVel = angVelA - angVelB;
      relAngVel = glm::conjugate (sBOrientation) * relAngVel;
      double collisionMass = 1.0 / (1.0 / m_sA->m_momentOfInertia + 1.0 / m_sB->m_momentOfInertia);
      if (relAxis.z > m_zConstraint && relAngVel.z <= 0.0) {
         return;
      } else if (relAxis.z < -m_zConstraint && relAngVel.z >= 0.0) {
         return;
      }
      glm::dvec3 torque{ 0,0, -relAngVel.z * collisionMass };
      torque = sBOrientation * torque;
      angVelA += torque / m_sA->m_momentOfInertia;
      angVelB -= torque / m_sB->m_momentOfInertia;
      m_sA->m_rotationAxis = angVelA;
      m_sB->m_rotationAxis = angVelB;
      m_sA->m_rotationVel = glm::length (angVelA);
      m_sB->m_rotationVel = glm::length (angVelB);
   }
   */
}

void AngleConstraint::constrainPosition () {
   //glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate(glm::angleAxis (glm::half_pi<double> ()*0.0, glm::dvec3 (1, 0, 0)));
   glm::dquat sAOrientation = m_sA->m_orientation * glm::conjugate (m_offsetA);
   glm::dquat sBOrientation = m_sB->m_orientation * glm::conjugate (m_offsetB);
   // A's orientation relative to B.
   glm::dquat relOrientation{ sAOrientation * glm::conjugate (sBOrientation) };
   double angle = 2.0 * acos (relOrientation.w);
   glm::dvec3 axis{};
   double squareW = relOrientation.w * relOrientation.w;
   double div{};
   if (squareW >= 1.0 || (div = sqrt (1.0 - relOrientation.w * relOrientation.w)) == 0) { // Larger or equal just in case length of the quaternion is not exacly 1.
      axis = glm::dvec3{ 0,0,0 };
   } else {
      axis = {
            relOrientation.x / div,
            relOrientation.y / div,
            relOrientation.z / div,
      };
   }
   double ll = glm::length (axis);
   if (ll > 0.0) {
      axis *= angle / ll;
   }
   glm::dvec3 relAxis = glm::conjugate (sBOrientation) * axis;
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{0,0,3}, axis });
   //DebugInfo::vectors.push_back (DebugVector{ m_sB->m_position + glm::dvec3{-3,0,3}, relAxis });
   double displacementZ{};
   if (relAxis.z > m_zConstraint) {
      displacementZ = m_zConstraint - relAxis.z;
   } else if (relAxis.z < -m_zConstraint) {
      displacementZ = (-m_zConstraint) - relAxis.z;
   } else {
      return;
   }
   double collisionMass = 1.0 / (1.0 / m_sA->m_momentOfInertia + 1.0 / m_sB->m_momentOfInertia);
   glm::dvec3 torqueDisplacement{ 0,0, displacementZ * collisionMass };
   torqueDisplacement = sBOrientation * torqueDisplacement;
   glm::dvec3 displacementA = torqueDisplacement / m_sA->m_momentOfInertia;
   glm::dvec3 displacementB = -torqueDisplacement / m_sB->m_momentOfInertia;
   ll = glm::length (displacementA);
   if (ll > 0.0) {
      m_sA->m_orientation = glm::angleAxis (ll, displacementA / ll) * m_sA->m_orientation;
   }
   ll = glm::length (displacementB);
   if (ll > 0.0) {
      m_sB->m_orientation = glm::angleAxis (ll, displacementB / ll) * m_sB->m_orientation;
   }

   //angVelA += torque / m_sA->m_momentOfInertia;
   //angVelB -= torque / m_sB->m_momentOfInertia;
   //m_sA->m_rotationAxis = angVelA;
   //m_sB->m_rotationAxis = angVelB;
   //m_sA->m_rotationVel = glm::length (angVelA);
   //m_sB->m_rotationVel = glm::length (angVelB);

   /*
   glm::dvec3 rotationAxis;
      if (ss->m_rotationAxis.x != 0 || ss->m_rotationAxis.y != 0 || ss->m_rotationAxis.z != 0) {
         rotationAxis = glm::normalize (ss->m_rotationAxis);
      } else {
         rotationAxis = glm::dvec3{1,0,0};
      }
      ss->m_orientation = glm::angleAxis (ss->m_rotationVel, rotationAxis) * ss->m_orientation;
   */
}
