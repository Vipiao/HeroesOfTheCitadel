









#include "DistanceConstraint.h"

#include "DebugInfo.h"











void DistanceConstraint::measureMovementError () {
   glm::dvec3 normal{};
   double impulse{ calculateForce (&normal) };
   m_compensator += impulse;
}

double DistanceConstraint::calculateForce (glm::dvec3* normalOut) {
   glm::dvec3 diff = m_sA->m_position - m_sB->m_position;
   double distBetween = glm::length (diff);
   if (distBetween > 0.0) {
      glm::dvec3 normal = diff / distBetween;
      (*normalOut) = normal;
      glm::dvec3 relVel = m_sB->m_velocity - m_sA->m_velocity;
      double relVelTowards = glm::dot (relVel, normal);
      double collisionMass = 1.0 / (1.0 / m_sA->getMass () + 1.0 / m_sB->getMass ());
      double impulse{ relVelTowards * collisionMass };

      return impulse;
   }

   return 0.0;
}

void DistanceConstraint::constrainMomentum (bool useCompensator, double compensatorFactor) {
   glm::dvec3 normal{};
   double impulse{calculateForce(&normal)};
   if (useCompensator) {
      impulse += m_compensator * compensatorFactor;
   }
   glm::dvec3 impulseVec = impulse * normal;
   //DebugInfo::vectors.push_back (DebugVector{
   //   m_sA->m_position + glm::dvec3 {0, 0, m_sA->getRadius () * 1.5 },
   //   impulseVec*10.
   //   });
   //DebugInfo::vectors.push_back (DebugVector{
   //   m_sB->m_position + glm::dvec3 {0, 0, m_sB->getRadius () * 1.5 },
   //   -impulseVec * 10.
   //   });
   m_sA->m_velocity += impulseVec / m_sA->getMass ();
   m_sB->m_velocity -= impulseVec / m_sB->getMass ();
   
   //DebugInfo::vectors.push_back (DebugVector{ m_sA->m_position + glm::dvec3 {m_sA->getRadius()*1.5, 0, 0.0 }, impulseVec * 1000. });
}

void DistanceConstraint::constrainPosition () {
   glm::dvec3 diff = m_sA->m_position - m_sB->m_position;
   double dist = glm::length (diff);
   if (dist > 0.0) {
      glm::dvec3 dir = diff / dist;
      glm::dvec3 target = dir * m_distance;
      glm::dvec3 movement = target - diff;
      double factor = 0.5;
      movement *= factor;
      double collisionMass = 1.0 / (1.0 / m_sA->getMass () + 1.0 / m_sB->getMass ());
      m_sA->m_position += movement * collisionMass / m_sA->getMass ();
      m_sB->m_position -= movement * collisionMass / m_sB->getMass();
   }
}
