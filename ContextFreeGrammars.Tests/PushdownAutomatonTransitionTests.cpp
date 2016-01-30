#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ContextFreeGrammars/IRightHandSideElement.h"
#include "../ContextFreeGrammars/Terminal.h"
#include "../ContextFreeGrammars/Variable.h"
#include "../ContextFreeGrammars/RightSide.h"
#include "../ContextFreeGrammars/PushdownAutomatonTransition.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ContextFreeGrammarsTests
{
	TEST_CLASS(PushdownAutomatonTransitionTests)
	{
		TEST_METHOD(PushdownAutomatonTransitionConstructor)
		{
			IRightHandSideElement* el = new Variable("VAR");

			PushdownAutomatonTransition pat(
				PushdownAutomatonState("s"),
				Terminal('a'),
				el,
				PushdownAutomatonState("s"), 
				RightSide());
		}
	};
}