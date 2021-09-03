







#include "FixedPoint.h"


#include <ctgmath>
#include <limits.h>




FixedPoint::FixedPoint () {}

FixedPoint::FixedPoint (int v) {
   int64_t vv = v;
   if (vv > 0x000000007fffffff) {
      // Overflow.
      m_v = 0x7fffffffffffffff;
   } else if (vv < (int64_t)0xffffffff80000000) {
      // Underflow.
      m_v = 0x8000000000000000;
   } else {
      m_v = vv << 32;
   }
}

FixedPoint::FixedPoint (int64_t v) {
   if (v > 0x000000007fffffff) {
      // Overflow.
      m_v = 0x7fffffffffffffff;
   } else if (v < (int64_t)0xffffffff80000000) {
      // Underflow.
      m_v = 0x8000000000000000;
   } else {
      m_v = v << 32;
   }
}

FixedPoint::FixedPoint (uint64_t v) {
   if (v > 0x000000007fffffff) {
      // Overflow.
      m_v = 0x7fffffffffffffff;
   } else {
      m_v = v << 32;
   }
}

FixedPoint::FixedPoint (unsigned long v) {
   if (v > 0x000000007fffffff) {
      // Overflow.
      m_v = 0x7fffffffffffffff;
   } else {
      m_v = (int64_t)v << 32;
   }
}

FixedPoint::FixedPoint (double v) {
   if (v > pow (2, 31) - 1) {
      m_v = 0x7fffffffffffffff;
   } else if (v < -pow(2,31)) {
      m_v = 0x8000000000000000;
   } else {
      m_v = (int64_t)(v * pow (2, m_numDecimals));
   }
}

FixedPoint FixedPoint::initRaw (int64_t v) {
   FixedPoint r{};
   r.m_v = v;
   return r;
}

FixedPoint operator+(FixedPoint a, FixedPoint b) {
   FixedPoint f;
   // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-multiply-overflow
   if ((b.m_v > 0) && (a.m_v > (int64_t)0x7fffffffffffffff - b.m_v)) {
      f.m_v = 0x7fffffffffffffff; // Overflow.
      return f;
   }
   if ((b.m_v < 0) && (a.m_v < (int64_t)0x8000000000000000 - b.m_v)) {
      f.m_v = 0x8000000000000000; // Underflow.
      return f;
   }
   f.m_v = a.m_v + b.m_v;
   return f;
}

FixedPoint operator-(FixedPoint a, FixedPoint b) {
   FixedPoint f;
   // https://stackoverflow.com/questions/199333/how-do-i-detect-unsigned-integer-multiply-overflow
   if ((b.m_v < 0) && (a.m_v > (int64_t)0x7fffffffffffffff + b.m_v)) {
      f.m_v = 0x7fffffffffffffff; // Overflow.
      return f;
   }
   if ((b.m_v > 0) && (a.m_v < (int64_t)0x8000000000000000 + b.m_v)) {
      f.m_v = 0x8000000000000000; // Underflow.
      return f;
   }
   f.m_v = a.m_v - b.m_v;
   return f;
}

bool operator<(FixedPoint a, FixedPoint b) {
   return a.m_v < b.m_v;
}

bool operator>(FixedPoint a, FixedPoint b) {
   return a.m_v > b.m_v;
}

FixedPoint FixedPoint::operator-() const {
   FixedPoint f{};
   f.m_v = -m_v;
   return f;
}

FixedPoint operator*(FixedPoint a, FixedPoint b) {
   bool isNegative = a.m_v < 0 != b.m_v < 0;
   a.m_v = std::abs (a.m_v);
   b.m_v = std::abs (b.m_v);

   uint64_t x1y1;
   uint64_t x1 = a.m_v >> 32;
   uint64_t y1 = b.m_v >> 32;
   x1y1 = x1 * y1;
   if (x1y1 > 0x000000007fffffff) { // 0x000000007fffffff = pow(2,31)-1.
      if (isNegative) {
         if (x1y1 > 0x0000000080000000) { // (int64_t)0xffffffff80000000 = -pow (2, 31)
            // Underflow.
            return FixedPoint::initRaw(FixedPoint::m_MIN_VALUE);
         }
      } else {
         // Overflow.
         return FixedPoint::initRaw (FixedPoint::m_MAX_VALUE);
      }
   }
   x1y1 <<= 32;

   int64_t x2y2;
   int64_t x2 = a.m_v & 0x00000000ffffffff;
   int64_t y2 = b.m_v & 0x00000000ffffffff;
   x2y2 = (x2 * y2) >> 32;

   int64_t x1y2;
   x1y2 = x1 * y2;

   int64_t x2y1;
   x2y1 = x2 * y1;

   if (((a.m_v | b.m_v) & 0x7fff800000000000) == 0) {
      // Will not overflow as the product of two numbers cannot increase by more than the combined
      // number of digits.
      return FixedPoint::initRaw ((x1y1 + x2y2 + x1y2 + x2y1) * (1 - 2 * (int64_t)isNegative));
   }

   FixedPoint result = FixedPoint::initRaw( x1y1 ) +
      FixedPoint::initRaw (x2y2) +
      FixedPoint::initRaw (x1y2) +
      FixedPoint::initRaw (x2y1);
   result.m_v *= (1 - 2 * (int64_t)isNegative);

   return result;
}
FixedPoint operator/(FixedPoint a, FixedPoint b) {
   if (b.m_v == 0) {
      throw std::runtime_error ("ERROR: Cannot divide by zero.");
   }
   bool isNegative = a.m_v < 0 != b.m_v < 0;
   a.m_v = std::abs (a.m_v);
   b.m_v = std::abs (b.m_v);
   FixedPoint c{ a.toDouble () / b.toDouble() };
   FixedPoint bc = b * c;
   if (bc < a) {
      if (c.m_v == (int64_t)0x7fffffffffffffff) {
         if (isNegative) {
            c.m_v = 0x8000000000000000;
         } else {
            c.m_v = 0x7fffffffffffffff;
         }
         return c;
      }
      c.m_v++;
      while ((bc = b * c) < a) {
         c.m_v++;
      }
      if (bc == a) {
         return c * (1 - 2 * (int64_t)isNegative);
      }
      c.m_v--;
      return c * (1 - 2 * (int64_t)isNegative);
   } else if (bc > a) {
      c.m_v--;
      while (b * c > a) {
         c.m_v--;
      }
      return c * (1 - 2 * (int64_t)isNegative);
   } else {
      return c * (1 - 2 * (int64_t)isNegative);
   }
}

bool operator==(FixedPoint a, FixedPoint b) {
   return a.m_v == b.m_v;
}

FixedPoint FixedPoint::sqrt (FixedPoint a) {
   if (a.m_v < 0) {
      throw std::runtime_error ("ERROR: Cannot calculate square root of negative number.");
   }
   FixedPoint r{ std::sqrt (a.toDouble ()) };
   FixedPoint rr = r*r;
   if (rr < a) {
      r.m_v++;
      while ((rr = r*r) < a) {
         r.m_v++;
      }
      if (rr == a) {
         return r;
      }
      r.m_v--;
      return r;
   } else if (rr > a) {
      r.m_v--;
      while (r * r > a) {
         r.m_v--;
      }
      return r;
   } else {
      return r;
   }
}

double FixedPoint::toDouble () {
   return ((double)m_v) / pow (2, m_numDecimals);
}