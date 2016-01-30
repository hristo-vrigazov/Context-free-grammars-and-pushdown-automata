#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/PushdownAutomatonConfiguration.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(PushdownAutomatonConfigurationTests)
	{
		TEST_METHOD(PushdownAutomatonConfigurationConstructor)
		{		
			PushdownAutomatonConfiguration pag(
				PushdownAutomatonState("s"),
				"abc",
				RightSide());

			Assert::IsTrue(pag.getState() == PushdownAutomatonState("s"));
			Assert::IsTrue(pag.getCurrentWord() == "abc");
			Assert::IsTrue(pag.getTopOfStack() == RightSide());
		}

		TEST_METHOD(PushdownAutomatonConfigurationIsAccepting)
		{
			PushdownAutomatonConfiguration pag1(
				PushdownAutomatonState("s"),
				"abc",
				RightSide());

			Assert::IsFalse(pag1.isAccepting());

			PushdownAutomatonConfiguration pag2(
				PushdownAutomatonState("s"),
				"",
				RightSide());

			Assert::IsTrue(pag2.isAccepting());
		}

		TEST_METHOD(PushdownAutomatonConfigurationGetAdjacentConfigurations)
		{
			PushdownAutomatonState initial("s");

			PushdownAutomatonConfiguration pac(
				initial,
				"abc",
				RightSide::parseLabel("{A}"));

			vector<PushdownAutomatonTransition> transitions;

			transitions.push_back(PushdownAutomatonTransition(
				PushdownAutomatonState("s"),
				Terminal('a'),
				new Terminal('@'),
				PushdownAutomatonState("q"),
				RightSide::parseLabel("{A}{B}{C}")));

			vector<PushdownAutomatonConfiguration> expected;
			expected.push_back(PushdownAutomatonConfiguration(
				PushdownAutomatonState("q"),
				"bc",
				RightSide::parseLabel("{A}{B}{C}")));

			auto actual = pac.getAdjacentConfigurations(transitions);

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(PushdownAutomatonConfigurationNextConfiguration)
		{
			PushdownAutomatonConfiguration pag1(
				PushdownAutomatonState("s"),
				"abc",
				RightSide());

			PushdownAutomatonTransition pat(
				PushdownAutomatonState("s"),
				Terminal('a'),
				new Terminal('@'),
				PushdownAutomatonState("q"),
				RightSide::parseLabel("{A}{B}{C}"));

			PushdownAutomatonConfiguration expected(
				PushdownAutomatonState("q"),
				"bc",
				RightSide::parseLabel("{A}{B}{C}"));

			auto actual = pag1.goToNextConfiguration(pat);

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(ConfigurationEqualityTest)
		{
			PushdownAutomatonConfiguration pag1(
				PushdownAutomatonState("s"),
				"abc",
				RightSide());

			PushdownAutomatonConfiguration pag2(
				PushdownAutomatonState("s"),
				"ac",
				RightSide());

			Assert::IsTrue(pag1 == pag1);
			Assert::IsFalse(pag1 == pag2);
		}
	};
}