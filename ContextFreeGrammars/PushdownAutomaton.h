#pragma once
#include "Rule.h"
#include "PushdownAutomatonTransition.h"
#include "PushdownAutomatonConfiguration.h"
#include "PushdownAutomatonState.h"
#include "Utility.h"
#include <set>
#include <vector>
#include <queue>

using namespace std;

class PushdownAutomaton
{
public:
	PushdownAutomaton(const set<Rule> & rules, const Variable & initialVar);
	PushdownAutomaton();
	~PushdownAutomaton();

	void addTransition(const PushdownAutomatonTransition & newTransition);

	const vector<PushdownAutomatonTransition> getTransitions() const;
	const bool wordBelongsToLanguage(string word) const;
private:
	vector<PushdownAutomatonTransition> transitions;
	PushdownAutomatonState initialState;
	Variable initialVariableOfGrammar;
};

