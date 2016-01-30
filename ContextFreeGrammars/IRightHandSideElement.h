#pragma once
#include <string>

using namespace std;

class RightSide;
// interface that all elements in a
// right side of an equation should
// implement
class IRightHandSideElement
{
public:
	virtual const string toString() const = 0;
	virtual const bool isTerminal() const = 0;
	virtual const bool isVariable() const = 0;
	virtual const bool isContainedIn(const RightSide & rs) = 0;
};

const bool areEqual(const IRightHandSideElement * r1, const IRightHandSideElement * r2);