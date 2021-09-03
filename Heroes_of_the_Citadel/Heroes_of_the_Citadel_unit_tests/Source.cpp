#include "pch.h"
#include "CppUnitTest.h"

#include "../Heroes_of_the_Citadel/IntFloat.h"

#include <vector>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HeroesoftheCitadelunittests {
   TEST_CLASS (HeroesoftheCitadelunittests) {
public:

   TEST_METHOD (Conversion) {
      std::vector<std::pair<double, IntFloat>> inputs{
         std::pair<double, IntFloat>{        1.0, IntFloat{  0, (int64_t)0x4000000000000000 }},
         std::pair<double, IntFloat>{        0.0, IntFloat{  0, (int64_t)0x0000000000000000 }},
         std::pair<double, IntFloat>{       10.0, IntFloat{  3, (int64_t)0x5000000000000000 }},
         std::pair<double, IntFloat>{       -1.0, IntFloat{  0, (int64_t)0xc000000000000000 }},
         // 0111 1011 0000..
         std::pair<double, IntFloat>{      123.0, IntFloat{  6, (int64_t)0x7b00000000000000 }},
         // 1000 0101 0000..
         std::pair<double, IntFloat>{     -123.0, IntFloat{  6, (int64_t)0x8500000000000000 }},
         std::pair<double, IntFloat>{999999999.0, IntFloat{ 29, (int64_t)0x773593fe00000000 }},
         std::pair<double, IntFloat>{        0.5, IntFloat{ -1, (int64_t)0x4000000000000000 }},
         std::pair<double, IntFloat>{       -0.5, IntFloat{ -1, (int64_t)0xc000000000000000 }},
         std::pair<double, IntFloat>{        0.1, IntFloat{ -4, (int64_t)0x6666666666666800 }},
         std::pair<double, IntFloat>{       -0.1, IntFloat{ -4, (int64_t)0x9999999999999800 }},
         std::pair<double, IntFloat>{     0.0625, IntFloat{ -4, (int64_t)0x4000000000000000 }},
         std::pair<double, IntFloat>{    -0.0625, IntFloat{ -4, (int64_t)0xc000000000000000 }},
         std::pair<double, IntFloat>{     0.0123, IntFloat{ -7, (int64_t)0x64c2f837b4a23400 }},
         std::pair<double, IntFloat>{    -0.0123, IntFloat{ -7, (int64_t)0x9b3d07c84b5dcc00 }},
      };
      for (size_t ii = 0; ii < inputs.size (); ii++) {
         double normalFloat = inputs[ii].first;
         IntFloat customFloat = inputs[ii].second;
         Assert::IsTrue (normalFloat == customFloat);

         double convertedBack = customFloat.toDouble ();
         convertedBack = convertedBack;
         Assert::AreEqual (normalFloat, convertedBack);
      }
   }
   TEST_METHOD (Summation) {
      {
         IntFloat a{ 0.0 };
         Assert::IsTrue (a == -(-a));
      }
      {
         IntFloat a{ 10.0 };
         Assert::IsTrue (-a == -10.0);
      }
      {
         IntFloat a{ 123.123 };
         IntFloat b{ -123.123 };
         Assert::IsTrue (-a == b);
         Assert::IsTrue (-b == a);
      }
      {
         IntFloat a{ 0.0 };
         IntFloat b{ 0.0 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (a - b == 0.0);
      }
      {
         IntFloat a{ 1.0 };
         IntFloat b{ 1.0 };
         Assert::IsTrue (a + b == 2.0);
         Assert::IsTrue (a - b == 0.0);
      }
      {
         IntFloat a{ 2.0 };
         IntFloat b{ 1.0 };
         Assert::IsTrue (a + b == 3.0);
         Assert::IsTrue (b + a == 3.0);
         Assert::IsTrue (a - b == 1.0);
         Assert::IsTrue (b - a == -1.0);
      }
      {
         IntFloat a{ 1.0 };
         IntFloat b{ -1.0 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (b + a == 0.0);
         Assert::IsTrue (a - b == 2.0);
         Assert::IsTrue (b - a == -2.0);
      }
      {
         IntFloat a{ 10.0 };
         IntFloat b{ -5.0 };
         Assert::IsTrue (a + b == 5.0);
         Assert::IsTrue (b + a == 5.0);
         Assert::IsTrue (a - b == 15.0);
         Assert::IsTrue (b - a == -15.0);
      }
      {
         IntFloat a{ 10.0 };
         IntFloat b{ -5.3 };
         IntFloat c{ -0.000321 };
         Assert::IsTrue (std::abs ((a + b + c).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((a + c + b).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((b + a + c).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((b + c + a).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((c + a + b).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((c + b + a).toDouble () - 4.699679) < 0.0000001);
      }
      {
         IntFloat a{ 12345.25 };
         IntFloat b{ -12345.25 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (b + a == 0.0);
         Assert::IsTrue (a - b == 24690.5);
         Assert::IsTrue (b - a == -24690.5);
      }
      {
         IntFloat a{ 3827584019284758.0625 };
         IntFloat b{ -3827584019284758.0625 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (b + a == 0.0);
      }
      {
         IntFloat a{ 38275019284758.0625 };
         IntFloat b{ -38275019284758.125 };
         Assert::IsTrue (a + b == -0.0625);
         Assert::IsTrue (b + a == -0.0625);
         double test = (a + b).toDouble ();
         test = test;
      }
      {
         IntFloat a{ 382019284758.0625 };
         IntFloat b{ -382019284758.125 };
         Assert::IsTrue (a + b == -0.0625);
         Assert::IsTrue (b + a == -0.0625);
         double test = (a + b).toDouble ();
         test = test;
      }
   }
   };
}
