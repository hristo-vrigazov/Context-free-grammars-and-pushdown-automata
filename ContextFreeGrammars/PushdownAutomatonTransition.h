#pragma once
#include "PushdownAutomatonState.h"
#include "RightSide.h"
#include "Terminal.h"

class PushdownAutomatonTransition
{
public:
	PushdownAutomatonTransition(
		PushdownAutomatonState currentState,
		Terminal characterInWord,
		IRightHandSideElement* topOfStack,
		PushdownAutomatonState nextState,
		RightSide newTopOfStack);
	~PushdownAutomatonTransition();

	const PushdownAutomatonState getCurrentState() const;
	const Terminal getCharacterInWord() const;
	const IRightHandSideElement* getTopOfStack() const;
	const PushdownAutomatonState getNextState() const;
	const RightSide getNextTopOfStack() const;

private:
	PushdownAutomatonState currentState;
	Terminal characterInWord;
	IRightHandSideElement* topOfStack;
	PushdownAutomatonState nextState;
	RightSide newTopOfStack;
};

