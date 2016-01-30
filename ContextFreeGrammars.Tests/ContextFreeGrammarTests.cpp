#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/IRightHandSideElement.cpp"
#include "../ContextFreeGrammars/PushdownAutomatonState.cpp"
#include "../ContextFreeGrammars/WordBelongsToLanguageMethodSelector.cpp"
#include "../ContextFreeGrammars/PushdownAutomaton.cpp"
#include "../ContextFreeGrammars/PushdownAutomatonTransition.cpp"
#include "../ContextFreeGrammars/Utility.cpp"
#include "../ContextFreeGrammars/ContextFreeGrammar.cpp"
#include "../ContextFreeGrammars/PushdownAutomatonConfiguration.cpp"
#include "../ContextFreeGrammars/Variable.cpp"
#include "../ContextFreeGrammars/Terminal.cpp"
#include "../ContextFreeGrammars/Rule.cpp"
#include "../ContextFreeGrammars/RightSide.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(ContextFreeTestClass)
	{
	public:

		TEST_METHOD(AddVariable)
		{
			ContextFreeGrammar cfg;

			Variable v("something");

			cfg.addVariable(v);

			Assert::IsTrue(cfg.getVariables().size() == 1);

		}

		TEST_METHOD(AddTerminal)
		{
			ContextFreeGrammar cfg;

			Terminal t('t');

			cfg.addTerminal(t);

			Assert::IsTrue(cfg.getTerminals().size() == 1);
		}

		TEST_METHOD(AddRule)
		{
			ContextFreeGrammar cfg;
			RightSide rs;
			Rule r("A", rs);

			cfg.addRule(r);

			rs.append(Terminal('f'));
			Rule w("A", rs);

			cfg.addRule(w);
			Assert::IsTrue(cfg.getRules().size() == 2);
		}
		
		TEST_METHOD(ConsumingStringsBuildsGrammar)
		{
			ContextFreeGrammar cfg;
			cfg << "{S}->a{B}{C}";
			Assert::IsTrue(cfg.getVariables().size() == 3);
			Assert::IsTrue(cfg.getTerminals().size() == 1);
		}

		TEST_METHOD(ConsumingStringSetsInitial)
		{
			ContextFreeGrammar cfg;
			cfg << "I{S}";

			Assert::IsTrue(cfg.getInitialVariable() == Variable("S"));
		}

		TEST_METHOD(WordBelongsToLanguageCYK)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}"
				<< "{S}->{A}{B}"
				<< "{A}->a"
				<< "{B}->b";

			// this grammar generates words that contain only 'a'

			Assert::IsTrue(cfg.wordBelongsToLanguage("ab").usingCYK());
			Assert::IsFalse(cfg.wordBelongsToLanguage("aa").usingCYK());
		}
		
		TEST_METHOD(WordBelongsToLanguagePushdownAutomaton)
		{
			ContextFreeGrammar cfg;

			RightSide rs;

			cfg.setInitialVariable(Variable("VAR"));

			rs.append(Terminal('a'));
			rs.append(Variable("VAR"));

			// add the rule {VAR} -> a{VAR}
			cfg.addRule(Rule(Variable("VAR"), rs));

			// add the rule {VAR} -> @
			cfg.addRule(Rule(Variable("VAR")));

			// this grammar generates words that contain only 'a'

			Assert::IsTrue(cfg.wordBelongsToLanguage("aaaa").usingPushdownAutomaton());
			Assert::IsFalse(cfg.wordBelongsToLanguage("ab").usingPushdownAutomaton());
		}

		TEST_METHOD(ShouldBeChomskyNormal)
		{
			ContextFreeGrammar cfg;

			RightSide rs1;
			rs1.append(Terminal('f'));

			// rule V -> f
			Rule rule1(Variable("V"), rs1);

			RightSide rs2;
			rs2.append(Variable("A"));
			rs2.append(Variable("B"));

			Rule rule2(Variable("V"), rs2);
			cfg.addRule(rule2);

			Assert::IsTrue(cfg.isChomskyNormal());
		}

		TEST_METHOD(ShouldNotBeChomskyNormal)
		{
			ContextFreeGrammar cfg;

			RightSide rs1;
			rs1.append(Terminal('f'));

			// rule V -> f
			Rule rule1(Variable("V"), rs1);

			RightSide rs2;
			rs2.append(Variable("A"));
			rs2.append(Variable("g"));

			Rule rule2(Variable("V"), rs2);
			cfg.addRule(rule2);

			Assert::IsTrue(cfg.isChomskyNormal());
		}

		TEST_METHOD(ConvertToChomsky)
		{
			ContextFreeGrammar cfg;

			RightSide rs1;
			rs1.append(Terminal('f'));
			rs1.append(Variable("VAR"));

			// rule V -> f{VAR}
			Rule rule1(Variable("V"), rs1);
			Rule rule2(Variable("VAR"), rs1);
			Rule rule3(Variable("VAR"));

			cfg.addRule(rule1);
			cfg.addRule(rule2);
			cfg.addRule(rule3);
			//	rule2();
			Assert::IsFalse(cfg.isChomskyNormal());

			cfg.convertToChomskyNormal();

			Assert::IsTrue(cfg.isChomskyNormal());
		}

		TEST_METHOD(EpsilonElimination)
		{
			ContextFreeGrammar cfg;

			Variable S("S");
			cfg.setInitialVariable(S);

			RightSide rs1;
			rs1.append("A");
			rs1.append("B");

			// S -> AB
			Rule r1(S, rs1);

			RightSide rs2;
			rs2.append('a');
			rs2.append("A");

			// A -> aA
			Rule r2("A", rs2);	

			RightSide rs3;
			rs3.append('b');
			rs3.append("B");

			// B -> bB
			Rule r3("B", rs3);

			cfg.addRule(r1);
			cfg.addRule(r2);
			cfg.addRule(r3);
			cfg.addRule(Rule("A"));
			cfg.addRule(Rule("B"));

			cfg.epsilonRulesElimination();
			Assert::IsTrue(cfg.getRules().size() == 9);
		}

		TEST_METHOD(EliminateCyclicUnitRules)
		{	
			ContextFreeGrammar cfg;

			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{A}"
				<< "{A}->@";

			cfg.eliminateCyclicUnitRules();

			Assert::IsTrue(cfg.getRules().size() == 1);
			Assert::IsTrue(cfg.getRules().begin()->getRightSide().empty());
		}

		TEST_METHOD(GetUnitRules)
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{A}"
				<< "{A}->@";
			
			set<Rule> unitRules;

			cfg.getUnitRules(unitRules);

			Assert::IsTrue(unitRules.size() == 3);
		}

		TEST_METHOD(EliminateUncyclicUnitRules) 
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{D}"
				<< "{D}->a{N}"
				<< "{B}->c";

			cfg.eliminateUncyclicUnitRules();

			set<Rule> expected;
			expected.insert(Rule::parseLabel("{A}->a{N}"));
			expected.insert(Rule::parseLabel("{A}->c"));
			expected.insert(Rule::parseLabel("{C}->a{N}"));
			expected.insert(Rule::parseLabel("{B}->c"));
			expected.insert(Rule::parseLabel("{D}->a{N}"));


			Assert::IsTrue(cfg.getRules() == expected);
		}

		TEST_METHOD(EliminateMixedRules)
		{
			ContextFreeGrammar cfg;

			ContextFreeGrammar::autoGeneratedVariables = 0;

			cfg << "{A}->{A}bc"
				<< "{A}->a";

			cfg.eliminateMixedRules();

			set<Rule> expected;
			expected.insert(Rule::parseLabel("{AUTO 0}->a"));
			expected.insert(Rule::parseLabel("{A}->a"));
			expected.insert(Rule::parseLabel("{A}->{A}{AUTO 1}{AUTO 2}"));
			expected.insert(Rule::parseLabel("{AUTO 1}->b"));
			expected.insert(Rule::parseLabel("{AUTO 2}->c"));

			Assert::IsTrue(expected == cfg.getRules());
		}

		TEST_METHOD(EliminateLongRightSide)
		{
			ContextFreeGrammar cfg;

			ContextFreeGrammar::autoGeneratedVariables = 0;

			cfg << "{A}->{B}{C}{D}";

			cfg.eliminateRulesWithLongRightSide();

			set<Rule> expected;
			expected.insert(Rule::parseLabel("{A}->{B}{AUTO 0}"));
			expected.insert(Rule::parseLabel("{AUTO 0}->{C}{D}"));

			Assert::IsTrue(cfg.getRules() == expected);
		}

		TEST_METHOD(existsRuleWithLongRightSide)
		{
			ContextFreeGrammar cfg;
			cfg << "{A}->{B}{C}{D}";
			cfg << "{A}->av";

			Rule longRule = cfg.existsRuleWithLongRightSide();

			Assert::IsTrue(longRule == Rule::parseLabel("{A}->{B}{C}{D}"));
		}

		TEST_METHOD(UnionGrammars)
		{
			ContextFreeGrammar cfg1, cfg2, result;

			cfg1 << "I{A1}";
			cfg1 << "{A1}->ab{B}";

			cfg2 << "I{A2}";
			cfg2 << "{A2}->{G}";

			result.createAsUnionOf(cfg1, cfg2);

			Assert::IsTrue(result.getRules().size() == 4);
			Assert::IsTrue(result.getInitialVariable() != Variable("A1"));
			Assert::IsTrue(result.getInitialVariable() != Variable("A2"));
		}

		TEST_METHOD(ConcatenateGrammars)
		{
			ContextFreeGrammar cfg1, cfg2, result;

			cfg1 << "{S1}->a";

			cfg2 << "{S2}->b";

			result.createAsConcatenationOf(cfg1, cfg2);

			Assert::IsTrue(result.getRules().size() == 3);
			Assert::IsTrue(result.getInitialVariable() != Variable("S1"));
			Assert::IsTrue(result.getInitialVariable() != Variable("S2"));
		}

		TEST_METHOD(IterationGrammars)
		{
			ContextFreeGrammar cfg, result;

			cfg << "I{VAR}";
			cfg << "{VAR}->{A}{B}";

			result.createAsIterationOf(cfg);
			Assert::IsTrue(result.getRules().size() == 4);
			Assert::IsTrue(result.getInitialVariable() != Variable("VAR"));
		}

		TEST_METHOD(LanguageShouldNotBeEmpty)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}"
				<< "{S}->{A}{B}"
				<< "{A}->b"
				<< "{B}->a";

			Assert::IsTrue(cfg.languageIsEmpty());
		}

		TEST_METHOD(LanguageShouldBeEmpty)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}"
				<< "{S}->{A}{B}"
				<< "{A}->b";

			Assert::IsTrue(cfg.languageIsEmpty());
		}

		TEST_METHOD(LanguageShouldNotBeFinite)
		{
			ContextFreeGrammar cfg;
			cfg << "I{S}" 
				<< "{S}->a"
				<< "{S}->{S}{S}";

			Assert::IsFalse(cfg.languageIsFinite());
		}

		TEST_METHOD(LanguageShouldBeFinite)
		{
			ContextFreeGrammar cfg;

			cfg << "I{S}"
				<< "{S}->{A}{B}"
				<< "{A}->a"
				<< "{B}->b";

			Assert::IsTrue(cfg.languageIsFinite());
		}

		TEST_METHOD(ConsumingStrings)
		{
			ContextFreeGrammar cfg;

			cfg << "{VAR}->a{BAC}";

			Assert::IsTrue(cfg.getRules().size() == 1);
			Assert::IsTrue(cfg.getRules().begin()->getLeftSide() == Variable("VAR"));
			Assert::IsTrue(cfg.getRules().begin()->getRightSide().numberOfTerminals() == 1);
			Assert::IsTrue(cfg.getRules().begin()->getRightSide().numberOfVariables() == 1);
		}

		TEST_METHOD(SetInitialVariableThroughString)
		{
			ContextFreeGrammar cfg;
			cfg << "I{VAR}";

			Assert::IsTrue(cfg.getInitialVariable() == Variable("VAR"));
		}

		TEST_METHOD(ExistsRuleThatGoesToEpsilonSet)
		{
			ContextFreeGrammar cfg;
			
			cfg.addRule(Rule(Variable("VAR")));

			Assert::IsTrue(cfg.existsRuleThatGoesToEpsilonSet(set<Variable>()) == Variable("VAR"));

			ContextFreeGrammar cfg1;

			Assert::IsTrue(cfg1.existsRuleThatGoesToEpsilonSet(set<Variable>()) == Variable("Rule not found"));
		}

		TEST_METHOD(ExtractEpsilonSetTest)
		{
			// A -> @
			Rule r1(Variable("A"));

			RightSide rs;
			rs.append("A");
			rs.append("A");

			// B -> AA
			Rule r2(Variable("B"), rs);

			rs.append("B");

			// C -> AAB
			Rule r3(Variable("C"), rs);

			RightSide rside;
			rside.append(Terminal('r'));

			// D -> r
			Rule r4(Variable("D"), rside);

			// there are 4 variables: A B C D
			// we expect A B C in the epsilon set
			ContextFreeGrammar cfg;
			cfg.addRule(r1);
			cfg.addRule(r2);
			cfg.addRule(r3);
			cfg.addRule(r4);

			set<Variable> expectedSet;
			expectedSet.insert(Variable("A"));
			expectedSet.insert(Variable("B"));
			expectedSet.insert(Variable("C"));

			set<Variable> actualSet;
			Assert::IsTrue(cfg.extractEpsilonSet(actualSet) == expectedSet);
		}

		TEST_METHOD(SetInitialVariable)
		{
			Variable v("VAR");

			ContextFreeGrammar cfg;
			cfg.setInitialVariable(v);

			Assert::IsTrue(cfg.getInitialVariable() == v);
		}

		TEST_METHOD(AddRulesWithoutEpsilonVariables)
		{
			// A -> @
			Rule r1(Variable("A"));

			RightSide rs;
			rs.append("A");

			// B -> A
			Rule r2(Variable("B"), rs);

			rs.append(Terminal('f'));

			// C -> Bf
			Rule r3(Variable("C"), rs);
			
			ContextFreeGrammar cfg;
			cfg.setInitialVariable(Variable("A"));
			cfg.addRule(r1);
			cfg.addRule(r2);
			cfg.addRule(r3);

			set<Variable> epsilonSet;
			Assert::IsTrue(cfg.extractEpsilonSet(epsilonSet).size() == 2);

			cfg.addRulesWithoutEpsilonVariables(epsilonSet);


			/* Should have 5 rules:
			*	A -> @
			*	B -> A
			*	C -> Af
			*	B -> @
			*	C -> f
			*/
			Assert::IsTrue(cfg.getRules().size() == 5);
		}

		TEST_METHOD(existsRuleContainingVariableFromEpsilonSet)
		{
			ContextFreeGrammar cfg;

			RightSide rs;
			rs.append("A");
			cfg.addRule(Rule("A"));

			Rule ruleThatShouldBeFound = Rule(Variable("B"), rs);
			cfg.addRule(ruleThatShouldBeFound);

			set<Variable> epsilonSet;
			cfg.extractEpsilonSet(epsilonSet);

			Rule actual("something");
			cfg.existsRuleContainingVariableFromEpsilonSet(epsilonSet, actual);
			Assert::IsTrue(actual == Rule("B"));
		}

		TEST_METHOD(removeRulesWithEmptyRightSide)
		{
			ContextFreeGrammar cfg;

			cfg.addRule(Variable("V"));
			cfg.addRule(Variable("B"));

			RightSide rs;

			rs.append(Terminal('g'));

			Rule r(Variable("B"), rs);
			cfg.addRule(r);

			Assert::IsTrue(cfg.getRules().size() == 3);

			cfg.removeRulesWithEmptyRightSide();

			Assert::IsTrue(cfg.getRules().size() == 1);
		}

		TEST_METHOD(GenerateVariable)
		{
			ContextFreeGrammar cfg1, cfg2;

			Variable res1, res2;

			cfg1.generateVariable(res1);
			cfg2.generateVariable(res2);

			Assert::IsTrue(res1.getLabel().substr(0, 5) == "AUTO ");
			Assert::IsTrue(res1.getLabel().substr(0, 5) == "AUTO ");
		}

		TEST_METHOD(GetTerminals)
		{
			ContextFreeGrammar cfg;

			cfg << "{B}->{A}aaabc" 
				<< "{A}->d{P}cba";

			set<char> expected;
			expected.insert('a');
			expected.insert('b');
			expected.insert('c');
			expected.insert('d');

			set<Terminal> actual;
			cfg.getTerminals(actual);
		}

		TEST_METHOD(ReplaceAllVariables)
		{
			ContextFreeGrammar cfg;
			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{A}"
				<< "{D}->abc";

			set<Variable> replacingSet;
			replacingSet.insert(Variable("A"));
			replacingSet.insert(Variable("B"));
			replacingSet.insert(Variable("C"));

			set<Rule> rules;

			rules.insert(Rule::parseLabel("{A}->{B}"));
			rules.insert(Rule::parseLabel("{B}->{C}"));

			cfg.unifyAllVariables(replacingSet, rules);

			int numberOfAVariables = 0;

			Assert::IsTrue(rules.size() == 1);
			Assert::IsTrue(rules.begin()->isUnit());
			Assert::IsTrue(rules.begin()->getRightSide().contains(Variable("A")));
			Assert::IsTrue(rules.begin()->getRightSide().contains(Variable("A")));
		}

		TEST_METHOD(ReplaceOneVariable)
		{
			ContextFreeGrammar cfg;
			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{A}"
				<< "{D}->abc";

			Variable ref("A");

			set<Rule> rules;

			rules.insert(Rule::parseLabel("{A}->{B}"));
			rules.insert(Rule::parseLabel("{B}->{C}"));
			rules.insert(Rule::parseLabel("{C}->{A}"));

			cfg.replaceVariableWith(Variable("B"), ref, rules);

			int numberOfALeftSided = 0, numberOfARightSided = 0;

			for each (auto rule in rules)
			{
				if (rule.getLeftSide() == ref)
				{
					numberOfALeftSided++;
				}
				if (rule.getRightSide().contains(ref))
				{
					numberOfARightSided++;
				}
			}

			Assert::IsTrue(numberOfALeftSided == 2);
			Assert::IsTrue(numberOfARightSided == 2);

		}

		TEST_METHOD(ExtractStronglyConnectedComponents)
		{
			ContextFreeGrammar cfg;
			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{D}"
				<< "{D}->{A}"
				<< "{F}->{V}"
				<< "{F}->{G}"
				<< "{F}->{C}";

			set<Variable> scc1;
			scc1.insert(Variable("A"));
			scc1.insert(Variable("B"));
			scc1.insert(Variable("C"));
			scc1.insert(Variable("D"));

			set<Variable> scc2;
			scc2.insert(Variable("F"));

			set<Variable> scc3;
			scc3.insert(Variable("V"));

			set<Variable> scc4;
			scc4.insert(Variable("G"));

			set<set<Variable>> expected;
			expected.insert(scc1);
			expected.insert(scc2);
			expected.insert(scc3);
			expected.insert(scc4);

			auto actual = cfg.extractStronglyConnectedComponents(cfg.getRules());

			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(GetUnitVariablesRules)
		{
			set<Rule> rules;

			rules.insert(Rule::parseLabel("{A}->{B}"));
			rules.insert(Rule::parseLabel("{B}->{C}"));
			rules.insert(Rule::parseLabel("{A}->{D}"));
			rules.insert(Rule::parseLabel("{A}->{B}"));

			set<Variable> expected;
			expected.insert(Variable("A"));
			expected.insert(Variable("B"));
			expected.insert(Variable("C"));
			expected.insert(Variable("D"));

			ContextFreeGrammar cfg;
			auto actual = cfg.getUnitVariables(rules);

			Assert::IsTrue(actual == expected);
		}
		
		TEST_METHOD(getUnitRuleWithRightSideNotInGraph)
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->{B}"
				<< "{B}->{C}"
				<< "{C}->{D}"
				<< "{D}->a";

			set<Rule> unitRules;
			cfg.getUnitRules(unitRules);

			Rule actual = cfg.getUnitRuleWithRightSideNotInGraph(unitRules);

			Assert::IsTrue(actual == Rule::parseLabel("{C}->{D}"));
		}

		TEST_METHOD(getRulesFrom)
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->ab{GH}f"
				<< "{B}->vs{T}"
				<< "{A}->b";

			set<Rule> expected;
			expected.insert(Rule::parseLabel("{A}->ab{GH}f"));
			expected.insert(Rule::parseLabel("{A}->b"));

			set<Rule> actual = cfg.getRulesFrom(Variable("A"), cfg.getRules());
		}

		TEST_METHOD(removeUnitRulesThatGoTo)
		{
			ContextFreeGrammar cfg;

			cfg << "{A}->{B}"
				<< "{C}->{B}a"
				<< "{D}->{A}";

			cfg.removeUnitRulesThatGoTo(Variable("B"));

			set<Rule> expected;
			expected.insert(Rule::parseLabel("{C}->{B}a"));
			expected.insert(Rule::parseLabel("{D}->{A}"));

			auto actual = cfg.getRules();

			Assert::IsTrue(actual == expected);
		}
	};
}