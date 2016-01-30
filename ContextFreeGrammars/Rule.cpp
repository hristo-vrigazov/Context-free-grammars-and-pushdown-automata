#include "Rule.h"



Rule::Rule(const string & s, const RightSide & rightSide) :
	leftSide(s),
	rightSide(rightSide)
{
}

Rule::Rule(const Variable & v, const RightSide & rightSide) :
	leftSide(v), rightSide(rightSide)
{
}

Rule::~Rule()
{
}

const Variable & Rule::getLeftSide() const
{
	return this->leftSide;
}

const RightSide & Rule::getRightSide() const
{
	return this->rightSide;
}

void Rule::setLeftSide(Variable v)
{
	this->leftSide = v;
}

Rule Rule::parseLabel(string toRead)
{
	string firstPart = "";
	string secondPart = "";

	bool writingInFirstPart = true;
	for (unsigned i = 0; i < toRead.length() - 1; i++)
	{
		if (toRead[i] == '-' && toRead[i + 1] == '>')
		{
			writingInFirstPart = false;
			i++;
			continue;
		}

		writingInFirstPart ? firstPart += toRead[i] : secondPart += toRead[i];
	}

	secondPart += toRead[toRead.length() - 1];

	Variable leftSide(Variable::parseLabel(firstPart));
	RightSide rightSide = RightSide::parseLabel(secondPart);
	Rule r(leftSide, rightSide);
	return r;
}

void Rule::replaceRightSide(Variable v)
{
	RightSide newRightSide;
	newRightSide.append(v);

	this->rightSide = newRightSide;
}

const bool Rule::isUnit() const
{
	return this->getRightSide().isUnit();
}

const bool Rule::isUseless() const
{
	return this->isUnit() && (this->getRightSide().equals(this->getLeftSide()));
}

set<Terminal> Rule::getTerminals() const
{
	return this->getRightSide().getTerminals();
}

set<Variable> Rule::getVariables() const
{
	set<Variable> result;
	result.insert(this->getLeftSide());

	set<Variable> variablesInRightSide = this->getRightSide().getVariables();
	result.insert(variablesInRightSide.begin(), variablesInRightSide.end());
	return result;
}

void Rule::replaceTerminalWithVariableInRightSide(Terminal t, Variable v)
{
	this->rightSide.replaceTerminalWithVariable(t, v);
}

const RightSide Rule::extractLastTwoVariables() const
{
	return this->getRightSide().extractLastTwoVariables();
}

const RightSide Rule::replaceLastTwoVariablesWith(const Variable & v) const
{
	return this->getRightSide().replaceLastTwoVariablesWith(v);
}

bool operator<(const Rule & r, const Rule & w)
{
	if (r.getLeftSide() == w.getLeftSide())
	{
		return r.getRightSide() < w.getRightSide();
	}

	return r.getLeftSide() < w.getLeftSide();
}

bool operator==(const Rule & r, const Rule & w)
{
	return !(r < w) && !(r > w);
}

bool operator>(const Rule & r, const Rule & w)
{
	return w < r;
}