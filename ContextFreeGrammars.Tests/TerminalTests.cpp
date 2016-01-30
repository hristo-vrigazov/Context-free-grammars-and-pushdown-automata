#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/Terminal.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(TerminalTestClass)
	{
	public:
		TEST_METHOD(TerminalGetLabel)
		{
			Terminal t('g');

			Assert::IsTrue(t.getLabel() == 'g');
		}

		TEST_METHOD(CreatesBlankWordIfNotSpecified)
		{
			Terminal t;

			Assert::IsTrue(t.isBlankWord());
		}

		TEST_METHOD(ReturnsBlankWordIfTerminal)
		{
			Terminal t('@');

			Assert::IsTrue(t.toString() == "");
		}

		TEST_METHOD(ToStringEmptyWord)
		{
			Terminal t;
			Assert::IsTrue(t.isBlankWord());
		}

		TEST_METHOD(ToStringNormalCharacter)
		{
			Terminal t('g');
			Assert::IsTrue(t.toString() == "g");
		}

		TEST_METHOD(TerminalSpecialCharacter)
		{
			Terminal t("\\{");

			Assert::IsTrue(t.toString() == "{");
		}

		TEST_METHOD(GenerateWordsOfFixedLength)
		{
			set<Terminal> s;
			s.insert(Terminal('a'));
			s.insert(Terminal('b'));

			vector<string> result;

			Terminal::generateWords(3, s, result);

			Assert::IsTrue(result.size() == 8);
		}

		TEST_METHOD(GenerateWordsInRange)
		{
			set<Terminal> s;
			s.insert(Terminal('a'));
			s.insert(Terminal('b'));

			vector<string> result;

			// 1 + 2 + 4 + 8 = 15
			Terminal::generateWords(0, 4, s, result);

			Assert::IsTrue(result.size() == 15);
		}

		TEST_METHOD(AddLetters)
		{
			vector<string> res;
			set<char> s;
			s.insert('a');
			s.insert('b');

			Terminal::addLetters("", 3, s, res);

			Assert::IsTrue(res.size() == 8);
		}
	};
}