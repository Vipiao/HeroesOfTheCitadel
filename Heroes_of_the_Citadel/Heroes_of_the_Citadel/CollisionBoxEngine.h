#pragma once

#include <vector>

#include "Sphere.h"

class Edge {
private:
   double v{0};
   void* pointer;
public:
};

class CollisionBox {
private:
   Edge* xMin{ nullptr };
   Edge* xMax{ nullptr };
   Edge* yMin{ nullptr };
   Edge* yMax{ nullptr };
   Edge* zMin{ nullptr };
   Edge* zMax{ nullptr };
public:
   friend class CollisionBoxEngine;
};

class CollisionBoxEngine {
private:
   std::vector<Edge*> edgesX{};
   std::vector<Edge*> edgesY{};
   std::vector<Edge*> edgesZ{};
public:
};

