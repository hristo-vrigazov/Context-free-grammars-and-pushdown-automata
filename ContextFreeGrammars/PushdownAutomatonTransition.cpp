#include "PushdownAutomatonTransition.h"



PushdownAutomatonTransition::PushdownAutomatonTransition(
	PushdownAutomatonState currentState,
	Terminal characterInWord,
	IRightHandSideElement* topOfStack,
	PushdownAutomatonState nextState,
	RightSide newTopOfStack) :

	currentState(currentState),
	characterInWord(characterInWord),
	topOfStack(topOfStack),
	nextState(nextState),
	newTopOfStack(newTopOfStack)
{
}


PushdownAutomatonTransition::~PushdownAutomatonTransition()
{
}

const PushdownAutomatonState PushdownAutomatonTransition::getCurrentState() const
{
	return this->currentState;
}

const Terminal PushdownAutomatonTransition::getCharacterInWord() const
{
	return this->characterInWord;
}

const IRightHandSideElement * PushdownAutomatonTransition::getTopOfStack() const
{
	return this->topOfStack;
}

const PushdownAutomatonState PushdownAutomatonTransition::getNextState() const
{
	return this->nextState;
}

const RightSide PushdownAutomatonTransition::getNextTopOfStack() const
{
	return this->newTopOfStack;
}
