#include "ContextFreeGrammar.h"
#include "WordBelongsToLanguageMethodSelector.h"

int ContextFreeGrammar::autoGeneratedVariables = 0;

ContextFreeGrammar::ContextFreeGrammar()
{
}

ContextFreeGrammar::ContextFreeGrammar(const set<Rule>& rules, const Variable & variable)
{
	for each (auto rule in rules)
	{
		this->addRule(rule);
	}

	this->setInitialVariable(variable);
}

ContextFreeGrammar::~ContextFreeGrammar()
{
}

void ContextFreeGrammar::addVariable(const Variable & v)
{
	this->variables.insert(v);
}

void ContextFreeGrammar::addTerminal(const Terminal & t)
{
	this->terminals.insert(t);
}

void ContextFreeGrammar::addRule(const Rule & r)
{
	this->addVariable(r.getLeftSide());

	set<char> newTerminals;

	r.getRightSide().getTerminals(newTerminals);

	for each (char character in newTerminals)
	{
		this->addTerminal(Terminal(character));
	}

	set<string> newVariables;

	r.getRightSide().getVariables(newVariables);

	for each (auto variable in newVariables)
	{
		this->addVariable(variable);
	}

	this->rules.insert(r);
}

void ContextFreeGrammar::setInitialVariable(const Variable & v)
{
	this->addVariable(v);
	this->initialVariable = v;
}

const set<Variable>& ContextFreeGrammar::getVariables() const
{
	return this->variables;
}

const set<Terminal>& ContextFreeGrammar::getTerminals() const
{
	return this->terminals;
}

const set<Terminal>& ContextFreeGrammar::getTerminals(set<Terminal> & terminals) const
{
	for each (auto rule in this->getRules())
	{
		set<char> localTerminals;
		rule.getRightSide().getTerminals(localTerminals);

		for each (auto character in localTerminals)
		{
			terminals.insert(character);
		}
	}
	return terminals;
}

const set<Rule>& ContextFreeGrammar::getRules() const
{
	return this->rules;
}

const Variable ContextFreeGrammar::getInitialVariable() const
{
	return this->initialVariable;
}

const set<Rule>& ContextFreeGrammar::getUnitRules(set<Rule>& result) const
{
	for each (auto rule in this->getRules())
	{
		if (rule.isUnit())
		{
			result.insert(rule);
		}
	}

	return result;
}

WordBelongsToLanguageMethodSelector ContextFreeGrammar::wordBelongsToLanguage(string word) const
{
	return WordBelongsToLanguageMethodSelector(*this, word);
}

const bool ContextFreeGrammar::isChomskyNormal() const
{
	for each (auto rule in this->getRules())
	{
		if (!rule.getRightSide().isChomskyRightSide())
		{
			return false;
		}
	}

	return true;
}

void ContextFreeGrammar::convertToChomskyNormal()
{
	this->epsilonRulesElimination();
	this->eliminateCyclicUnitRules();
	this->eliminateUncyclicUnitRules();
	this->eliminateMixedRules();
	this->eliminateRulesWithLongRightSide();
}

void ContextFreeGrammar::epsilonRulesElimination()
{
	set<Variable> epsilonSet;
	this->extractEpsilonSet(epsilonSet);
	this->addRulesWithoutEpsilonVariables(epsilonSet);
	this->removeRulesWithEmptyRightSide();

	bool initialVariableIsContainedInEpsilonSet = epsilonSet.find(this->initialVariable) != epsilonSet.end();

	if (initialVariableIsContainedInEpsilonSet)
	{
		RightSide rs;
		rs.append(this->initialVariable);
		this->generateVariable(this->initialVariable);
		Rule r(this->initialVariable), w(this->initialVariable, rs);
		this->addRule(r);
		this->addRule(w);
	}
}

void ContextFreeGrammar::eliminateCyclicUnitRules()
{
	set<Rule> newUnitRules;
	set<Rule> oldUnitRules;

	this->getUnitRules(oldUnitRules);
	newUnitRules = oldUnitRules;

	auto stronglyConnectedComponents = this->extractStronglyConnectedComponents(newUnitRules);

	for each (auto stronglyConnectedComponent in stronglyConnectedComponents)
	{
		this->unifyAllVariables(stronglyConnectedComponent, newUnitRules);
	}

	set<Rule> newGrammarRules;

	for each (auto rule in this->getRules())
	{
		// remove old unit rules; we will add the new later
		if (!oldUnitRules.count(rule) && !rule.isUseless())
		{
			newGrammarRules.insert(rule);
		}
	}

	for each (auto rule in newUnitRules)
	{
		if (!rule.isUseless())
		{
			newGrammarRules.insert(rule);
		}
	}

	this->rules = newGrammarRules;
}

void ContextFreeGrammar::eliminateUncyclicUnitRules()
{
	Rule feasibleRule = this->getUnitRuleWithRightSideNotInGraph(this->getRules());

	while (feasibleRule.getLeftSide() != Variable("Rule not found"))
	{
		Variable X = feasibleRule.getLeftSide();
		Variable Y = Variable(feasibleRule.getRightSide().getVariables().begin()->toString());

		set<Rule> rulesThatTerminate = this->getRulesFrom(Y, this->getRules());
		
		for each (auto ruleThatTerminates in rulesThatTerminate)
		{
			auto ruleToAdd = Rule(X, ruleThatTerminates.getRightSide());
			this->rules.insert(ruleToAdd);
		}

		this->removeUnitRulesThatGoTo(Y);

		feasibleRule = this->getUnitRuleWithRightSideNotInGraph(this->getRules());
	}
}

const Rule ContextFreeGrammar::getUnitRuleWithRightSideNotInGraph(const set<Rule>& unitRules)
{
	for each (auto unitRule in unitRules)
	{
		if (unitRule.isUnit())
		{
			Variable rightSide = Variable(unitRule.getRightSide().getVariables().begin()->toString());

			for each (auto rule in this->getRules())
			{
				if (rightSide == rule.getLeftSide() && !rule.isUnit())
				{
					return unitRule;
				}
			}
		}
	}

	return Rule(Variable("Rule not found"));
}

const set<Rule> ContextFreeGrammar::getRulesFrom(const Variable & v, const set<Rule>& rules)
{
	set<Rule> result;

	for each (auto rule in rules)
	{
		if (v == rule.getLeftSide())
		{
			result.insert(rule);
		}
	}

	return result;
}

void ContextFreeGrammar::removeUnitRulesThatGoTo(const Variable & v)
{
	set<Rule> newRules;

	for each (auto rule in this->getRules())
	{
		if (!(rule.isUnit() && 
			Variable(rule.getRightSide().getVariables().begin()->toString()) == v))
		{
			newRules.insert(rule);
		}
	}

	this->rules = newRules;
}

void ContextFreeGrammar::eliminateMixedRules()
{
	set<Terminal> alphabet = this->getTerminals();

	map<Terminal, Variable> replacingVariable;

	set<Rule> newRules;

	for each (auto terminal in alphabet)
	{
		Variable generated;
		ContextFreeGrammar::generateVariable(generated);
		replacingVariable[terminal] = generated;
		RightSide rs;
		rs.append(terminal);
		newRules.insert(Rule(replacingVariable[terminal], rs));
	}

	for each (auto rule in this->getRules())
	{
		if (rule.getRightSide().length() >= 2)
		{
			Rule newRule(rule);
			auto terminalsInRule = rule.getTerminals();

			for each (auto terminalInRule in terminalsInRule)
			{
				newRule.replaceTerminalWithVariableInRightSide(terminalInRule, replacingVariable[terminalInRule]);
			}

			newRules.insert(newRule);
		}

		else
		{
			newRules.insert(rule);
		}
	}

	this->rules = newRules;
}

void ContextFreeGrammar::eliminateRulesWithLongRightSide()
{
	Rule ruleWithLongRightSide = this->existsRuleWithLongRightSide();

	while (ruleWithLongRightSide.getLeftSide() != Variable("Rule not found"))
	{
		Variable newVariable;
		ContextFreeGrammar::generateVariable(newVariable);

		RightSide rs = ruleWithLongRightSide.extractLastTwoVariables();

		Rule newRule(newVariable, rs);

		this->addRule(newRule);

		Rule maintainOldRule(
			ruleWithLongRightSide.getLeftSide(),
			ruleWithLongRightSide.replaceLastTwoVariablesWith(newVariable));

		this->addRule(maintainOldRule);

		this->rules.erase(ruleWithLongRightSide);

		ruleWithLongRightSide = this->existsRuleWithLongRightSide();
	}
}

const Rule ContextFreeGrammar::existsRuleWithLongRightSide() const
{
	for each (auto rule in this->getRules())
	{
		if (rule.getRightSide().length() >= 3)
		{
			return rule;
		}
	}

	return Rule(Variable("Rule not found"));
}

const set<set<Variable>> ContextFreeGrammar::extractStronglyConnectedComponents(const set<Rule>& rules) const
{
	/*
		Tarjan's algorithm for strongly connected components
	*/
	set<set<Variable>> result;

	stack<Variable> s;
	int globalIndex = 0;

	map<Variable, int> index;
	map<Variable, int> lowlink;
	map<Variable, bool> onStack;

	auto unitVariables = this->getUnitVariables(rules);

	for each (auto unitVariable in unitVariables)
	{
		if (!index.count(unitVariable))
		{
			this->tarjanStronglyConnected(unitVariable, unitVariables, rules, s, globalIndex, index, lowlink, onStack, result);
		}
	}

	return result;
}

void ContextFreeGrammar::tarjanStronglyConnected(
	const Variable & unitVariable, 
	const set<Variable>& unitVariables, 
	const set<Rule>& unitRules, 
	stack<Variable>& s, 
	int & globalIndex, 
	map<Variable, int> & index, 
	map<Variable, int> & lowlink, 
	map<Variable, bool> & onStack,
	set<set<Variable>> & result) const
{
	index[unitVariable] = globalIndex;
	lowlink[unitVariable] = globalIndex;
	globalIndex++;
	s.push(unitVariable);
	onStack[unitVariable] = true;

	for each (auto rule in unitRules)
	{
		if (rule.getLeftSide() == unitVariable)
		{
			auto successor = Variable(rule.getRightSide().getVariables().begin()->toString());
			
			if (!index.count(successor))
			{
				this->tarjanStronglyConnected(successor, unitVariables, unitRules, s, globalIndex, index, lowlink, onStack, result);
				lowlink[unitVariable] = min(lowlink[unitVariable], lowlink[successor]);
			}

			else if (onStack[successor])
			{
				lowlink[unitVariable] = min(lowlink[unitVariable], index[successor]);
			}
		}
	}

	/*
	if (v.lowlink = v.index) then
	start a new strongly connected component
	repeat
	w := S.pop()
	w.onStack := false
	add w to current strongly connected component
	while (w != v)
	output the current strongly connected component
	end if
	end function
	*/
	if (lowlink[unitVariable] == index[unitVariable])
	{
		set<Variable> newStronglyConnectedComponent;
		newStronglyConnectedComponent.insert(unitVariable);

		Variable w;
		do
		{
			w = s.top();
			s.pop();
			onStack[w] = false;
			newStronglyConnectedComponent.insert(w);
		} while (w != unitVariable);

		result.insert(newStronglyConnectedComponent);
	}
}

const set<Variable> ContextFreeGrammar::getUnitVariables(const set<Rule>& rules) const
{
	set<Variable> result;

	for each (auto rule in rules)
	{
		result.insert(rule.getLeftSide());
		auto variablesFromRightSide = rule.getRightSide().getVariables();
		result.insert(variablesFromRightSide.begin(), variablesFromRightSide.end());
	}

	return result;
}

const Variable ContextFreeGrammar::existsRuleThatGoesToEpsilonSet(const set<Variable>& epsilonSet) const
{
	for each (auto rule in this->getRules())
	{
		bool variableDoesNotBelongToEpsilonSet = epsilonSet.find(rule.getLeftSide()) == epsilonSet.end();
		bool rightSideIsMadeOfEpsilonSet = rule.getRightSide().consistsOfVariablesOfSet(epsilonSet);

		if (variableDoesNotBelongToEpsilonSet && rightSideIsMadeOfEpsilonSet)
		{
			return rule.getLeftSide();
		}
	}

	return Variable("Rule not found");
}

const Rule & ContextFreeGrammar::existsRuleContainingVariableFromEpsilonSet(const set<Variable>& epsilonSet, Rule & result) const
{
	for each (auto rule in this->getRules())
	{
		for each (auto variable in epsilonSet)
		{
			if (rule.getRightSide().contains(variable))
			{
				RightSide newRightSide;
				rule.getRightSide().withoutVariable(variable, newRightSide);
				result = Rule(rule.getLeftSide(), newRightSide);

				bool isAlreadyContained = this->getRules().find(result) != this->getRules().end();

				if (!isAlreadyContained)
				{
					return result;
				}
			}
		}
	}

	result = Rule("Rule not found");
	return result;
}

void ContextFreeGrammar::unifyAllVariables(const set<Variable>& variables, set<Rule>& rules)
{
	Variable referenceVariable = *variables.begin();

	for each (auto variable in variables)
	{
		this->replaceVariableWith(variable, referenceVariable, rules);
	}
}

void ContextFreeGrammar::replaceVariableWith(const Variable & v, const Variable & replacing, set<Rule> & rules)
{
	set<Rule> result;
	for each (auto rule in rules)
	{
		if (rule.getLeftSide() == v)
		{
			rule.setLeftSide(replacing);
		}

		if (rule.getRightSide().equals(v))
		{
			rule.replaceRightSide(replacing);
		}
		result.insert(rule);
	}

	rules = result;
}

const set<Variable>& ContextFreeGrammar::extractEpsilonSet(set<Variable>& epsilonSet) const
{
	Variable variableToBeAdded = this->existsRuleThatGoesToEpsilonSet(epsilonSet);

	while (variableToBeAdded != Variable("Rule not found"))
	{
		epsilonSet.insert(variableToBeAdded);
		variableToBeAdded = this->existsRuleThatGoesToEpsilonSet(epsilonSet);
	}

	return epsilonSet;
}

void ContextFreeGrammar::addRulesWithoutEpsilonVariables(set<Variable>& epsilonSet)
{
	Rule resultRule("Initial");
	this->existsRuleContainingVariableFromEpsilonSet(epsilonSet, resultRule);

	while (resultRule.getLeftSide() != Rule("Rule not found").getLeftSide())
	{
		this->addRule(resultRule);
		this->existsRuleContainingVariableFromEpsilonSet(epsilonSet, resultRule);
	}
}

void ContextFreeGrammar::removeRulesWithEmptyRightSide()
{
	set<Rule> newRules;

	for each (auto rule in this->getRules())
	{
		if (!rule.getRightSide().empty())
		{
			newRules.insert(rule);
		}
	}

	this->rules = newRules;
}

const Variable & ContextFreeGrammar::generateVariable(Variable & v) const
{
	v = Variable("AUTO " + std::to_string(ContextFreeGrammar::autoGeneratedVariables));

	autoGeneratedVariables++;
	return v;
}

void ContextFreeGrammar::createAsUnionOf(const ContextFreeGrammar & cfg1, const ContextFreeGrammar & cfg2)
{
	for each (auto rule in cfg1.getRules())
	{
		this->addRule(rule);
	}

	for each (auto rule in cfg2.getRules())
	{
		this->addRule(rule);
	}

	Variable newInitial;
	ContextFreeGrammar::generateVariable(newInitial);

	RightSide rightSideFirstNewRule;
	rightSideFirstNewRule.append(cfg1.getInitialVariable());
	Rule ruleToFirstGrammarInitialVariable(newInitial, rightSideFirstNewRule);

	RightSide rightSideSecondNewRule;
	rightSideSecondNewRule.append(cfg2.getInitialVariable());
	Rule ruleToSecondGrammarInitialVariable(newInitial, rightSideSecondNewRule);

	this->addRule(ruleToFirstGrammarInitialVariable);
	this->addRule(ruleToSecondGrammarInitialVariable);

	this->setInitialVariable(newInitial);
}

void ContextFreeGrammar::createAsConcatenationOf(const ContextFreeGrammar & cfg1, const ContextFreeGrammar & cfg2)
{
	for each (auto rule in cfg1.getRules())
	{
		this->addRule(rule);
	}

	for each (auto rule in cfg2.getRules())
	{
		this->addRule(rule);
	}

	Variable newInitial;
	ContextFreeGrammar::generateVariable(newInitial);

	RightSide rs;
	rs.append(cfg1.getInitialVariable());
	rs.append(cfg2.getInitialVariable());
	Rule newRule(newInitial, rs);

	this->addRule(newRule);
}

void ContextFreeGrammar::createAsIterationOf(const ContextFreeGrammar & cfg)
{
	for each (auto rule in cfg.getRules())
	{
		if (!(rule.getLeftSide() == cfg.getInitialVariable() && rule.getRightSide().empty()))
		{
			this->addRule(rule);
		}
	}

	Variable newInitial;
	ContextFreeGrammar::generateVariable(newInitial);

	this->setInitialVariable(newInitial);
	this->addRule(Rule(newInitial));

	RightSide rs;
	rs.append(cfg.getInitialVariable());
	this->addRule(Rule(newInitial, rs));

	rs.append(cfg.getInitialVariable());
	this->addRule(Rule(newInitial, rs));
}

const bool ContextFreeGrammar::languageIsEmpty() const
{
	set<Variable> Marked;

	for each (auto rule in this->getRules())
	{
		set<char> terminals;
		rule.getRightSide().getTerminals(terminals);

		for each (auto character in terminals)
		{
			Marked.insert(Variable(std::to_string(character)));
		}
	}

	Variable toAdd = this->existsRuleThatGoesToEpsilonSet(Marked);

	while (toAdd != Variable("Rule not found"))
	{
		Marked.insert(toAdd);
		toAdd = this->existsRuleThatGoesToEpsilonSet(Marked);
	}

	return Marked.find(this->initialVariable) == Marked.end();
}

const bool ContextFreeGrammar::languageIsFinite() const
{
	ContextFreeGrammar cfg(*this);

	if (!cfg.isChomskyNormal())
	{
		cfg.convertToChomskyNormal();
	}

	unsigned n = cfg.getRules().size();

	set<Terminal> alphabet;
	this->getTerminals(alphabet);

	vector<string> words;
	Terminal::generateWords(n, 2 * n, alphabet, words);

	for each (auto word in words)
	{
		if (this->wordBelongsToLanguage(word).usingPushdownAutomaton())
		{
			// language is infinite
			return false;
		}
	}

	return true;
}

ContextFreeGrammar & operator<<(ContextFreeGrammar & cfg, string toRead)
{
	// indicates we are setting an initial variable
	if (toRead[0] == 'I')
	{
		string parsedNameOfVariable = Variable::parseLabel(toRead.substr(1, toRead.length() - 1));
		cfg.setInitialVariable(Variable(parsedNameOfVariable));
		return cfg;
	}

	// else we are dealing with a string in the format
	// {VariableName}->RightSide

	Rule newRule = Rule::parseLabel(toRead);

	cfg.addRule(newRule);

	return cfg;
}
