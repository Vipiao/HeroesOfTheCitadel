#include "RandomNumberGenerator.h"

#include "Hash.h"

RandomNumberGenerator::RandomNumberGenerator () {}

RandomNumberGenerator::RandomNumberGenerator (uint64_t newSeed) {
   m_seed = newSeed;
}

void RandomNumberGenerator::setSeed (uint64_t newSeed) {
   m_seed = newSeed;
   m_iterator = 0;
}

uint64_t RandomNumberGenerator::getNext () {
   m_seed = Hash::pcg (m_seed + m_iterator++);
   return m_seed;
}

double RandomNumberGenerator::getNextUnit () {
   m_seed = Hash::pcg (m_seed + m_iterator++);
   return Hash::toUnit (m_seed);;
}
