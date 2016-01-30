#pragma once
#include "IRightHandSideElement.h"
#include <set>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Terminal : public IRightHandSideElement
{
public:
	Terminal(char c = '@');

	// constructor for special characters
	// it is of length 2 and begins with '\'
	Terminal(string s);
	~Terminal();

	const char getLabel() const;

	const bool isBlankWord() const;

	friend bool operator<(const Terminal & t, const Terminal & u);
	friend bool operator==(const Terminal & t, const Terminal & u);
	friend bool operator>(const Terminal & t, const Terminal & u);

	const string toString() const;

	const bool isTerminal() const;
	const bool isVariable() const;

	const bool isContainedIn(const RightSide & rs);

	// generates all words from an alphabet of length between n and m and
	// writes it into the result
	static void generateWords(int n, int m, set<Terminal> & alphabet, vector<string> & result);
	static void generateWords(int n, set<Terminal>& alphabet, vector<string>& result);
	static void addLetters(string base, int depth, set<char>& alphabet, vector<string>& result);
private:
	char label;
};

