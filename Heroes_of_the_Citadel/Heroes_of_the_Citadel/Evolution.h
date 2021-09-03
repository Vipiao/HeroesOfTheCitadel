#pragma once

#include "Game.h"
#include "InputHandler.h"

class Evolution: public UpdateCallback {
private:
   Game* m_game{};
   InputHandler m_inputHandler{};
public:
   Ball* m_dragBall{ nullptr };
   double m_dragBallDist{ 0 };
   glm::dvec3 m_prevTargetPos{};
   int m_grabTime{ 0 };

   Evolution ();
   ~Evolution ();
   void run ();
   void updateCallback ();
};

