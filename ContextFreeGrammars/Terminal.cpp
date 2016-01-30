#include "Terminal.h"
#include "RightSide.h"


Terminal::Terminal(char c)
{
	this->label = c;
}

Terminal::Terminal(string s)
{
	(s.length() == 2 && s[0] == '\\') ? this->label = s[1] : this->label = '@';
}

Terminal::~Terminal()
{
}

const char Terminal::getLabel() const
{
	return this->label;
}

const bool Terminal::isBlankWord() const
{
	return this->getLabel() == '@';
}

const string Terminal::toString() const
{
	if (this->isBlankWord())
	{
		return "";
	}

	return string(1, this->getLabel());
}

const bool Terminal::isTerminal() const
{
	return true;
}

const bool Terminal::isVariable() const
{
	return false;
}

const bool Terminal::isContainedIn(const RightSide & rs)
{
	return rs.contains(*this);
}

bool operator<(const Terminal & t, const Terminal & u)
{
	return t.getLabel() < u.getLabel();
}

bool operator==(const Terminal & t, const Terminal & u)
{
	return t.getLabel() == u.getLabel();
}

bool operator>(const Terminal & t, const Terminal & u)
{
	return t.getLabel() > u.getLabel();
}

void Terminal::generateWords(int n, int m, set<Terminal>& alphabet, vector<string>& result)
{
	for (int i = n; i < m; i++)
	{
		Terminal::generateWords(i, alphabet, result);
	}
}

void Terminal::generateWords(int n, set<Terminal>& alphabet, vector<string>& result)
{
	string startString = "";
	set<char> alphabetAsCharacters;

	for each (auto terminal in alphabet)
	{
		alphabetAsCharacters.insert(terminal.getLabel());
	}

	Terminal::addLetters(startString, n, alphabetAsCharacters, result);
}

void Terminal::addLetters(string base, int depth, set<char> & alphabet, vector<string>& result)
{
	if (base.length() == depth) 
	{
		result.push_back(base);
		return;
	}

	for each (auto letter in alphabet)
	{
		stringstream ss;
		ss << letter;
		addLetters(base + ss.str(), depth, alphabet, result);
	}
}