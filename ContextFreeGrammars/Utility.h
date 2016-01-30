#pragma once
#include <set>
#include "Rule.h"

class Utility
{
public:
	static set<Terminal> getTerminals(const set<Rule> & rules);
};

