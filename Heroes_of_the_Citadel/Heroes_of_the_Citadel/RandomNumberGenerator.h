#pragma once

#include <cstdint>



class RandomNumberGenerator {
private:
   uint64_t m_seed{0};
   uint64_t m_iterator{ 0 };
public:
   RandomNumberGenerator ();
   RandomNumberGenerator (uint64_t newSeed);
   void setSeed (uint64_t newSeed);
   uint64_t getNext ();
   double getNextUnit ();

};

