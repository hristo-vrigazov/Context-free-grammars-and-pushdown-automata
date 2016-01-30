#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(UtilityTests)
	{
	public:
		TEST_METHOD(ExtractTerminalsFromRules)
		{
			set<Rule> rules;

			rules.insert(Rule::parseLabel("{A}->{C}ad"));
			rules.insert(Rule::parseLabel("{A}->c"));
			rules.insert(Rule::parseLabel("{A}->@"));

			set<Terminal> expected;
			expected.insert('a');
			expected.insert('c');
			expected.insert('d');

			set<Terminal> actual = Utility::getTerminals(rules);

			Assert::IsTrue(expected == actual);
		}
	};
}