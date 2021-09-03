#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DebugVector {
private:
public:
   glm::dvec3 m_startPosition{};
   glm::dvec3 m_direction{};
   double m_size{1.0};
   glm::vec3 m_color{1,0,0};
};

class DebugInfo {
private:
public:
   //static Game* globalGame;
   static std::vector<DebugVector> vectors;
};

