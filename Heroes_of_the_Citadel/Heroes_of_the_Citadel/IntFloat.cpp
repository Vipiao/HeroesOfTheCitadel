






#include "IntFloat.h"

#include <ctgmath>

inline int IntFloat::mostSignificantBitIndex (uint64_t v) {
   double ff = (double)(v | 1);

   uint32_t tmp;
   memcpy (&tmp, ((const char*)& ff) + sizeof (uint32_t), sizeof (uint32_t));
   return (tmp >> 20) - 1023;
}

IntFloat::IntFloat () {

}

IntFloat::IntFloat (int64_t exponent, int64_t mantisa) {
   m_exponent = exponent;
   m_mantisa = mantisa;
}

IntFloat::IntFloat (double v) {
   if (v == 0.) {
      m_exponent = m_mantisa = 0;
   } else {
      m_exponent = (int64_t)floor(log2 (std::abs(v)));
      if (m_exponent != 0) {
         m_mantisa = (int64_t)((v / pow(2, m_exponent)) * pow (2, m_mantisaSize - 2));
      } else {
         m_mantisa = (int64_t)(v * pow (2., m_mantisaSize - 2));
      }
   }
}

IntFloat IntFloat::operator-() const {
   return IntFloat(m_exponent, -m_mantisa);
}

double IntFloat::toDouble () {
   return m_mantisa / pow (2, m_mantisaSize - 2) * pow (2, m_exponent);
}

IntFloat operator+(IntFloat a, IntFloat b) {
   int64_t resultExponent;
   int64_t resultMantisa;
   if (a.m_exponent > b.m_exponent) {
      resultExponent = a.m_exponent + 1;
      //a.m_mantisa >>= 1;
      //b.m_mantisa >>= 1 + a.m_exponent - b.m_exponent;
      resultMantisa = (a.m_mantisa >> 1) + (b.m_mantisa >> (1 + a.m_exponent - b.m_exponent));
   } else {
      resultExponent = b.m_exponent + 1;
      //a.m_mantisa >>= 1 + b.m_exponent - a.m_exponent;
      //b.m_mantisa >>= 1;
      resultMantisa = (b.m_mantisa >> 1) + (a.m_mantisa >> (1 + b.m_exponent - a.m_exponent));
   }
   //int64_t resultMantisa = a.m_mantisa + b.m_mantisa;
   if (resultMantisa == 0) {
      return IntFloat (0, 0);
   }

   int shift = (int64_t)62 - IntFloat::mostSignificantBitIndex (std::abs(resultMantisa));
   resultMantisa <<= shift;
   resultExponent -= shift;
   return IntFloat (resultExponent, resultMantisa);

   /*if (std::abs (resultMantisa) < 0x4000000000000000) {
      resultMantisa <<= 1;
      resultExponent -= 1;
   } else {
      return IntFloat (resultExponent, resultMantisa);
   }
   if (std::abs (resultMantisa) < 0x0000000080000000) {
      resultMantisa <<= 32;
      resultExponent -= 32;
   }
   if (std::abs (resultMantisa) < 0x0000800000000000) {
      resultMantisa <<= 16;
      resultExponent -= 16;
   }
   if (std::abs (resultMantisa) < 0x0080000000000000) {
      resultMantisa <<= 8;
      resultExponent -= 8;
   }
   if (std::abs (resultMantisa) < 0x0800000000000000) {
      resultMantisa <<= 4;
      resultExponent -= 4;
   }
   if (std::abs (resultMantisa) < 0x2000000000000000) {
      resultMantisa <<= 2;
      resultExponent -= 2;
   }
   if (std::abs (resultMantisa) < 0x4000000000000000) {
      resultMantisa <<= 1;
      resultExponent -= 1;
   }
   return IntFloat (resultExponent, resultMantisa);*/
}

IntFloat operator-(IntFloat a, IntFloat b) {
   return a+(-b);
}

bool operator==(IntFloat a, IntFloat b) {
   return a.m_exponent == b.m_exponent && a.m_mantisa == b.m_mantisa;
}
