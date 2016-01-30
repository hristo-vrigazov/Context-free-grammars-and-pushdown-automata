#pragma once
#include "ContextFreeGrammar.h"
#include "PushdownAutomaton.h"

class WordBelongsToLanguageMethodSelector
{
public:
	WordBelongsToLanguageMethodSelector(const ContextFreeGrammar & cfg, const string word);
	~WordBelongsToLanguageMethodSelector();

	const bool usingCYK() const;
	const bool usingPushdownAutomaton() const;

private:
	set<Rule> rules;
	Variable initialVariable;
	string word;
};

