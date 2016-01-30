#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/WordBelongsToLanguageMethodSelector.h"
#include "../ContextFreeGrammars/ContextFreeGrammar.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(WordBelongsToLanguageMethodSelectorTest)
	{
	public:
		TEST_METHOD(UsingCYK)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}";
			cfg << "{S}->{A}{B}"
				<< "{A}->a"
				<< "{B}->b";

			// this grammar generates words that contain only 'a'
			WordBelongsToLanguageMethodSelector checker(cfg, "ab");

			Assert::IsTrue(checker.usingCYK());
			
			WordBelongsToLanguageMethodSelector checker2(cfg, "aa");

			Assert::IsFalse(checker2.usingCYK());
		}

		TEST_METHOD(UsingPushdownAutomaton)
		{
			ContextFreeGrammar cfg;

			RightSide rs;

			rs.append(Terminal('a'));
			rs.append(Variable("VAR"));
			cfg.setInitialVariable(Variable("VAR"));
			// add the rule {VAR} -> a{VAR}
			cfg.addRule(Rule(Variable("VAR"), rs));

			// add the rule {VAR} -> @
			cfg.addRule(Rule(Variable("VAR")));

			// this grammar generates words that contain only 'a'
			WordBelongsToLanguageMethodSelector checker_1(cfg, "aa");

			Assert::IsTrue(checker_1.usingPushdownAutomaton());

			WordBelongsToLanguageMethodSelector checker_2(cfg, "ab");

			Assert::IsFalse(checker_2.usingPushdownAutomaton());
		}

		TEST_METHOD(Palindrome)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}"
				<< "{S}->a{S}a"
				<< "{S}->b{S}b"
				<< "{S}->@";

			Assert::IsTrue(cfg.wordBelongsToLanguage("abaaba").usingPushdownAutomaton());
			Assert::IsFalse(cfg.wordBelongsToLanguage("ababba").usingPushdownAutomaton());
		}
	};
}