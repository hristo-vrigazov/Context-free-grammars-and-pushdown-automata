#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/Variable.h"
#include "../ContextFreeGrammars/RightSide.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(VariableTests)
	{
	public:
		TEST_METHOD(VariableGetLabel)
		{
			Variable v("g");

			Assert::IsTrue(v.getLabel() == "g");
		}

		TEST_METHOD(VariableToString)
		{
			Variable v("dfdf");

			Assert::IsTrue(v.toString() == "dfdf");
		}

		TEST_METHOD(SimpleParseLabelTest)
		{
			string raw = "{someLabel}";

			Assert::IsTrue(Variable::parseLabel(raw) == "someLabel");
		}

		TEST_METHOD(ParseLabelWithOneEscapedCharacter)
		{
			string raw = "{Some\\@string}";

			Assert::IsTrue(Variable::parseLabel(raw) == "Some@string");
		}

		TEST_METHOD(ParseLabelWithSlashButNotEscaped)
		{
			string raw = "{Some\\string}";

			Assert::IsTrue(Variable::parseLabel(raw) == "Some\\string");
		}

		TEST_METHOD(ParseLabelWithComplexStructure)
		{
			string raw = "{Something\\@\\\\ivan\\s}";

			Assert::IsTrue(Variable::parseLabel(raw) == "Something@\\ivan\\s");
		}

		TEST_METHOD(UnEqualityTests)
		{
			Variable v("VAR"), w("fVAR");

			Assert::IsTrue(v != w);
		}

		TEST_METHOD(isContainedInRightSideTest)
		{
			Variable v("GTH");

			RightSide rs;
			rs.append("Gosho");
			rs.append("something");

			Assert::IsFalse(v.isContainedIn(rs));

			rs.append("GTH");

			Assert::IsTrue(v.isContainedIn(rs));
		}
	};
}