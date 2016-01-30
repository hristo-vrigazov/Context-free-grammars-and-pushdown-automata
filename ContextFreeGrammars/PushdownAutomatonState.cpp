#include "PushdownAutomatonState.h"

int PushdownAutomatonState::n = 1;

PushdownAutomatonState::PushdownAutomatonState(string label) :
	label(label)
{
}

PushdownAutomatonState::~PushdownAutomatonState()
{
}

const string PushdownAutomatonState::getLabel() const
{
	return this->label;
}

const bool PushdownAutomatonState::operator==(const PushdownAutomatonState & other) const
{
	return this->getLabel() == other.getLabel();
}

PushdownAutomatonState PushdownAutomatonState::generate()
{
	return PushdownAutomatonState("AUTO " + std::to_string(PushdownAutomatonState::n++));
}
