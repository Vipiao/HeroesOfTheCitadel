#pragma once

#include "Mesh.h"
#include "Sphere.h"


class Ball {
private:
public:
   Mesh* m_mesh{};
   Sphere* m_sphere{};
   size_t m_index{ 0 };
};

