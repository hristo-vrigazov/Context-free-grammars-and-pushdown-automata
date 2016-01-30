#include "PushdownAutomaton.h"


PushdownAutomaton::PushdownAutomaton(const set<Rule> & rules, const Variable & pds)
{
	this->initialVariableOfGrammar = pds;
	this->initialState = PushdownAutomatonState::generate();

	set<Terminal> terminals = Utility::getTerminals(rules);

	for each (auto terminal in terminals)
	{
		this->addTransition(PushdownAutomatonTransition(
			this->initialState,
			terminal,
			new Terminal(terminal),
			this->initialState,
			RightSide()));
	}

	for each (auto rule in rules)
	{
		this->addTransition(PushdownAutomatonTransition(
			this->initialState,
			Terminal(),
			new Variable(rule.getLeftSide()),
			this->initialState,
			rule.getRightSide()));
	}
}

PushdownAutomaton::PushdownAutomaton()
{
}


PushdownAutomaton::~PushdownAutomaton()
{
}

void PushdownAutomaton::addTransition(const PushdownAutomatonTransition & newTransition)
{
	this->transitions.push_back(newTransition);
}

const vector<PushdownAutomatonTransition> PushdownAutomaton::getTransitions() const
{
	return this->transitions;
}

const bool PushdownAutomaton::wordBelongsToLanguage(string word) const
{
	RightSide initialStack;

	initialStack.append(this->initialVariableOfGrammar);

	PushdownAutomatonConfiguration initialConfiguration(
		this->initialState,
		word,
		initialStack);


	// perform Breadth-first search
	queue<PushdownAutomatonConfiguration> q;

	q.push(initialConfiguration);

	while (!q.empty())
	{
		PushdownAutomatonConfiguration current = q.front();
		q.pop();

		if (current.isAccepting())
		{
			return true;
		}
		else
		{
			auto configurations = current.getAdjacentConfigurations(this->transitions);
			for each (auto configuration in configurations)
			{
				q.push(configuration);
			}
		}
	}

	return false;
}
