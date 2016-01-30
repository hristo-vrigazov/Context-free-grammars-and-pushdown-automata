#pragma once
#include "IRightHandSideElement.h"
#include <string>
#include <algorithm>

using namespace std;

class RightSide;

class Variable : public IRightHandSideElement
{
public:
	Variable(string s="A");
	~Variable();

	const string getLabel() const;

	// so that we could make a set of variables
	friend bool operator<(const Variable & v, const Variable & w);
	friend bool operator==(const Variable & v, const Variable & w);
	friend bool operator!=(const Variable & v, const Variable & w);
	friend bool operator>(const Variable & v, const Variable & w);

	friend istream& operator>>(istream& is, Variable & v);

	const bool isContainedIn(const RightSide & rs);

	const string toString() const;

	// parses a string to obtain label of a variable
	static string parseLabel(string raw);

	const bool isTerminal() const;
	const bool isVariable() const;

private:
	string label;
};

