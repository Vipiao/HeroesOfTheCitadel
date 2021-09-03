#include "pch.h"
#include "CppUnitTest.h"

#include "Hash.h"

#include <vector>
#include <iostream>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Hash_unit_test {
   TEST_CLASS (HeroesoftheCitadelunittests) {
public:

   TEST_METHOD (distribution) {
      {
         double randNum = Hash::pcgUnit (0);
         Assert::IsTrue (randNum >= 0 && randNum < 1.0);
      }
      {
         std::vector<int> baskets{0,0,0,0,0,0,0,0,0,0};
         for (size_t ii = 0; ii < baskets.size() * 1e5; ii++) {
            double randNum = Hash::pcgUnit (ii);
            Assert::IsTrue (randNum >= 0.0 && randNum < 1.0);
            baskets[(int)(randNum * (double)baskets.size ())] ++;
         }
         for (size_t ii = 0; ii < baskets.size (); ii++) {
            Assert::IsTrue (baskets[ii] > 9e4 && baskets[ii] < 11e4);
         }
      }
   }
   };
}
