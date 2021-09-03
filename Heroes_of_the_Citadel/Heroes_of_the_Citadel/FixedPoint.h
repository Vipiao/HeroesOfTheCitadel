#pragma once





#include <stdint.h>

class FixedPoint {
private:
   int64_t m_v{ 0 };

   static const int m_numDecimals{32};
public:
   static const int64_t m_MAX_VALUE{ (int64_t)0x7fffffffffffffff };
   static const int64_t m_MIN_VALUE{ (int64_t)0x8000000000000000 };

   FixedPoint ();
   FixedPoint (int v);
   FixedPoint (int64_t v);
   FixedPoint (uint64_t v);
   FixedPoint (unsigned long v);
   FixedPoint (double v);
   static FixedPoint initRaw (int64_t v);
   friend FixedPoint operator+(FixedPoint a, FixedPoint b);
   friend FixedPoint operator-(FixedPoint a, FixedPoint b);
   friend bool operator<(FixedPoint a, FixedPoint b);
   friend bool operator>(FixedPoint a, FixedPoint b);
   FixedPoint operator-() const;
   friend FixedPoint operator*(FixedPoint a, FixedPoint b);
   friend FixedPoint operator/(FixedPoint a, FixedPoint b);
   friend bool operator==(FixedPoint a, FixedPoint b);
   static FixedPoint sqrt (FixedPoint a);
   double toDouble ();
};

