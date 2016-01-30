#pragma once
#include <string>

using namespace std;

class PushdownAutomatonState
{
public:
	PushdownAutomatonState(string label = "");
	~PushdownAutomatonState();

	const string getLabel() const;

	const bool operator==(const PushdownAutomatonState & other) const;

	static PushdownAutomatonState generate();

private:
	string label;

	// for generating states
	static int n;
};

