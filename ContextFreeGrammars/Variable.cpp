#include "RightSide.h"
#include "Variable.h"

Variable::Variable(string label) :
	label(label)
{
}


Variable::~Variable()
{
}

const string Variable::getLabel() const
{
	return this->label;
}

const bool Variable::isContainedIn(const RightSide & rs)
{
	return rs.contains(*this);
}

const string Variable::toString() const
{
	return this->getLabel();
}

string Variable::parseLabel(string raw)
{
	/* raw is a string in the format
	* {label}, in which we may escape
	* characters using "\"
	* the characters that could be escaped
	* are \ @ { }
	*/
	//
	string withoutCurlyBraces = raw.substr(1, raw.length() - 2);
	string result = "";

	int length = withoutCurlyBraces.length() - 1;

	for (int i = 0; i < length; i++)
	{
		bool nextCharacterEscapes =
			withoutCurlyBraces[i + 1] == '\\' ||
			withoutCurlyBraces[i + 1] == '@' ||
			withoutCurlyBraces[i + 1] == '{' ||
			withoutCurlyBraces[i + 1] == '}';

		if (withoutCurlyBraces[i] == '\\' && nextCharacterEscapes)
		{
			i++;
		}

		result += withoutCurlyBraces[i];
	}

	result += withoutCurlyBraces[length];

	return result;
}

const bool Variable::isTerminal() const
{
	return false;
}

const bool Variable::isVariable() const
{
	return true;
}

bool operator<(const Variable & v, const Variable & w)
{
	return v.label < w.label;
}

bool operator==(const Variable & v, const Variable & w)
{
	return v.label == w.label;
}

bool operator!=(const Variable & v, const Variable & w)
{
	return !(v == w);
}

bool operator>(const Variable & v, const Variable & w)
{
	return v.label > w.label;
}

istream& operator>>(istream& is, Variable & v)
{
	string rawLabel, parsedLabel;

	is >> rawLabel;
	// do some parsing
	parsedLabel = Variable::parseLabel(rawLabel);
	v.label = parsedLabel;

	return is;
}