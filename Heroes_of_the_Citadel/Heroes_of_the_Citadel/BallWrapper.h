#pragma once

#include "Ball.h"
#include "VisualConnection.h"

//class SphereWrapper : public Sphere {
//   friend class Sphere;


class BallWrapper : public Ball {
   friend class Ball;
private:
public:
   std::vector<VisualConnection*> m_visualConnections;
};

