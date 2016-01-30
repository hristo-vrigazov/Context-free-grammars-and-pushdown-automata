#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/IRightHandSideElement.h"
#include "../ContextFreeGrammars/Terminal.h"
#include "../ContextFreeGrammars/Variable.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(IRightSideElementsTests)
	{
	public:
		TEST_METHOD(AreEqualTest)
		{
			IRightHandSideElement * r1 = new Terminal('a');
			IRightHandSideElement * r2 = new Variable("a");

			Assert::IsFalse(areEqual(r1, r2));

			IRightHandSideElement * r3 = new Terminal('a');

			Assert::IsTrue(areEqual(r1, r3));
		}
	};
}