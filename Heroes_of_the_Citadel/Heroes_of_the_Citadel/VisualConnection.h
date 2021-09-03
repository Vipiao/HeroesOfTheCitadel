#pragma once

#include "Ball.h"

class VisualConnection {
private:
public:
   Ball* aa{};
   Ball* bb{};
   double radius{1.0};

   Mesh* m_mesh{};

   size_t m_index{};
};

