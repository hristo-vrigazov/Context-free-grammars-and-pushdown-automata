#pragma once
#include "RightSide.h"
#include "Variable.h"
#include <string>

using namespace std;

class Rule
{
public:

	Rule(const string & s, const RightSide & rightSide = RightSide());
	Rule(const Variable & v, const RightSide & rightSide = RightSide());
	~Rule();

	const Variable & getLeftSide() const;
	const RightSide & getRightSide() const;

	void setLeftSide(Variable v);

	friend bool operator<(const Rule & r, const Rule & w);
	friend bool operator>(const Rule & r, const Rule & w);
	friend bool operator==(const Rule & r, const Rule & w);

	static Rule parseLabel(string toRead);

	void replaceRightSide(Variable v);

	const bool isUnit() const;
	const bool isUseless() const;

	set<Terminal> getTerminals() const;
	set<Variable> getVariables() const;

	void replaceTerminalWithVariableInRightSide(Terminal t, Variable v);

	const RightSide extractLastTwoVariables() const;
	const RightSide replaceLastTwoVariablesWith(const Variable & v) const;

private:
	Variable leftSide;
	RightSide rightSide;
};

