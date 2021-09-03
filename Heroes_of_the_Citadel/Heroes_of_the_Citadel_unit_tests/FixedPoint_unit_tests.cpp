#include "pch.h"
#include "CppUnitTest.h"

#include "FixedPoint.h"

#include <vector>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HeroesoftheCitadelunittests {
   TEST_CLASS (FixedPoint_unit_test) {
public:
   TEST_METHOD (Summation) {
      {
         FixedPoint a{ 0.0 };
         Assert::IsTrue (a == -(-a));
      }
      {
         FixedPoint a{ 10.0 };
         Assert::IsTrue (-a == -10.0);
      }
      {
         FixedPoint a{ 123.123 };
         FixedPoint b{ -123.123 };
         Assert::IsTrue (-a == b);
         Assert::IsTrue (-b == a);
      }
      {
         FixedPoint a{ 0.0 };
         FixedPoint b{ 0.0 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (a - b == 0.0);
      }
      {
         FixedPoint a{ 1.0 };
         FixedPoint b{ 1.0 };
         Assert::IsTrue (a + b == 2.0);
         Assert::IsTrue (a - b == 0.0);
      }
      {
         FixedPoint a{ 2.0 };
         FixedPoint b{ 1.0 };
         Assert::IsTrue (a + b == 3.0);
         Assert::IsTrue (b + a == 3.0);
         Assert::IsTrue (a - b == 1.0);
         Assert::IsTrue (b - a == -1.0);
      }
      {
         FixedPoint a{ 1.0 };
         FixedPoint b{ -1.0 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (b + a == 0.0);
         Assert::IsTrue (a - b == 2.0);
         Assert::IsTrue (b - a == -2.0);
      }
      {
         FixedPoint a{ 10.0 };
         FixedPoint b{ -5.0 };
         Assert::IsTrue (a + b == 5.0);
         Assert::IsTrue (b + a == 5.0);
         Assert::IsTrue (a - b == 15.0);
         Assert::IsTrue (b - a == -15.0);
      }
      {
         FixedPoint a{ 10.0 };
         FixedPoint b{ -5.3 };
         FixedPoint c{ -0.000321 };
         Assert::IsTrue (std::abs ((a + b + c).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((a + c + b).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((b + a + c).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((b + c + a).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((c + a + b).toDouble () - 4.699679) < 0.0000001);
         Assert::IsTrue (std::abs ((c + b + a).toDouble () - 4.699679) < 0.0000001);
      }
      {
         FixedPoint a{ 12345.25 };
         FixedPoint b{ -12345.25 };
         Assert::IsTrue (a + b == 0.0);
         Assert::IsTrue (b + a == 0.0);
         Assert::IsTrue (a - b == 24690.5);
         Assert::IsTrue (b - a == -24690.5);
      }
      {
         FixedPoint a{ 2147483648 }; // Max size is 2147483647.9999... = pow(2,31)-1+0.9999...
         FixedPoint b{ -2147483648.0 }; // Max size is 2147483648.9999... = -pow(2,31)-0.9999...
         Assert::IsTrue (a+1 == a); // Cannot overflow.
         Assert::IsTrue (b-1 == b); // Cannot overflow.
      }
   }
   TEST_METHOD (Multiplication) {
      {
         FixedPoint a{ 0 };
         FixedPoint b{ 0 };
         Assert::IsTrue (a * b == 0);
      }
      {
         FixedPoint a{ 1 };
         FixedPoint b{ 0 };
         Assert::IsTrue (a * b == 0);
         Assert::IsTrue (b * a == 0);
      }
      {
         FixedPoint a{ 1 };
         FixedPoint b{ 1 };
         Assert::IsTrue (a * b == 1);
         Assert::IsTrue (b * a == 1);
      }
      {
         FixedPoint a{ -1 };
         FixedPoint b{ 0 };
         Assert::IsTrue (a * b == 0);
         Assert::IsTrue (b * a == 0);
      }
      {
         FixedPoint a{ -1 };
         FixedPoint b{ -1 };
         Assert::IsTrue (a * b == 1);
      }
      {
         FixedPoint a{ 1.5 };
         FixedPoint b{ 10.25 };
         Assert::IsTrue (a * b == 15.375);
         Assert::IsTrue (b * a == 15.375);
      }
      {
         FixedPoint a{ pow (2,16) };
         FixedPoint b{ pow (2,16) };
         Assert::IsTrue (a * b == FixedPoint::m_MAX_VALUE);
         Assert::IsTrue (a * (-b) == FixedPoint::m_MIN_VALUE);
         Assert::IsTrue ((-a) * b == FixedPoint::m_MIN_VALUE);
         Assert::IsTrue ((-a) * (-b) == FixedPoint::m_MAX_VALUE);
      }
      {
         FixedPoint a{ (double)0x7fffffff + (double)0xffffffff / pow(2,32) };
         FixedPoint b{ 2 };
         Assert::IsTrue (a * b == FixedPoint::m_MAX_VALUE);
         Assert::IsTrue (a * (-b) == FixedPoint::m_MIN_VALUE);
         Assert::IsTrue ((-a) * b == FixedPoint::m_MIN_VALUE);
         Assert::IsTrue ((-a) * (-b) == FixedPoint::m_MAX_VALUE);
      }
      {
         FixedPoint a{ 123.0625 };
         FixedPoint b{ -999.125 };
         Assert::IsTrue (std::abs((a * b -(-122954.820313)).toDouble()) < 0.000001);
         Assert::IsTrue (std::abs((b * a -(-122954.820313)).toDouble()) < 0.000001);
      }
   }
   TEST_METHOD (SquareRoot) {
      {
         FixedPoint a{ 123.0625 };
         FixedPoint b = FixedPoint::sqrt (a);
         Assert::IsTrue (std::abs ((b * b - a).toDouble ()) < 0.0000001);
      }
      {
         FixedPoint a{ 1.0 };
         FixedPoint b = FixedPoint::sqrt (a);
         Assert::IsTrue (std::abs ((b * b - a).toDouble ()) < 0.0000001);
      }
      {
         FixedPoint a{ 0.0 };
         FixedPoint b = FixedPoint::sqrt (a);
         Assert::IsTrue (std::abs ((b * b - a).toDouble ()) < 0.0000001);
      }
      {
         FixedPoint a{ 100.0 };
         FixedPoint b = FixedPoint::sqrt (a);
         Assert::IsTrue (std::abs ((b * b - a).toDouble ()) < 0.0000001);
      }
      {
         FixedPoint a{ 11111.11111 };
         FixedPoint b = FixedPoint::sqrt (a);
         Assert::IsTrue (std::abs ((b * b - a).toDouble ()) < 0.0000001);
      }
      {
         try {
            FixedPoint a{ -1 };
            FixedPoint b = FixedPoint::sqrt (a);
            Assert::Fail ();
         } catch (const std::exception& e) {
            Assert::AreEqual (e.what(), "ERROR: Cannot calculate square root of negative number.");
         }
      }
   }
   TEST_METHOD (Division) {
      {
         FixedPoint a{ 1 };
         FixedPoint b{ 1 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 1);
      }
      {
         FixedPoint a{ 1.5 };
         FixedPoint b{ 1.5 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 1);
      }
      {
         FixedPoint a{ 1.5 };
         FixedPoint b{ 0.5 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 3);
      }
      {
         FixedPoint a{ 1.5 };
         FixedPoint b{ -0.5 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == -3);
      }
      {
         FixedPoint a{ 3 };
         FixedPoint b{ 2 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 1.5);
      }
      {
         FixedPoint a{ 1 };
         FixedPoint b{ 2 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 0.5);
      }
      {
         FixedPoint a{ -3458.6413 };
         FixedPoint b{ 35342.666 };
         FixedPoint r = a / b;
         Assert::IsTrue (std::abs ((r - (-0.0978602265)).toDouble ()) < 0.0001);
      }
      {
         FixedPoint a{ -58.6413 };
         FixedPoint b{ 353427657.666 };
         FixedPoint r = a / b;
         Assert::IsTrue (std::abs ((r - (-1.6592165e-7)).toDouble ()) < 0.00000000001);
      }
      {
         FixedPoint a{ -1.543421e-6 };
         FixedPoint b{ 2.784275e+6 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == 0);
      }
      {
         FixedPoint a{ 1.543421e+6 };
         FixedPoint b{ 2.784275e-6 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == FixedPoint::m_MAX_VALUE);
      }
      {
         FixedPoint a{ -1.543421e+6 };
         FixedPoint b{ 2.784275e-6 };
         FixedPoint r = a / b;
         Assert::IsTrue (r == FixedPoint::m_MIN_VALUE);
      }
   }
   };
}
