#pragma once
#include "Terminal.h"
#include "Variable.h"
#include "IRightHandSideElement.h"
#include <vector>
#include <set>

using namespace std;

class RightSide
{
public:
	RightSide();
	~RightSide();

	// functions for appending terminals
	void append(const Terminal & t);
	void append(char terminal);

	// functions for appending variables
	void append(const Variable & v);
	void append(string s);

	const bool empty() const;

	const string toString() const;

	const unsigned length() const;
	const unsigned numberOfVariables() const;
	const unsigned numberOfTerminals() const;

	const bool isChomskyRightSide() const;

	const bool consistsOfVariablesOfSet(const set<Variable> & setVariables) const;
	const bool contains(const Variable & v) const;
	const bool contains(const Terminal & t) const;

	const bool startsWith(const IRightHandSideElement * rs) const;

	void withoutVariable(const Variable & v, RightSide & result) const;

	friend bool operator<(const RightSide & r, const RightSide & w);
	friend bool operator>(const RightSide & r, const RightSide & w);
	friend bool operator==(const RightSide & r, const RightSide & w);

	static RightSide parseLabel(string label);

	void getTerminals(set<char> & result) const;
	void getVariables(set<string> & result) const;
	void getVariables(set<Variable> & result) const;

	set<Terminal> getTerminals() const;
	set<Variable> getVariables() const;

	const bool equals(Variable v) const;
	const bool isUnit() const;

	const RightSide replaceBeginning(const RightSide & newBeginning) const;
	void replaceTerminalWithVariable(Terminal t, Variable v);

	const RightSide extractLastTwoVariables() const;
	const RightSide replaceLastTwoVariablesWith(const Variable & v) const;
private:
	vector<IRightHandSideElement*> elements;

	unsigned numOfVariables;
	unsigned numOfTerminals;
};

