#include "RagdollCreator.h"




glm::dquat RagdollCreator::lookAtOrientation (glm::dvec3 pos, glm::dvec3 target, glm::dvec3 up) {
   glm::dquat result{ glm::lookAt (pos, target, up) };
   //result = glm::lookAt (glm::dvec3{ 0 }, glm::dvec3{ 0,1,0 }, glm::dvec3{0,0,1});
   result = glm::conjugate (result);
   //result = glm::rotate (result, glm::radians (-90.), glm::dvec3{ 1,0,0 });
   result = result * glm::angleAxis (glm::radians (-90.), glm::dvec3{ 1,0,0 });
   return result;
}

glm::dquat RagdollCreator::turnAngle (glm::dvec3 posA, glm::dvec3 posB) {
   glm::dvec3 diff{ posB - posA };
   double dd{ glm::dot (glm::dvec3{ 0,0,1 }, diff) };
   double diffLengthSqr = glm::length2 (diff);
   if (diffLengthSqr == 0.0) {
      return glm::dquat{1,0,0,0};
   } else {
      glm::dvec3 axis = glm::cross (glm::dvec3{0,0,1}, diff);
      double axisLengthSqr = glm::length2 (axis);
      if (axisLengthSqr == 0) {
         axis = glm::dvec3{1,0,0};
      } else {
         axis = axis / glm::sqrt (axisLengthSqr);
      }
      double angle{ glm::acos(dd / glm::sqrt (diffLengthSqr)) };
      return glm::angleAxis (angle, axis);
   }
}

std::vector<Ball*> RagdollCreator::createHuman (Game* game, glm::dvec3 offset, double size) {

   std::vector<Ball*> balls{};

   // Create balls.
   Ball* hipCenter{ game->createBall () };
   hipCenter->m_sphere->m_position = glm::dvec3{ 0, 0, 0 } *size;
   hipCenter->m_sphere->setRadiusDensity (size * 0.15, 1.0);
   balls.push_back (hipCenter);

   Ball* hipRight{ game->createBall () };
   hipRight->m_sphere->m_position = glm::dvec3{ 0.2, 0, -0.2 } *size;
   hipRight->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (hipRight);

   Ball* hipLeft{ game->createBall () };
   hipLeft->m_sphere->m_position = glm::dvec3{ -0.2, 0, -0.2 } *size;
   hipLeft->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (hipLeft);

   Ball* kneeRight{ game->createBall () };
   kneeRight->m_sphere->m_position = glm::dvec3{ 0.2, 0, -0.8 } *size;
   kneeRight->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (kneeRight);

   Ball* kneeLeft{ game->createBall () };
   kneeLeft->m_sphere->m_position = glm::dvec3{ -0.2, 0, -0.8 } *size;
   kneeLeft->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (kneeLeft);

   Ball* ancleRight{ game->createBall () };
   ancleRight->m_sphere->m_position = glm::dvec3{ 0.2, 0, -1.6 } *size;
   ancleRight->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (ancleRight);

   Ball* ancleLeft{ game->createBall () };
   ancleLeft->m_sphere->m_position = glm::dvec3{ -0.2, 0, -1.6 } *size;
   ancleLeft->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (ancleLeft);

   Ball* lowerTorso{ game->createBall () };
   lowerTorso->m_sphere->m_position = glm::dvec3{ 0, 0, 0.3 } *size;
   lowerTorso->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   balls.push_back (lowerTorso);

   Ball* upperTorso{ game->createBall () };
   upperTorso->m_sphere->m_position = glm::dvec3{ 0, 0, 0.6 } *size;
   upperTorso->m_sphere->setRadiusDensity (size * 0.15, 1.0);
   balls.push_back (upperTorso);

   Ball* neck{ game->createBall () };
   neck->m_sphere->m_position = glm::dvec3{ 0, 0, 0.85 } *size;
   neck->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (neck);

   Ball* leftSholder{ game->createBall () };
   leftSholder->m_sphere->m_position = glm::dvec3{ -0.3, 0, 0.85 } *size;
   leftSholder->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (leftSholder);

   Ball* rightSholder{ game->createBall () };
   rightSholder->m_sphere->m_position = glm::dvec3{ 0.3, 0, 0.85 } *size;
   rightSholder->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (rightSholder);

   Ball* leftElbow{ game->createBall () };
   leftElbow->m_sphere->m_position = glm::dvec3{ -0.85, 0, 0.85 } *size;
   leftElbow->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (leftElbow);

   Ball* rightElbow{ game->createBall () };
   rightElbow->m_sphere->m_position = glm::dvec3{ 0.85, 0, 0.85 } *size;
   rightElbow->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (rightElbow);

   Ball* leftHand{ game->createBall () };
   leftHand->m_sphere->m_position = glm::dvec3{ -1.3, 0, 0.85 } *size;
   leftHand->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (leftHand);

   Ball* rightHand{ game->createBall () };
   rightHand->m_sphere->m_position = glm::dvec3{ 1.3, 0, 0.85 } *size;
   rightHand->m_sphere->setRadiusDensity (size * 0.075, 1.0);
   balls.push_back (rightHand);

   Ball* head{ game->createBall () };
   head->m_sphere->m_position = glm::dvec3{ 0, 0, 1.15 } *size;
   head->m_sphere->setRadiusDensity (size * 0.2, 1.0);
   balls.push_back (head);

   for (size_t ii = 0; ii < balls.size(); ii++) {
      balls[ii]->m_sphere->setMomentOfInertiaMultiplier (5.0);
   }

   // Create constraints.
   //{
   //   glm::dquat offsetOrientation{ RagdollCreator::turnAngle (hipCenter->m_sphere->m_position, hipRight->m_sphere->m_position) };
   //   game->createDirectionConstraint (hipCenter, hipRight, offsetOrientation, glm::dvec3{ 0 }, true);
   //   game->createDistanceConstraint (hipCenter, hipRight, glm::length (hipCenter->m_sphere->m_position - hipRight->m_sphere->m_position));
   //}
   //{
   //   glm::dquat offsetOrientation{ RagdollCreator::turnAngle (hipCenter->m_sphere->m_position, hipLeft->m_sphere->m_position) };
   //   game->createDirectionConstraint (hipCenter, hipLeft, offsetOrientation, glm::dvec3{ 0 }, true);
   //   game->createDistanceConstraint (hipCenter, hipLeft, glm::length (hipCenter->m_sphere->m_position - hipLeft->m_sphere->m_position));
   //}
   std::vector<Ball*> connections{
      hipCenter, hipLeft,
      hipCenter, hipRight,
      hipLeft, kneeLeft,
      kneeLeft, ancleLeft,
      hipRight, kneeRight,
      kneeRight, ancleRight,
      hipCenter, lowerTorso,
      lowerTorso, upperTorso,
      upperTorso, neck,
      neck, leftSholder,
      leftSholder, leftElbow,
      leftElbow, leftHand,
      neck, rightSholder,
      rightSholder, rightElbow,
      rightElbow, rightHand,
      neck, head,
   };
   for (size_t ii = 0; ii < connections.size(); ii += 2) {
      Ball* aa = connections[ii];
      Ball* bb = connections[ii+1];
      glm::dquat offsetOrientation{ RagdollCreator::turnAngle (aa->m_sphere->m_position, bb->m_sphere->m_position) };
      game->createDirectionConstraint (aa, bb, offsetOrientation, glm::dvec3{ 0 }, true);
      game->createDistanceConstraint (aa, bb, glm::length (aa->m_sphere->m_position - bb->m_sphere->m_position));
      glm::dquat offsetOrientation2{ RagdollCreator::turnAngle (bb->m_sphere->m_position, aa->m_sphere->m_position) };
      game->createDirectionConstraint (bb, aa, offsetOrientation2, glm::dvec3{ 0 }, true);

      game->createVisualConnection (aa, bb, glm::min(aa->m_sphere->getRadius(), bb->m_sphere->getRadius()) * 0.8);
   }

   for (size_t ii = 0; ii < balls.size(); ii++) {
      Sphere* ss = balls[ii]->m_sphere;
      ss->m_position += offset;
      if (balls[ii] != upperTorso && balls[ii] != neck && balls[ii] != head) {
         //game->removeBall (balls[ii]);
      }
   }
   std::vector<Ball*> removeBalls{
      lowerTorso, hipCenter, hipRight,hipLeft,kneeLeft,kneeRight,ancleLeft, ancleRight, head, upperTorso,rightHand,leftHand, leftElbow
   };
   for (size_t ii = 0; ii < removeBalls.size (); ii++) {
      //game->removeBall (removeBalls[ii]);
   }

   //Ball* test{ game->createBall () };
   //test->m_sphere->m_position = glm::dvec3{ 0, 0.01, 1.15 } *size + offset;
   //test->m_sphere->setRadiusDensity (size * 0.1, 1.0);
   //balls.push_back (test);


   return balls;
}
