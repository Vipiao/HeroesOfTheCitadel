#pragma once




#include <cstdint>






class Hash {
private:
   static uint64_t rotate (uint64_t x, uint64_t b);
public:
   static double toUnit (uint64_t integer);
   static uint64_t pcg (uint64_t a);
   static double pcgUnit (uint64_t a);
   static uint64_t pcg (uint64_t a, uint64_t b);
   static double pcgUnit (uint64_t a, uint64_t b);
   static uint64_t pcg (uint64_t a, uint64_t b, uint64_t c);
   static double pcgUnit (uint64_t a, uint64_t b, uint64_t c);
};

