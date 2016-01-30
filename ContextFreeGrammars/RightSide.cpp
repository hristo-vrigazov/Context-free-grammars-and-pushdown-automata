#include "RightSide.h"

RightSide::RightSide() :
	numOfVariables(0),
	numOfTerminals(0)
{
}

RightSide::~RightSide()
{
}

void RightSide::append(const Terminal & t)
{
	IRightHandSideElement* elementToPush = new Terminal(t);
	this->elements.push_back(elementToPush);
	this->numOfTerminals++;
}

void RightSide::append(char terminal)
{
	this->append(Terminal(terminal));
}

void RightSide::append(const Variable & v)
{
	IRightHandSideElement* elementToPush = new Variable(v);
	this->elements.push_back(elementToPush);
	this->numOfVariables++;
}

void RightSide::append(string s)
{
	this->append(Variable(s));
}

const bool RightSide::empty() const
{
	if (this->elements.empty())
	{
		return true;
	}

	else
	{
		if (this->numberOfTerminals() == this->length())
		{
			for each (auto terminal in this->elements)
			{
				if (terminal->toString() != Terminal('@').toString())
				{
					return false;
				}
			}

			return true;
		}
	}

	return false;
}

const string RightSide::toString() const
{
	string s = "";

	for each (auto element in this->elements)
	{
		element->isTerminal() ? s += element->toString() : s += ("{" + element->toString() + "}");
	}

	return s;
}

const unsigned RightSide::length() const
{
	return this->numberOfTerminals() + this->numberOfVariables();
}

const unsigned RightSide::numberOfVariables() const
{
	return this->numOfVariables;
}

const unsigned RightSide::numberOfTerminals() const
{
	return this->numOfTerminals;
}

const bool RightSide::isChomskyRightSide() const
{
	if (this->length() == 1 && this->numberOfTerminals() == 1)
	{
		return true;
	}

	if (this->length() == 2 && this->numberOfVariables() == 2)
	{
		return true;
	}

	return false;
}

const bool RightSide::consistsOfVariablesOfSet(const set<Variable>& setVariables) const
{
	for each (auto element in this->elements)
	{
		bool elementIsPresentInSetVariables = false;

		for each (auto var in setVariables)
		{
			if (var.toString() == element->toString())
			{
				elementIsPresentInSetVariables = true;
				break;
			}
		}

		if (!elementIsPresentInSetVariables)
		{
			return false;
		}
	}

	return true;
}

const bool RightSide::contains(const Variable & v) const
{
	for each (auto element in this->elements)
	{
		if (element->isVariable() &&
			element->toString() == v.toString())
		{
			return true;
		}
	}

	return false;
}

const bool RightSide::contains(const Terminal & t) const
{
	for each (auto element in this->elements)
	{
		if (element->isTerminal() &&
			element->toString() == t.toString())
		{
			return true;
		}
	}

	return false;
}

void RightSide::withoutVariable(const Variable & v, RightSide & result) const
{
	for each (auto element in this->elements)
	{
		if (!(element->isVariable() && element->toString() == v.toString()))
		{
			result.elements.push_back(element);
			
			element->isTerminal() ? result.numOfTerminals++ : result.numOfVariables++;
		}
	}
}

bool operator<(const RightSide & r, const RightSide & w)
{
	if (w.length() == 0 && r.length() == 0)
	{
		return false;
	}

	for (unsigned i = 0; i < r.length(); i++)
	{
		if (i < w.length())
		{
			// both r[i] and w[i] exist
			if (r.elements[i]->isVariable())
			{
				if (w.elements[i]->isVariable())
				{
					if (r.elements[i]->toString() != w.elements[i]->toString())
					{
						return r.elements[i]->toString() < w.elements[i]->toString();
					}

					else
					{
						continue;
					}
				}

				// r > w
				else
				{
					return false;
				}
			}

			if (r.elements[i]->isTerminal())
			{
				// r < w
				if (w.elements[i]->isVariable())
				{
					return true;
				}

				else
				{
					if (r.elements[i]->toString() != w.elements[i]->toString())
					{
						return r.elements[i]->toString() < w.elements[i]->toString();
					}
					else
					{
						continue;
					}
				}
			}
		}

		// r[i] does not have equivalent element, this means r > w
		else
		{
			return false;
		}
	}

	// if we got to this point, all is equal
	if (w.length() == r.length())
	{
		return false;
	}

	// else, the r is longer than w
	return true;
}

bool operator>(const RightSide & r, const RightSide & w)
{
	return w < r;
}

bool operator==(const RightSide & r, const RightSide & w)
{
	return !(r < w) && !(r > w);
}

RightSide RightSide::parseLabel(string label)
{
	if (label.length() == 1 && label[0] == '@')
	{
		return RightSide();
	}

	RightSide rs;

	bool readingVariable = (label[0] == '{');

	string tempVariable = "";

	for (unsigned i = 0; i < label.length(); i++)
	{
		if (label[i] == '{')
		{
			readingVariable = true;
			continue;
		}

		if (label[i] == '}')
		{
			readingVariable = false;
			rs.append(Variable(tempVariable));
			tempVariable = "";
			continue;
		}

		if (readingVariable)
		{
			tempVariable += label[i];
		}
		else
		{
			rs.append(Terminal(label[i]));
		}
	}

	return rs;
}

void RightSide::getTerminals(set<char> & result) const
{
	for each (auto element in this->elements)
	{
		if (element->isTerminal())
		{
			result.insert(element->toString()[0]);
		}
	}
}

void RightSide::getVariables(set<string>& result) const
{
	for each (auto element in this->elements)
	{
		if (element->isVariable())
		{
			result.insert(element->toString());
		}
	}
}

void RightSide::getVariables(set<Variable> & result) const
{
	for each (auto element in this->elements)
	{
		if (element->isVariable())
		{
			result.insert(Variable(element->toString()));
		}
	}
}

const bool RightSide::isUnit() const
{
	return this->elements.size() == 1 && (*this->elements.begin())->isVariable();
}

const bool RightSide::equals(Variable v) const
{
	if (this->isUnit() && v.toString() == (*this->elements.begin())->toString())
	{
		return true;
	}

	return false;
}

set<Terminal> RightSide::getTerminals() const
{
	set<Terminal> result;

	for each (auto element in this->elements)
	{
		if (element->isTerminal())
		{
			result.insert(Terminal(element->toString()[0]));
		}
	}

	return result;
}

set<Variable> RightSide::getVariables() const
{
	set<Variable> result;

	for each (auto element in this->elements)
	{
		if (element->isVariable())
		{
			result.insert(Variable(element->toString()));
		}
	}

	return result;
}

const bool RightSide::startsWith(const IRightHandSideElement * rs) const
{
	if (this->empty())
	{
		return false;
	}

	// all words start with the empty word
	if (rs->isTerminal() && rs->toString() == "")
	{
		return true;
	}

	return areEqual(this->elements[0], rs);
}

const RightSide RightSide::replaceBeginning(const RightSide & newBeginning) const
{
	if (this->empty())
	{
		return newBeginning;
	}

	auto copyElements = this->elements;
	copyElements.erase(copyElements.begin());
	copyElements.insert(copyElements.begin(), newBeginning.elements.begin(), newBeginning.elements.end());
	
	RightSide result;

	for each (auto element in copyElements)
	{
		if (element->isTerminal())
		{
			result.append(Terminal(element->toString()[0]));
		}

		else
		{
			result.append(Variable(element->toString()));
		}
	}

	return result;
}

void RightSide::replaceTerminalWithVariable(Terminal t, Variable v)
{
	vector<IRightHandSideElement*> newElements;

	this->numOfVariables = 0;
	this->numOfTerminals = 0;

	for each (auto element in this->elements)
	{
		IRightHandSideElement * toAdd;

		if (element->isTerminal() &&
			Terminal(element->toString()[0]) == t)
		{ 
			toAdd = new Variable(v);
			this->numOfVariables++;
		}

		else
		{
			toAdd = element;
			element->isTerminal() ? numOfTerminals++ : numOfVariables++;
		}

		newElements.push_back(toAdd);
	}

	this->elements = newElements;
}

const RightSide RightSide::extractLastTwoVariables() const
{
	RightSide result;

	result.append(Variable(this->elements[this->elements.size() - 2]->toString()));
	result.append(Variable(this->elements[this->elements.size() - 1]->toString()));

	return result;
}

const RightSide RightSide::replaceLastTwoVariablesWith(const Variable & v) const
{
	RightSide rs;

	for (auto it = this->elements.begin(); it != this->elements.end() - 2; it++)
	{
		auto current = (*it);

		if (current->isTerminal())
		{
			rs.append(Terminal(current->toString()[0]));
		}

		if (current->isVariable())
		{
			rs.append(Variable(current->toString()));
		}
	}

	rs.append(v);

	return rs;
}