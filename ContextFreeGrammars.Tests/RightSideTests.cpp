#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/RightSide.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(RightSideTests)
	{
	public:
		TEST_METHOD(AppendTerminalTest)
		{
			RightSide rs;
			Terminal t('F');
			rs.append(t);

			Assert::IsTrue(rs.toString() == "F");
		}

		TEST_METHOD(AppendCharacterTest)
		{
			RightSide rs;
			rs.append('g');

			Assert::IsTrue(rs.toString() == "g");
		}

		TEST_METHOD(RightSideLengthTest)
		{
			RightSide rs;
			rs.append(Terminal('f'));
			rs.append(Variable("Ivan"));

			Assert::IsTrue(rs.length() == 2);
		}
		
		TEST_METHOD(RightSideNumVariablesTest)
		{
			RightSide rs;

			rs.append(Terminal('f'));
			rs.append(Variable("afv"));
			rs.append(Variable("fgfv"));

			Assert::IsTrue(rs.numberOfVariables() == 2);
		}

		TEST_METHOD(RightSideNumTerminalsTest)
		{
			RightSide rs;

			rs.append(Terminal('f'));
			rs.append(Variable("afv"));
			rs.append(Variable("fgfv"));
			rs.append(Terminal('g'));

			Assert::IsTrue(rs.numberOfTerminals() == 2);
		}

		TEST_METHOD(RightSideTwoVariablesChomsky)
		{
			RightSide rs;
			rs.append(Variable("var"));
			rs.append(Variable("bir"));

			Assert::IsTrue(rs.isChomskyRightSide());
		}

		TEST_METHOD(RightSideOneTerminalChomsky)
		{
			RightSide rs;
			rs.append(Terminal('t'));

			Assert::IsTrue(rs.isChomskyRightSide());
		}

		TEST_METHOD(RightSideNotChomsky)
		{
			RightSide rs;
			rs.append(Terminal('t'));
			rs.append(Variable("var"));
			rs.append(Variable("bir"));

			Assert::IsFalse(rs.isChomskyRightSide());
		}

		TEST_METHOD(ConsistsOfVariablesOfAPredefinedTest)
		{
			RightSide rs;

			rs.append(Variable("VAR"));
			rs.append(Variable("BAR"));
			rs.append(Variable("CAR"));

			set<Variable> setOfVariables;
			setOfVariables.insert(Variable("VAR"));
			setOfVariables.insert(Variable("BAR"));

			Assert::IsFalse(rs.consistsOfVariablesOfSet(setOfVariables));

			setOfVariables.insert(Variable("CAR"));

			Assert::IsTrue(rs.consistsOfVariablesOfSet(setOfVariables));
		}

		TEST_METHOD(ContainsVariable)
		{
			RightSide rs;
			Variable v("VAR");

			rs.append("ghb");

			Assert::IsFalse(rs.contains(v));

			rs.append("VAR");

			Assert::IsTrue(rs.contains(v));
		}

		TEST_METHOD(ContainsTerminal)
		{
			RightSide rs;
			Terminal t('t');

			Assert::IsFalse(rs.contains(t));

			rs.append(t);

			Assert::IsTrue(rs.contains(t));
		}

		TEST_METHOD(RightSideWithoutVariable)
		{
			RightSide rs, result;

			rs.append("A");
			rs.append("B");
			rs.append("C");

			rs.withoutVariable(Variable("A"), result);

			Assert::IsTrue(result.numberOfVariables() == 2);
		}

		TEST_METHOD(CompareOperatorOneSideLongerThanOther)
		{
			RightSide rs1, rs2;

			rs1.append(Variable("A"));
			rs2.append(Variable("A"));
			rs2.append(Variable("B"));

			Assert::IsTrue(rs1 < rs2);
		}

		TEST_METHOD(CompareOperatorFirstShouldBeBigger)
		{
			RightSide rs1, rs2;

			rs1.append("A");
			rs2.append("B");

			Assert::IsTrue(rs1 < rs2);
		}

		TEST_METHOD(CompareOperatorTerminalVariable)
		{
			RightSide rs1, rs2;

			rs1.append("VAR");
			rs2.append("VAR");

			rs1.append("BUB");
			rs2.append("BUB");

			rs1.append('f');
			rs2.append("f");

			Assert::IsTrue(rs1 < rs2);
		}

		TEST_METHOD(RightSideParseLabel)
		{
			string labelToParse = "ba{B}";
			RightSide rs = RightSide::parseLabel(labelToParse);

			Assert::IsTrue(rs.toString() == labelToParse);
		}

		TEST_METHOD(RightSideToString)
		{
			RightSide rs;
			rs.append(Terminal('a'));
			rs.append(Variable("a"));

			Assert::IsTrue(rs.toString() == "a{a}");
		}

		TEST_METHOD(EmptyRightSideParsedCorrectly)
		{
			RightSide rs;

			rs.parseLabel("@");

			Assert::IsTrue(rs.length() == 0);
		}

		TEST_METHOD(GetTerminals)
		{
			RightSide rs;

			rs = RightSide::parseLabel("abc{Ivan}d");

			set<char> expected;
			expected.insert('a');
			expected.insert('b');
			expected.insert('c');
			expected.insert('d');

			set<char> actual;
			rs.getTerminals(actual);

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(getVariables)
		{
			RightSide rs = RightSide::parseLabel("{ABC}{BG}{JU}");

			set<string> variableNames;
			rs.getVariables(variableNames);

			set<string> expected;
			expected.insert("ABC");
			expected.insert("BG");
			expected.insert("JU");
		}

		TEST_METHOD(getVariablesReturnsVariables)
		{
			RightSide rs = RightSide::parseLabel("{A}{B}{C}");

			set<Variable> vars;

			rs.getVariables(vars);

			Assert::IsTrue(vars.size() == 3);
		}

		TEST_METHOD(IsUnit)
		{
			RightSide rs = RightSide::parseLabel("{A}");

			Assert::IsTrue(rs.isUnit());
		}

		TEST_METHOD(ShouldNotBeUnit)
		{
			RightSide rs;
			rs.append('a');
			Assert::IsFalse(rs.isUnit());
		}

		TEST_METHOD(Equals)
		{
			RightSide rs = RightSide::parseLabel("{VAR}");

			Assert::IsTrue(rs.equals(Variable("VAR")));
		}

		TEST_METHOD(GetTerminalsOtherWay)
		{
			RightSide rs = RightSide::parseLabel("z{B}ag");

			set<Terminal> expected;
			expected.insert('z');
			expected.insert('a');
			expected.insert('g');

			set<Terminal> actual = rs.getTerminals();

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(RightSideShouldStartWith)
		{
			RightSide rs = RightSide::parseLabel("{VAR}abc");
			IRightHandSideElement * var = new Variable("VAR");

			Assert::IsTrue(rs.startsWith(var));
		}

		TEST_METHOD(RightSideShouldNotStartWith)
		{
			RightSide rs = RightSide::parseLabel("a{VAR}abc");
			IRightHandSideElement * var = new Terminal('a');

			Assert::IsTrue(rs.startsWith(var));
		}

		TEST_METHOD(RightSideStartsWithEmptyWord)
		{
			RightSide rs = RightSide::parseLabel("a{VAR}abc");
			IRightHandSideElement * var = new Terminal('@');

			Assert::IsTrue(rs.startsWith(var));
		}

		TEST_METHOD(ReplaceBeginning)
		{
			RightSide rs = RightSide::parseLabel("{A}bc");

			Assert::IsTrue(rs.replaceBeginning(RightSide()) == RightSide::parseLabel("bc"));
		}

		TEST_METHOD(RightSideGetVariables)
		{
			RightSide rs = RightSide::parseLabel("{B}{Csds}sdsd{A}");

			set<Variable> expected;
			expected.insert(Variable("B"));
			expected.insert(Variable("Csds"));
			expected.insert(Variable("A"));

			auto actual = rs.getVariables();

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(replaceTerminalWithVariable)
		{
			RightSide rs = RightSide::parseLabel("aba");
			rs.replaceTerminalWithVariable(Terminal('a'), Variable("J"));

			Assert::IsTrue(rs == RightSide::parseLabel("{J}b{J}"));
		}

		TEST_METHOD(extractLastTwoVariables)
		{
			RightSide rs = RightSide::parseLabel("{A}{C}{D}{F}");

			Assert::IsTrue(RightSide::parseLabel("{D}{F}") == rs.extractLastTwoVariables());
		}

		TEST_METHOD(replaceLastTwoVariablesWith)
		{
			RightSide rs = RightSide::parseLabel("{A}{B}{C}");

			Variable v("VAR");

			Assert::IsTrue(rs.replaceLastTwoVariablesWith(v) == RightSide::parseLabel("{A}{VAR}"));
		}
	};
}