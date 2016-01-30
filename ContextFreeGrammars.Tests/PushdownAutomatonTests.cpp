#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/ContextFreeGrammar.h"
#include "../ContextFreeGrammars/PushdownAutomaton.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(PushdownAutomatonTests)
	{
		TEST_METHOD(PushdownAutomatonConstructor)
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->a{B}"
				<< "{G}->b";

			PushdownAutomaton pd(cfg.getRules(), cfg.getInitialVariable());

			Assert::IsTrue(pd.getTransitions().size() == 4);
		}

		TEST_METHOD(AddTransition)
		{
			PushdownAutomaton pd;
			PushdownAutomatonState s("initial");

			pd.addTransition(PushdownAutomatonTransition(
				s,
				Terminal('a'),
				new Variable("VAR"),
				s,
				RightSide()));

			Assert::IsTrue(pd.getTransitions().size() == 1);
		}

		TEST_METHOD(PushdownAutomatonWordBelongsToLanguage)
		{
			ContextFreeGrammar cfg;

			cfg << "I{A}" 
				<<"{A}->a{A}"
				<< "{A}->b";

			PushdownAutomaton pd(cfg.getRules(), cfg.getInitialVariable());

			Assert::IsTrue(pd.wordBelongsToLanguage("aaab"));
			Assert::IsFalse(pd.wordBelongsToLanguage("aaaa"));
		}
	};
}