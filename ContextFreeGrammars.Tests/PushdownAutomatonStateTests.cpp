#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/PushdownAutomatonState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(PushdownAutomatonStateTests)
	{
		TEST_METHOD(PushdownAutomatonStateConstructorTest)
		{
			PushdownAutomatonState s("initial");

			Assert::IsTrue(s.getLabel() == "initial");
		}

		TEST_METHOD(PushdownAutomatonStateGenerate)
		{
			PushdownAutomatonState generatedState = PushdownAutomatonState::generate();

			Assert::IsTrue(generatedState.getLabel().substr(0, 5) == "AUTO ");
		}
	};
}