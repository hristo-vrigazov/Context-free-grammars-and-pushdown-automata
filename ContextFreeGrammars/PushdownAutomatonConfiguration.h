#pragma once
#include <vector>
#include "PushdownAutomatonTransition.h"
#include "PushdownAutomatonState.h"

using namespace std;

class PushdownAutomatonConfiguration
{
public:
	PushdownAutomatonConfiguration(PushdownAutomatonState pas, string currentWord, RightSide topOfStack);
	~PushdownAutomatonConfiguration();

	const PushdownAutomatonState getState() const;
	const string getCurrentWord() const;
	const RightSide getTopOfStack() const;

	const bool isAccepting() const;
	const vector<PushdownAutomatonConfiguration> getAdjacentConfigurations(const vector<PushdownAutomatonTransition> rules) const;
	
	PushdownAutomatonConfiguration goToNextConfiguration(const PushdownAutomatonTransition rule) const;

	friend bool operator==(const PushdownAutomatonConfiguration & pac1, const PushdownAutomatonConfiguration & pac2);
private:
	PushdownAutomatonState state;
	string currentWord;
	RightSide topOfStack;
};

