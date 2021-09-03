#pragma once


#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MeshGenerator {
private:
public:
   static void cube (std::vector<glm::vec3>* vertices, std::vector<int>* indices);
   static void cube (std::vector<double>* vertexData, std::vector<int>* indices);
   static void sphere (
      std::vector<double>* vertexData, std::vector<int>* indices, int resolution, double radius
   );
   static void plane (std::vector<double>* vertexData, std::vector<int>* indices);
   static void crosshair (std::vector<double>* vertexData, std::vector<int>* indices);
};

