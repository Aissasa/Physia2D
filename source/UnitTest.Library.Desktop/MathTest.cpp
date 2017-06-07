#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace glm;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(MathTest)
	{
	public:

#pragma region Memory leak check
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#pragma endregion

#pragma region Test methods

		/*******************************************************/
		TEST_METHOD(CommonFuncs)
		{
			Assert::AreEqual(1, abs(-1));

			Assert::AreEqual(1, sign(5));
			Assert::AreEqual(-1, sign(-9));
			Assert::AreEqual(0, sign(0));

			Assert::AreEqual(1.0f, ceil(0.6f));
			Assert::AreEqual(1.0f, ceil(0.4f));

			Assert::AreEqual(0.6f, clamp(0.6f, 0.0f, 2.0f));

			Assert::AreEqual(0.0f, floor(0.6f));
			Assert::AreEqual(0.0f, floor(0.4f));

			Assert::AreEqual(0.5f, fract(1.5f));

			Assert::AreEqual(0.3f, max(0.1f, 0.3f));
			Assert::AreEqual(0.1f, min(0.1f, 0.3f));

			Assert::AreEqual(1.0f, round(0.6f));
			Assert::AreEqual(0.0f, round(0.4f));
		}

		/*******************************************************/
		TEST_METHOD(ArithmiticOperationsOnVec2)
		{
			vec2 v1(1, 0);
			vec2 v2(0, 1);

			Assert::IsTrue(v1 + v2 == vec2(1, 1));
			Assert::IsTrue(v1 - v2 == vec2(1, -1));
			Assert::IsTrue(v1 * v2 == vec2(0, 0));
			Assert::IsTrue(v1 / vec2(2, 5) == vec2(0.5f, 0));

			Assert::IsTrue(v1 + 2.0f == vec2(3, 2));
			Assert::IsTrue(v1 - 5.0f == vec2(-4, -5));
			Assert::IsTrue(v1 * 3.0f == vec2(3, 0));
			Assert::IsTrue(v1 / 0.5f == vec2(2, 0));
		}

		/*******************************************************/
		TEST_METHOD(DotProductOnVec2)
		{
			vec2 v1(1, 0);
			vec2 v2(0, 1);

			Assert::IsTrue(dot(v1, v2) == 0);
			Assert::IsTrue(dot(v1, v1) == 1);
			Assert::IsTrue(dot(v1 * 2.0f, v1) == 2);
			Assert::IsTrue(dot(v1, -v1) == -1);
			Assert::IsTrue(dot(v1* 2.0f, -v1) == -2);

			Assert::IsTrue(dot(vec2(5, 9), vec2(2, 1)) == 19);
		}

		/*******************************************************/
		TEST_METHOD(OtherGeometricFuncsOnVec2)
		{
			vec2 v1(1, 0);
			vec2 v2(0, 1);
			vec2 v3(5, 9);

			// cross product not available for vec2

			Assert::IsTrue(length(v3) == sqrt(v3.x * v3.x + v3.y * v3.y));
			Assert::IsTrue(distance(v1, v2) == sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y)));
			Assert::IsTrue(normalize(vec2(v3)) == v3 / length(v3));
			Assert::IsTrue(reflect(vec2(1, -1), v2) == vec2(1, 1));
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState MathTest::sStartMemState;
}