


#include "Hash.h"






double Hash::toUnit (uint64_t integer) {
   return (double)integer / (double)0xffffffffffffffff;
}

uint64_t Hash::rotate (uint64_t x, uint64_t b) {
   return (x << b) ^ (x >> (64 - b));
}

uint64_t Hash::pcg (uint64_t a) {
   uint64_t b{ a };
   for (size_t ii = 0; ii < 3; ii++) {
      a = Hash::rotate ((a ^ 0xcafebabe) + (b ^ 0xdeadbeef), 23);
      b = Hash::rotate ((a ^ 0xcabba6e5) + (b ^ 0xb01dface), 5);
   }
   return a ^ b;
}

double Hash::pcgUnit (uint64_t a) {
   return Hash::toUnit (Hash::pcg (a));
}

uint64_t Hash::pcg (uint64_t a, uint64_t b) {
   for (size_t ii = 0; ii < 3; ii++) {
      a = Hash::rotate ((a ^ 0xcafebabe) + (b ^ 0xdeadbeef), 23);
      b = Hash::rotate ((a ^ 0xcabba6e5) + (b ^ 0xb01dface), 5);
   }
   return a ^ b;
}

double Hash::pcgUnit (uint64_t a, uint64_t b) {
   return Hash::toUnit (Hash::pcg (a, b));
}

uint64_t Hash::pcg (uint64_t a, uint64_t b, uint64_t c) {
   for (size_t ii = 0; ii < 3; ii++) {
      a = Hash::rotate ((a ^ 0xcafebabe) + (b ^ 0xdeadbeef) + (c ^ 0x0b5e55ed), 23);
      b = Hash::rotate ((a ^ 0xcabba6e5) + (b ^ 0xb01dface) + (c ^ 0x0b5e55ed), 5);
      c = Hash::rotate ((a ^ 0xba5eba11) + (b ^ 0x6a5f1e1d) + (c ^ 0xdead50fa), 17);
   }
   // https://www.dcode.fr/words-containing
   return a ^ b ^ c;
}

double Hash::pcgUnit (uint64_t a, uint64_t b, uint64_t c) {
   return Hash::toUnit (Hash::pcg (a, b, c));
}
