#pragma once

#include <vector>

#include "Game.h"

class RagdollCreator {
private:
   static glm::dquat lookAtOrientation (glm::dvec3 pos, glm::dvec3 target, glm::dvec3 up);
   static glm::dquat turnAngle (glm::dvec3 posA, glm::dvec3 posB);
public:
   static std::vector<Ball*> createHuman (Game* game, glm::dvec3 offset, double size);
};

