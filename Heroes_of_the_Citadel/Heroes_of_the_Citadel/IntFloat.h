#pragma once



#include <stdint.h>


class IntFloat {
private:
   int64_t m_exponent{ 0 };
   int64_t m_mantisa{ 0 };
   static const int m_exponentSize{ 64 };
   static const int m_mantisaSize{ 64 };
   
   static inline int mostSignificantBitIndex (uint64_t v);

public:
   IntFloat ();
   IntFloat (int64_t exponent, int64_t mantisa);
   IntFloat (double v);
   friend IntFloat operator+(IntFloat a, IntFloat b);
   friend IntFloat operator-(IntFloat a, IntFloat b);
   IntFloat operator-() const;
   friend bool operator==(IntFloat a, IntFloat b);
   double toDouble ();
};

IntFloat operator+(IntFloat a, IntFloat b);

