#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/Rule.h"
#include "../ContextFreeGrammars/Terminal.h"
#include "../ContextFreeGrammars/Variable.h"
#include "../ContextFreeGrammars/RightSide.h"
#include "../ContextFreeGrammars/Terminal.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(RuleTests)
	{
	public:
		TEST_METHOD(RuleGetLeftSide)
		{
			Variable v("a");
			Rule r(v);

			Assert::IsTrue(r.getLeftSide() == Variable("a"));
		}

		TEST_METHOD(RuleDefaultConstructorInitializesEmptyRightSide) 
		{
			Variable v("a");
			Rule r(v);

			Assert::IsTrue(r.getRightSide().empty());
		}

		TEST_METHOD(RuleComparisonOperator)
		{
			Rule r("A"), w("Z");

			Assert::IsTrue(r < w);
			Assert::IsTrue(w > r);
			Assert::IsTrue(r == Rule("A"));
		}

		TEST_METHOD(RuleConstructorWithHeterogeneousRightSide)
		{
			RightSide rightSide;

			rightSide.append('a');
			rightSide.append('b');
			rightSide.append("A");

			Rule r("VAR", rightSide);

			Assert::IsTrue(r.getRightSide().toString() == "ab{A}");
		}

		TEST_METHOD(RuleWithEpsilonInRightSideDoesNotShowUp)
		{
			RightSide rightSide;

			rightSide.append('@');
			rightSide.append('@');
			rightSide.append("A");

			Rule r("VAR", rightSide);

			Assert::IsTrue(r.getRightSide().toString() == "{A}");
		}

		TEST_METHOD(RuleShouldBeChomsky)
		{
			RightSide rs;

			rs.append(Terminal('f'));

			Assert::IsTrue(rs.isChomskyRightSide());
		}

		TEST_METHOD(RuleParsesStringCorrectly)
		{
			Rule r = Rule::parseLabel("{VAR}->ab{B}c");

			Assert::IsTrue(r.getLeftSide() == Variable("VAR"));
			Assert::IsTrue(r.getRightSide().numberOfVariables() == 1);
			Assert::IsTrue(r.getRightSide().numberOfTerminals() == 3);
		}

		TEST_METHOD(ReplaceRightSide)
		{
			Rule r = Rule::parseLabel("{A}->ab{C}asd{D}");
			r.replaceRightSide(Variable("G"));

			Assert::IsTrue(r.getRightSide().isUnit());
			Assert::IsTrue(r.getRightSide().contains(Variable("G")));
		}

		TEST_METHOD(ShouldBeUnit)
		{
			Rule r = Rule::parseLabel("{A}->{B}");

			Assert::IsTrue(r.isUnit());
		}

		TEST_METHOD(ShouldNotBeUnit)
		{
			Rule r = Rule::parseLabel("{A}->{BUshdsud}sodsdk{a}");

			Assert::IsFalse(r.isUnit());
		}

		TEST_METHOD(ShouldNotBeUnitTerminal)
		{
			Rule r = Rule::parseLabel("{D}->a");

			Assert::IsFalse(r.isUnit());
		}

		TEST_METHOD(ShouldBeUseless)
		{
			Rule r = Rule::parseLabel("{A}->{A}");
			Assert::IsTrue(r.isUseless());
		}

		TEST_METHOD(ShouldNotBeUseless)
		{
			Rule r = Rule::parseLabel("{A}->{B}");
			Assert::IsFalse(r.isUseless());
		}

		TEST_METHOD(RuleGetTerminals)
		{
			Rule r = Rule::parseLabel("{A}->av{KLadsd}b");

			set<Terminal> expected;

			expected.insert('a');
			expected.insert('v');
			expected.insert('b');

			set<Terminal> actual = r.getTerminals();

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(RuleGetVariables)
		{
			Rule r = Rule::parseLabel("{A}->asjbd{VAR}sds{sds}");

			set<Variable> expected;
			expected.insert(Variable("A"));
			expected.insert(Variable("VAR"));
			expected.insert(Variable("sds"));

			set<Variable> actual = r.getVariables();

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(replaceTerminalWithVariableInRightSide)
		{
			Rule r = Rule::parseLabel("{A}->aba");
			r.replaceTerminalWithVariableInRightSide(Terminal('a'), Variable("J"));

			Assert::IsTrue(r == Rule::parseLabel("{A}->{J}b{J}"));
		}

		TEST_METHOD(extractLastTwoVariables)
		{
			Rule r = Rule::parseLabel("{A}->{A}{B}{C}{D}");
			RightSide rs = r.extractLastTwoVariables();

			Assert::IsTrue(rs == RightSide::parseLabel("{C}{D}"));
		}

		TEST_METHOD(replaceLastTwoVariablesWith)
		{
			Rule r = Rule::parseLabel("{A}->{B}{C}{D}");

			Variable v("AUTO");

			RightSide newRightSide = r.replaceLastTwoVariablesWith(v);

			Rule actual(v, newRightSide);
			Rule expected(v, RightSide::parseLabel("{B}{AUTO}"));

			Assert::IsTrue(actual == expected);
		}
	};
}